/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Monitor/monitor.hpp
 * Fecha: Enero 2018
 */

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "subastador.h"

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
		bool terminaRonda();
		void anadirRechaza();
		void anadirAcepta();
		void clearAceptan();
		void clearRechazan();
		int numPujadoresAceptan();
		void esperarFinRonda();
		void esperarFinSubasta();
		void notificarFinSubasta();
	private:
		const int RETARDO=10;
		bool fin;
		recursive_mutex finMtx;
		recursive_mutex colaMtx;
		recursive_mutex pujadoresMtx;
		recursive_mutex inscripcionMtx;
		condition_variable_any cv_cola;
		condition_variable_any cv_comenzar;
		condition_variable_any cv_finRonda;
		condition_variable_any cv_finSubasta;
		queue<datosValla> cola;
		int numPujadoresTotal;
		int numPujadoresActivos;
		int numPujadoresAceptan;
		int numPujadoresRechazan;
		bool aceptarPujadores;
		int precioActual;
};

control control;
