/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Monitor/monitor.hpp
 * Fecha: Enero 2018
 */

#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "../Subastador/subastador.hpp"

using namespace std;

struct datosValla{
	char url[500];
	int tiempo;
	char nombreCliente[500];
	int precio;
};

class Control{
	public:
		Control();
		void colaPop(datosValla& datos);
		void colaPush(datosValla datos);
		bool haTerminado();
		void avisarFin();
		void annadirPujador();
		void iniciarInscripcion();
		void esperarComienzo();
		bool seguirAceptando();
		bool terminaRonda();
		void anadirRechaza(subasta& subastaActual);
		void anadirAcepta(subasta& subastaActual);
		void clearAceptan();
		void clearRechazan();
		int numeroPujadoresAceptan();
		void esperarFinRonda();
		void esperarFinSubasta();
		void notificarFinSubasta();
		void mostrar(string texto);
	private:
		const int RETARDO=10;
		bool fin;
		recursive_mutex finMtx;
		recursive_mutex colaMtx;
		recursive_mutex pujadoresMtx;
		recursive_mutex inscripcionMtx;
		recursive_mutex textoMtx;
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
