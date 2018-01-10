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

Control::Control(){
	fin = false;
	numPujadoresTotal = 0;
	numPujadoresActivos = 0;
	aceptarPujadores = true;
	numPujadoresAceptan = 0;
	numPujadoresRechazan = 0;
	contador = 0;
}

void Control::colaPop(datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	while(cola.empty()){
		cv_cola.wait(lck);
	}
	datos = cola.front();
	cola.pop();
}

void Control::generaDatos(datosValla& datos, int numCliente, int tiempo, int precio, char* url){
	strcpy(datos.url,url);
	string cliente = to_string(numCliente);
	datos.tiempo = tiempo;
	strcpy(datos.nombreCliente, cliente.c_str());
	datos.precio = precio;
}

void Control::colaPush(datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	cola.push(datos);
	cv_cola.notify_all(); // La cola ya no esta vacia
}

bool Control::haTerminado(){
	unique_lock<recursive_mutex> lck(finMtx);
	return fin;
}

void Control::avisarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	fin=true;
}

void Control::annadirPujador(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresTotal++;
	numPujadoresActivos++;
}

void Control::iniciarInscripcion(){
	this_thread::sleep_for(chrono::seconds(RETARDO));
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	aceptarPujadores=false;
	cv_comenzar.notify_all();
}

void Control::esperarComienzo(){
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	cv_comenzar.wait(lck);
}

bool Control::seguirAceptando(){
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	return aceptarPujadores;
}

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

void Control::terminaRonda(subasta& subastaActual){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	if(contadorRonda + 1 == numPujadoresActivos){
		numPujadoresAceptan = 0;
		subastaActual.incrementarPrecio();
		cv_finRonda.notify_all();
		contadorRonda = 0;
	}
	else{
		contadorRonda++;
		cv_finRonda.wait(lck);
	}
}

int Control::numeroPujadoresAceptan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresAceptan;
}

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

void Control::mostrar(string texto){
	unique_lock<recursive_mutex> lck(textoMtx);
	cout << texto << endl;
}

string Control::obtenerInfoSistema(){
	string informacion;
	return informacion;
}

void Control::comprobarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	finSubasta = fin;
}

bool Control::finSubastas(){
	unique_lock<recursive_mutex> lck(finMtx);
	return finSubasta;
}
