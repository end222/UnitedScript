/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Administrador/admin.hpp
 * Fecha: Enero 2018
 */
#include "admin.hpp"

void procesoAdministrador(Control& control){
	this_thread::sleep_for(chrono::seconds(TIEMPO_SUBASTAS));
	control.avisarFin();
}

void estadistico(Control& control){
	this_thread::sleep_for(chrono::seconds(TIEMPO_ESTADISTICO));
	string cadena;
	cadena=control.obtenerInfoSistema();
	control.mostrar(cadena);
}
