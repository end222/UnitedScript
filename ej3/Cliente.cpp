//******************************************************************
// File:   Cliente.cpp
// Author: Daniel Naval
// Date:   Diciembre 2017   
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "Socket.hpp"

using namespace std;
const int N=100;
const int FILAS = 10;
const int COLUMNAS = 4;
const int MESSAGE_SIZE = 2000; //mensajes de no más 4000 caracteres

const string MENS_FIN("END OF SERVICE");

void inCliente(Socket& socket, int socket_fd, int cliente){
	

	int max_connections = N;
	int vFila;
	int vColumna;
	srand(cliente);
	
	
	
    string mensaje;
	int i=0;
	bool salir=false;
	int estado=1;
	
	
	while (estado==1){
		string pantalla;
		estado=5;
		vFila = rand() % 10 + 0;
		vColumna = rand() % 4 + 0;
		mensaje=to_string(vFila)+" "+to_string(vColumna)+" "+to_string(cliente);
		int send_bytes = socket.Send(socket_fd, mensaje);
		if(send_bytes == -1){
			cerr << "Error al enviar datos: " << strerror(errno) << cliente <<  endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
			// Buffer para almacenar la respuesta, como char[]
		    string buffer;
		    // Recibimos la respuesta del servidor  
		    int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
			estado=stoi(buffer);
			
			if (estado==0){
				salir=true;
				pantalla="EL CLIENTE " +to_string(cliente) 
					+ " HA CONSEGUIDO EL ASIENTO " + to_string(vFila) + "," + to_string(vColumna);
				cout << pantalla <<endl;
			}
			else if (estado==1){
				pantalla="EL ASIENTO "  + to_string(vFila) + "," + to_string(vColumna) +
					" ESTABA OCUPADO. CLIENTE: "+to_string(cliente);
				cout << pantalla <<endl;
				cout << buffer << endl;
			}
			else if (estado==3){
				salir=true;
				pantalla="EL AVIÓN ESTABA LLENO. CLIENTE: "+to_string(cliente);
				cout << pantalla << endl;
			}
			i++;
				if (i==4){
				estado=7;
				cout << "EL CLIENTE " << cliente << " HA DESISTIDO." << endl;
				}
				
	}
    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }
}

int main(int argc, char * argv[]) {
	
	string SERVER_ADDRESS;
	int SERVER_PORT;
	thread cliente[N];
    // Dirección y número donde escucha el proceso servidor
	
		//verificación del número de parámetros:
	if (argc != 3) {
		printf("Número de parámetros incorrecto \n");
		printf("Uso : %s servidor puerto/servicio\n", argv[0]);
		exit(1); //sale del programa indicando salida incorrecta (1)
	}
	else{
		SERVER_ADDRESS = argv[1];
		SERVER_PORT = atoi(argv[2]);
	}
	

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

	
    // Conectamos con el servidor. Probamos varias conexiones
	//const int MAX_ATTEMPS = 10;
	int count = 0;
	int socket_fd[N];
	int i = 0;
	bool continuar=true;
	while (i<N){
		socket_fd[i] = socket.Connect();
		// Chequeamos si se ha realizado la conexión
		if(socket_fd[i] == -1){
    	    cout << " Conexión NO POSIBLE" << endl;
    	}
		else{
			cliente[i] = thread(&inCliente, ref(socket), socket_fd[i], i);
		}
	
		i++;
	}
	i=0;
	while (i<N){
		cliente[i].join();
	}
}
