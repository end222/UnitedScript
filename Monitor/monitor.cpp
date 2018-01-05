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
	numPujadoresTotal=0;
	numPujadoresActivos=0;
	aceptarPujadores=true;
}

void Control::colaPop(struct datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	while(cola.empty()){
		cv_cola.wait(lck);
	}
	datos = cola.front();
	cola.pop();
}

void Control::colaPush(struct datosValla datos){
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
		clearAceptan();
		subastaActual.incrementarPrecio();
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
		clearAceptan();
		subastaActual.incrementarPrecio();
		cv_finRonda.notify_all();
	}
	else{
		cv_finRonda.wait(lck);
	}
}

void Control::clearAceptan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresAceptan=0;
}

void Control::clearRechazan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresRechazan=0;
}

bool Control::terminaRonda(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos;
}

int Control::numeroPujadoresAceptan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresAceptan;
}

void Control::esperarFinRonda(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	cv_finRonda.wait(lck);
}

void Control::esperarFinSubasta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	cv_finSubasta.wait(lck);
}

void Control::notificarFinSubasta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	cv_finSubasta.notify_all();
}
