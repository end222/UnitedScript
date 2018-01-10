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
#include <string>
#include <cstring>
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
		void colaPush(datosValla& datos);
		void generaDatos(datosValla& datos, int numCliente, int tiempo, int precio, char* url);
		bool haTerminado();
		void avisarFin();
		void annadirPujador();
		void iniciarInscripcion();
		void esperarComienzo();
		bool seguirAceptando();
		void terminaRonda(subasta& subastaActual);
		void anadirRechaza(subasta& subastaActual);
		void anadirAcepta(subasta& subastaActual);
		int numeroPujadoresAceptan();
		void esperarFinSubasta();
		void mostrar(string texto);
		void comprobarFin();
		bool finSubastas();
		string obtenerInfoSistema();
		int tamanoCola();
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
		int numPujadoresActivos;
		int numPujadoresAceptan;
		int numPujadoresRechazan;
		int numPujadoresJugando;
		bool aceptarPujadores;
		int precioActual;
		bool finSubasta;
		int contador;
		int contadorRonda;
		int tiempoMostrado;
		int tiempoEstimado;
		int totalEncolados;
};
