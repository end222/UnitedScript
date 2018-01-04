/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Cliente/cliente.cpp
 * Fecha: Enero 2018
 */

#include <iostream>
#include <chrono>
#include <thread>
#include "../Socket/Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

int main(int argc, char * argv[]) {
	if(argc != 3){
		cerr << "Numero de parametros incorrecto\n";
		exit(1);
	}

	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = argv[1];
	int SERVER_PORT = atoi(argv[2]);

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);

	// Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 10;
	int count = 0;
	int socket_fd;
	do {
		// Conexión con el servidor
    		socket_fd = socket.Connect();
    		count++;

    		// Si error --> esperamos 1 segundo para reconectar
    		if(socket_fd == -1){
    			this_thread::sleep_for(chrono::seconds(1));
    		}
	} while(socket_fd == -1 && count < MAX_ATTEMPS);

	// Chequeamos si se ha realizado la conexión
	if(socket_fd == -1){
		return socket_fd;
	}

    
	string mensaje;
	string aceptar = "ACCEPT";
	string rechazar = "REJECT";
	string buffer;

	bool finSubasta;
	int ronda;
	int subasta = 1;
	bool haTerminado = false;
	
	string start = "START";
	
	while(!haTerminado){
		int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
		if(buffer == start){
			finSubasta = false;
			ronda = 1;
			while(!finSubasta){
				int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
				cout << "SUBASTA " << subasta << endl;
				cout << "RONDA " << ronda << endl;
				cout << "Precio propuesto: " << buffer;
				cout << "¿Acepta la propuesta? (si/no)" << endl;
				cin >> mensaje;
				
				int send_bytes;

				if(mensaje == "si"){
					send_bytes = socket.Send(socket_fd, aceptar);
				}

				else{
					send_bytes = socket.Send(socket_fd, rechazar);
				}
				
				if(send_bytes == -1){
					cerr << "Error al enviar datos: " << strerror(errno) << endl;
					socket.Close(socket_fd);
					exit(1);
				}

				read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
				
				/* 
				 * Código:
				 * 0: Ha rechazado
				 * 1: Ha aceptado pero aún no ha ganado
				 * 2: Ha aceptado, ha ganado, pero no llega al precio de reserva
				 * 3: ha aceptado, ha ganado y llega al precio de reserva
				 */

				if(buffer=="0"){
					cout << "Ha rechazado la propuesta" << endl;
				}
				else if(buffer=="1"){
					cout << "Ha aceptado la propuesta" << endl;
				}
				else if(buffer=="2"){
					cout << "Ha aceptado la propuesta y ha ganado. Sin embargo, no ha llegado al " << 
						"precio de reserva" << endl;
				}
				else{
					cout << "Ha aceptado la propuesta y ha ganado. El anuncio se añadirá a la cola" <<
						" de la valla" << endl;
				}
			}
		}
		else{
			haTerminado = true;
		}
	}

    	// Cerramos el socket
    	int error_code = socket.Close(socket_fd);
	if(error_code == -1){
    	    	cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    	}
    	return error_code;

}
