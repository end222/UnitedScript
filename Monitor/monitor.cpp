/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Monitor/monitor.hpp
 * Fecha: Enero 2018
 */

#include "monitor.hpp"
using namespace std;

/*
 * PRE:
 * POST: Construye la clase Control que se usa como monitor. Inicializa parte de
 * 		las variables internas de Control. Pone los booleanos fin y finGestor a falso
 *		y aceptarPujadores a true. También inicializa a 0 los enteros numPujadoresActivos,
 *		numPujadoresJugando, aceptarPujadores, numPujadoresAceptan, numPujadoresRechazan,
 *		contador, tiempoMostrado y tiempoEstimado.
 */
Control::Control(){
	fin = false;
	finGestor = false;
	numPujadoresActivos = 0;
	numPujadoresJugando = 0;
	aceptarPujadores = true;
	numPujadoresAceptan = 0;
	numPujadoresRechazan = 0;
	contador = 0;
	tiempoMostrado = 0;
	tiempoEstimado = 0;
}

/*
 * PRE: Se ha construido Control
 * POST: Cambia el valor del booleano finGestor a true, para que de esta manera
 *		el gestor de vallas conozca cuando debe terminar.
 */
void Control::avisarFinGestor(){
	unique_lock<recursive_mutex> lck(colaMtx);
	finGestor = true;
}

/*
 * PRE: Se ha construido Control
 * POST: 
 */
bool Control::colaPop(datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	cout << cola.empty() << endl;
	if(cola.empty() && !finGestor){
		cv_cola.wait(lck);
	}
	if(!cola.empty()){
		datos = cola.front();
		tiempoMostrado += datos.tiempo;
		cola.pop();
		return 1;
	}
	else{
		return 0;
	}
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
int Control::totalPujadores(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresActivos;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::generaDatos(datosValla& datos, int numCliente, int tiempo, int precio, char* url){
	strcpy(datos.url,url);
	string cliente = to_string(numCliente);
	datos.tiempo = tiempo;
	strcpy(datos.nombreCliente, cliente.c_str());
	datos.precio = precio;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::colaPush(datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	cola.push(datos);
	totalEncolados++;
	tiempoEstimado += datos.tiempo;
	cv_cola.notify_all(); // La cola ya no esta vacia
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::notifyCola(){
	unique_lock<recursive_mutex> lck(colaMtx);
	cv_cola.notify_all();
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
bool Control::haTerminado(){
	unique_lock<recursive_mutex> lck(finMtx);
	return fin;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::avisarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	fin=true;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::annadirPujador(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresActivos++;
	numPujadoresJugando++;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::iniciarInscripcion(){
	this_thread::sleep_for(chrono::seconds(RETARDO));
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	aceptarPujadores=false;
	cv_comenzar.notify_all();
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::esperarComienzo(){
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	cv_comenzar.wait(lck);
}

/*
 * PRE:
 * POST:
 */
bool Control::seguirAceptando(){
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	return aceptarPujadores;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::anadirRechaza(subasta& subastaActual){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresRechazan++;
	if(numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos){
		cv_finRonda.notify_all();
	}
	else{
		cv_finRonda.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::anadirAcepta(subasta& subastaActual){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresAceptan++;
	if(numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos){
		cv_finRonda.notify_all();
	}
	else{
		cv_finRonda.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::terminaRonda(subasta& subastaActual){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	if(contadorRonda + 1 == numPujadoresJugando){
		numPujadoresJugando = numPujadoresAceptan;
		subastaActual.incrementarPrecio();
		if(numPujadoresAceptan <= 1){
			numPujadoresJugando = numPujadoresActivos;
			numPujadoresRechazan = 0;
			subastaActual.rehacer();
		}
		numPujadoresAceptan = 0;
		cv_finRonda.notify_all();
		contadorRonda = 0;
	}
	else{
		contadorRonda++;
		cv_finRonda.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
int Control::numeroPujadoresAceptan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresAceptan;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::esperarFinSubasta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	if(contador + 1 == numPujadoresActivos){
		cv_finSubasta.notify_all();
		contador = 0;
	}
	else{
		contador++;
		cv_finSubasta.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::mostrar(string texto){
	unique_lock<recursive_mutex> lck(textoMtx);
	cout << texto << endl;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
string Control::obtenerInfoSistema(){
	unique_lock<recursive_mutex> lck(colaMtx);
	string informacion;
	informacion="=============================\nInformacion historica del sistema\n\n";
	informacion+="Numero de imagenes mostradas: "+to_string(totalEncolados-cola.size());
	informacion+="\nTiempo total de imagenes mostradas: "+to_string(tiempoMostrado);
	informacion+="\nTiempo medio de imagenes mostradas: "
	+to_string((double)(totalEncolados-cola.size())/(double)tiempoMostrado);
	informacion+="\n-----------------------------\nInformacion del estado del sistema\n\n";
	informacion+="Numero de peticiones encoladas: "+to_string(cola.size());
	informacion+="\nTiempo contratado estimado: "+to_string(tiempoEstimado);
	informacion+="\n=============================";
	return informacion;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::comprobarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	finSubasta = fin;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
bool Control::finSubastas(){
	unique_lock<recursive_mutex> lck(finMtx);
	return finSubasta;
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
int Control::tamanoCola(){
	return cola.size();
}
