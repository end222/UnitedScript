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
#include "monitor.hpp"

using namespace std;

controlCola::controlCola(){
	fin = false;
}

void controlCola::colaPop(struct datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	while(cola.empty()){
		cv.wait(lck);
	}
	datos = cola.front();
	cola.pop();
}

void controlCola::colaPush(struct datosValla datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	cola.push(datos);
	cv.notify_all(); // La cola ya no esta vacia
}

bool controlCola::haTerminado(){
	unique_lock<recursive_mutex> lck(finMtx);
	return fin;
}

void controlCola::avisarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	fin=true;
}
