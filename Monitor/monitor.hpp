/*
 * Trabajo de PSCD
 * Daniel Naval
 * Victor Peñasco 
 * Pablo Orduna
 * Fichero: Monitor/monitor.hpp
 * Fecha: Enero 2018
 */

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

struct datosValla{
	string url;
	int tiempo;
	string nombreCliente;
	int precio;
};

class controlCola{
	public:
		controlCola();
		void colaPop(struct datosValla& datos);
		void colaPush(struct datosValla datos);
		bool haTerminado();
		void avisarFin();
	private:
		bool fin;
		recursive_mutex finMtx;
		recursive_mutex colaMtx;
		condition_variable_any cv;
		queue<datosValla> cola;
};
