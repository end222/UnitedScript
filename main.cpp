#include <iostream>
#include <string>
#include "Imagen/ImageDownloader.hpp"
#include "Imagen/CImg.h"
#include "Socket/Socket.hpp"
#include "Monitor/monitor.hpp"
#include "GestorVallas/gestor.hpp"
#include "Administrador/admin.hpp"
#include <thread>
#include <cstring> //manejo de cadenas tipo C

using namespace std;
using namespace cimg_library;

/* --VARIABLES GLOBALES DE USO COMPARTIDO ENTRE PROCESOS*/
bool aceptarPujadores=true; //Booleano para admitir la entrada de nuevos clientes a la subasta.
bool terminar=false;
Control control;
subasta subastaActual;

void comenzarInscripcion(){
	control.iniciarInscripcion();
}

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd, int numCliente) {
	control.esperarComienzo();

	char MENS_CANCEL[]="REJECT";
	char MENS_OK[]="ACCEPT";
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];
	string message;
	string mostrarMens;

	while(!control.finSubastas()){

		//Inicio de la subasta
		message="START";
		
		int send_bytes = soc.Send(client_fd, message);
		if(send_bytes == -1) {
			string mensError(strerror(errno));
			cerr << "Error al enviar datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}

		bool finSubasta=false;

		while(!finSubasta){
			message=to_string(subastaActual.obtenerPrecioActual()); //Precio de la subasta
			int send_bytes = soc.Send(client_fd, message);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
				cerr << "Error al enviar datos: " + mensError + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
			}


			// Recibimos el mensaje del cliente
			int rcv_bytes = soc.Recv(client_fd,buffer,length);
			if (rcv_bytes == -1) {
				string mensError(strerror(errno));
				cerr << "Error al recibir datos: " + mensError + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
			}

			mostrarMens = "Mensaje recibido del cliente " + to_string(numCliente) + ": " + buffer;
			control.mostrar(mostrarMens);
            
			/*
			 * Código:
			 * 0: Ha rechazado
			 * 1: Ha aceptado pero aún no ha ganado
			 * 2: Ha aceptado, ha ganado, pero no llega al precio de reserva
			 * 3: Ha aceptado, ha ganado y llega al precio de reserva
			 */
			if (0 == strcmp(buffer, MENS_OK)) {
				control.anadirAcepta(subastaActual);

				int numAceptan = control.numeroPujadoresAceptan();
				if(numAceptan == 1){//Es el unico que queda
					finSubasta = true; // Salir del bucle
					if(subastaActual.obtenerPrecioActual()>=subastaActual.obtenerPrecioDeReserva()){
						message="3"; //es el ganador
					}
					else{ // no consigue la subasta al no llegar al precio
						message="2";
						control.terminaRonda(subastaActual);
						control.esperarFinSubasta();
					}
				}
				else{
					//Acepta pero NO es el último cliente
					message="1";
				}

			}
			//MENSAJE ANULADO
			else{
				//El cliente rechaza a voluntad la subasta
				control.anadirRechaza(subastaActual);
				finSubasta = true; // Salir del bucle
				message="0";
				if(control.numeroPujadoresAceptan()==0){
					control.terminaRonda(subastaActual);
					control.comprobarFin();
					control.esperarFinSubasta();
				}
				else{			
					control.terminaRonda(subastaActual);
					control.esperarFinSubasta();//Esperar a que todos terminen la subasta
				}
			}
			
			send_bytes = soc.Send(client_fd, message);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
				cerr << "Error al enviar datos: " + mensError + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
			}
			if(message == "3"){
				//mete en la cola los datos del cliente y la venta
				rcv_bytes = soc.Recv(client_fd,buffer,length);
				if (rcv_bytes == -1) {
					string mensError(strerror(errno));
					cerr << "Error al recibir datos: " + mensError + "\n";
					// Cerramos los sockets
					soc.Close(client_fd);
					}
				int tiempo = subastaActual.obtenerTiempo();
				int precio = subastaActual.obtenerPrecioActual();
				datosValla datos;
				control.generaDatos(datos, numCliente, tiempo, precio, buffer);
				control.colaPush(datos);
				control.terminaRonda(subastaActual);
				control.comprobarFin();
				control.esperarFinSubasta();
			}
			else if(message == "1"){
				control.terminaRonda(subastaActual);
			}
		}

	}
 	message="END";
	int send_bytes = soc.Send(client_fd, message);
	if(send_bytes == -1) {
		string mensError(strerror(errno));
		cerr << "Error al enviar datos: " + mensError + "\n";
		// Cerramos los sockets
		soc.Close(client_fd);
	}
	soc.Close(client_fd);
}
//-------------------------------------------------------------
int main(int argc, char *argv[]) {
	const int N = 10;
	// Dirección y número donde escucha el proceso servidor
	if(argc!=2){
		cout << "Numero de argumentos incorrecto" << endl;
		cout << "ARGUMENTOS:<server_port>" << endl;
	}
	int SERVER_PORT = atoi(argv[1]);
	thread cliente[N];
	int client_fd[N];

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);

	// Bind
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el bind: " + mensError + "\n";
		exit(1);
	}

	// Listen
	int max_connections = N;
	int error_code = socket.Listen(max_connections);
	if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error en el listen: " + mensError + "\n";
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	thread th_inscripcion(&comenzarInscripcion);
	thread th_administrador(&procesoAdministrador, ref(control));
	thread th_vallas(&procesoGestorVallas, ref(control));
	for (int i=0; i<max_connections && control.seguirAceptando(); i++) {
		// Accept
		client_fd[i] = socket.Accept();
		if(control.seguirAceptando()){
			if(client_fd[i] == -1) {
				string mensError(strerror(errno));
    			cerr << "Error en el accept: " + mensError + "\n";
				// Cerramos el socket
				socket.Close(socket_fd);
				exit(1);
			}
			cliente[i] = thread(&servCliente, ref(socket), client_fd[i], i);
			cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
			control.annadirPujador(); //incrementa en un unidad el número de pujadores
		}
	}
	cout << "0" << endl;
	th_inscripcion.join();
	cout << "1" << endl;
	//¿Qué pasa si algún thread acaba inesperadamente?
	for (int i=0; i<max_connections; i++) {
		cliente[i].join();
	}
	cout << "A" << endl;
	th_administrador.join();
	cout << "B" << endl;
	control.avisarFinGestor();
	cout << "C" << endl;
	control.notifyCola();
	cout << "D" << endl;
	th_vallas.join();
	cout << "E" << endl;

    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if (error_code == -1) {
		string mensError(strerror(errno));
    	cerr << "Error cerrando el socket del servidor: " + mensError + "\n";
	}
	// Mensaje de despedida
	cout << "Bye bye" << endl;

    return error_code;
}
//-------------------------------------------------------------
