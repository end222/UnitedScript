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
	char url[500];
	int tiempo;
	char nombreCliente[500];
	int precio;
};

class control{
	public:
		control();
		void colaPop(struct datosValla& datos);
		void colaPush(struct datosValla datos);
		bool haTerminado();
		void avisarFin();
		void annadirPujador();
		void iniciarInscripcion();
		void esperarComienzo();
		bool seguirAceptando();
	private:
		const int RETARDO=10;
		bool fin;
		recursive_mutex finMtx;
		recursive_mutex colaMtx;
		recursive_mutex pujadoresMtx;
		recursive_mutex inscripcionMtx;
		condition_variable_any cv_cola;
		condition_variable_any cv_comenzar;
		queue<datosValla> cola;
		int numPujadoresTotal;
		int numPujadoresActivos;
		bool aceptarPujadores;
};

control control;
