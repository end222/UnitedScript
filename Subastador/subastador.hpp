/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: subastador.hpp
 * Fecha: Enero 2018
 */

#ifndef SUBASTADOR_H
#define SUBASTADOR_H

#include <iostream>
#include <string>

using namespace std;

class subasta{
	public:
		subasta();
		int obtenerPrecioSalida();
		int obtenerTiempo();
		int obtenerPrecioDeReserva();
		int obtenerPrecioActual();
		int obtenerPrecioDeIncremento();
		int incrementarPrecio();
	private:
		int precioSalida;
		int tiempo;
		int precioDeReserva;
		int precioActual;
		int precioDeIncremento;
};



void ();

#endif
