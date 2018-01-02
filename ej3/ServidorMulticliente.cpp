//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: Daniel Naval
// Date:   Diciembre 2017   
//*****************************************************************

#include "Socket.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>          // std::mutex
#include <cstring> //manejo de cadenas tipo C
int avion [10][4];
int numElementos=0;
mutex mtx;
using namespace std;




string libres(){
	string free;
	int i=0;
	int j=0;
	while (j<4){
		while (i<10){
			if (avion[i][j]==0){
			free=free+" ("+to_string(i)+","+to_string(j)+")";
			}
			i++;
		}
		i=0;
		j++;
	}
	return free;

}




//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd) {
	char MENS_FIN[]="END OF SERVICE";
	// Buffer para recibir el mensaje
	int length = 2000;
	char buffer[length];
	int fila;
	int columna;
	string ocupados;
	char *data;
	int cliente;
	int status;
	bool out = false; // Inicialmente no salir del bucle
	int vecesIntentado=0;
	while(!out) {
		// Recibimos el mensaje del cliente
		int rcv_bytes = soc.Recv(client_fd,buffer,length);
		if (rcv_bytes == -1) {
			string mensError(strerror(errno));
    		cerr << "Error al recibir datos: " + mensError + "\n";
			// Cerramos los sockets
			soc.Close(client_fd);
		}
		data=strtok(buffer," ");
		fila=atoi(data);
		data=strtok(NULL, " ");
		columna=atoi(data);
		data=strtok(NULL, " ");
		cliente=atoi(data);
		mtx.lock();
		cout << "Cliente " << cliente <<  " solicita fila: " << fila << " y columna: " << columna << endl;
		// Comprobamos si está libre o no
		if (numElementos<40){
			if (avion[fila][columna]==0){
				avion[fila][columna]=1;
				cout << "Asiento RESERVADO" << endl;
				numElementos++;
				status=0; //ESTADO_RESERVADO
				out=true;
			}
			else{
				cout << "Asiento OCUPADO" << endl;
				status=1; //ESTADO_OCUPADO
				vecesIntentado++;
				if (vecesIntentado==4){
					out=true;
				}
			}
		}
		else{
			cout << "AVION LLENO" << endl;
			status=3; //ESTADO_LLENO
			out=true;
		}
		
			// Enviamos la respuesta
			ocupados=libres();
			string Msj=to_string(status)+","+ocupados;
		mtx.unlock();
			int send_bytes = soc.Send(client_fd, Msj);
			if(send_bytes == -1) {
				string mensError(strerror(errno));
    			cerr << "Error al enviar datos: " + mensError + "\n";
				// Cerramos los sockets
				soc.Close(client_fd);
				exit(1);
			}
		}
		soc.Close(client_fd);
	}
	
	//cout << "Cliente Caido" << endl;
//-------------------------------------------------------------
int main(int argc, char * argv[]) {	
	const int N = 120;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
  int SERVER_PORT;
	// Puerto donde escucha el proceso servidor
		if (argc != 2) {
		printf("Número de parámetros incorrecto \n");
		printf("Uso : %s puerto\n", argv[0]);
		exit(1); //sale del programa indicando salida incorrecta (1)
	}
    else{
		SERVER_PORT = atoi(argv[1]);
	}
	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);
    thread cliente[N];
    int client_fd[N];
	int i=0;
	int j=0;
	while (j<4){
		while (i<10){
			avion[i][j]=0;
			i++;
		}
		i=0;
		j++;
	}
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
	cout << "Servidor corriendo en puerto: " << SERVER_PORT << endl;
	
	
	for (int i=0; i<max_connections; i++) {
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
	}
	
	//¿Qué pasa si algún thread acaba inesperadamente?
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
