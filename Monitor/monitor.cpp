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
#include "monitor.hpp"

using namespace std;

control::control(){
	fin = false;
	numPujadoresTotal=0;
	numPujadoresActivos=0;
	aceptarPujadores=true;
}

void control::colaPop(struct datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	while(cola.empty()){
		cv_cola.wait(lck);
	}
	datos = cola.front();
	cola.pop();
}

void control::colaPush(struct datosValla datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	cola.push(datos);
	cv_cola.notify_all(); // La cola ya no esta vacia
}

bool control::haTerminado(){
	unique_lock<recursive_mutex> lck(finMtx);
	return fin;
}

void control::avisarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	fin=true;
}

void control::annadirPujador(){
	unique_lock<recursive_mutex> lck(pujadoresMtx));
	numPujadoresTotal++;
	numPujadoresActivos++;
}

void control::iniciarInscripcion(){
	unique_lock<recursive_mutex> lck(inscripcionMtx));
	this_thread::sleep_for(chrono::seconds(RETARDO));
	aceptarPujadores=false;
	cv_comenzar.notify_all();
}

void control::esperarComienzo(){
	unique_lock<recursive_mutex> lck(inscripcionMtx));
	cv_comenzar.wait();
}

bool control::seguirAceptando(){
	unique_lock<recursive_mutex> lck(inscripcionMtx));
	return aceptarPujadores;
}

void control::anadirRechaza(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresRechazan++;
	if(numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos){
		clearAceptan();
		subasta.incrementarPrecio();
		cv_finRonda.notify_all();
	}
}

void control::anadirAcepta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresAceptan++;
	if(numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos){
		clearAceptan();
		subasta.incrementarPrecio();
		cv_finRonda.notify_all();
	}
}

void control::clearAceptan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresAceptan=0;
}

void control::clearRechazan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresRechazan=0;
}

bool control::terminaRonda(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos;
}

int control::numeroPujadoresAceptan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresAceptan();
}

void control::esperarFinRonda(){
	unique_lock<recursive_mutex> lck(pujadoresMtx));
	cv_finRonda.wait();
}

void control::esperarFinSubasta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx));
	cv_finSubasta.wait();
}

void control::notificarFinSubasta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx));
	cv_finSubasta.notify_all();
}
