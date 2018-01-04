/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: subastador.cpp
 * Fecha: Enero 2018
 */

#include <iostream>
#include "subastador.hpp"

using namespace std;

subasta::subasta(){
	srand(time(NULL));
	precioSalida=rand()%91+10; //Precio entre 10 y 100
	tiempo=precioSalida/10;
	precioDeReserva=precioSalida*3;
	precioDeIncremento=precioSalida/2;
	precioActual=precioSalida;
}

int obtenerPrecioSalida(){
	return precioSalida;
}

int obtenerTiempo(){
	return tiempo;
}

int obtenerPrecioDeReserva(){
	return precioDeReserva;
}

int obtenerPrecioActual(){
	return precioActual;
}

int obtenerPrecioDeIncremento(){
	return precioDeIncremento;
}

int incrementarPrecio(){
	precioActual+=precioDeIncremento;
	return precioActual;
}
