#include <iostream>
#include <string>
#include "Imagen/ImageDownloader.hpp"
#include "Imagen/CImg.h"
#include "../Socket/Socket.hpp"
#include <thread>
#include <cstring> //manejo de cadenas tipo C

using namespace std;
using namespace cimg_library;

/* --VARIABLES GLOBALES DE USO COMPARTIDO ENTRE PROCESOS*/
bool aceptarPujadores=true; //Booleano para admitir la entrada de nuevos clientes a la subasta.
bool terminar=false;

//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd) {
	while(salaDeEspera);
	char MENS_FIN[]="STOP";
	// Buffer para recibir el mensaje
	int length = 100;
	char buffer[length];

	bool out = false; // Inicialmente no salir del bucle
	while(!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}

		cout << "Mensaje recibido: " << buffer << endl;

		// Si recibimos "END OF SERVICE" --> Fin de la comunicación
		if (0 == strcmp(buffer, MENS_FIN)) {
			out = true; // Salir del bucle
		} else {
			// Contamos las vocales recibidas en el mensaje anterior
			int num_vocales = cuentaVocales(buffer);

			// Enviamos la respuesta
			string s = to_string(num_vocales);
			const char* message = s.c_str();

			int send_bytes = soc.Send(client_fd, message);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
    			cerr << "Error al enviar datos: " + mensError + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
				exit(1);
			}
		}
	}
	soc.Close(client_fd);
}
//-------------------------------------------------------------
int main(int argc, char *argv[]) {
	const int N = 10;
	// Dirección y número donde escucha el proceso servidor
	if(argc!=3){
		cout << "Numero de argumentos incorrecto" << endl;
		cout << "ARGUMENTOS: <server_adress> <server_port>" << endl;
	}
    string SERVER_ADDRESS = argv[1];
    int SERVER_PORT = atoi(argv[2]);
    thread cliente[N];
    int client_fd[N];

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	while(!terminar){

	}
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

	for (int i=0; i<max_connections && aceptarPujadores; i++) {
		// Accept
		client_fd[i] = socket.Accept();

		if(client_fd[i] == -1) {
			string mensError(strerror(errno));
    		cerr << "Error en el accept: " + mensError + "\n";
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}

		cout << "Lanzo thread nuevo cliente " + to_string(i) + "\n";
		cliente[i] = thread(&servCliente, ref(socket), client_fd[i]);
		cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
		annadirPujador(); //incrementa en un unidad el número de pujadores
	}

	//¿Qué pasa si algún thread acaba inesperadamente?
	for (int i=0; i<max_connections; i++) {
		cliente[i].join();
	}

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
