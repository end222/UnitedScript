/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: subastador.cpp
 * Fecha: Enero 2018
 */

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

int subasta::obtenerPrecioSalida(){
	return precioSalida;
}

int subasta::obtenerTiempo(){
	return tiempo;
}

int subasta::obtenerPrecioDeReserva(){
	return precioDeReserva;
}

int subasta::obtenerPrecioActual(){
	return precioActual;
}

int subasta::obtenerPrecioDeIncremento(){
	return precioDeIncremento;
}

int subasta::incrementarPrecio(){
	precioActual+=precioDeIncremento;
	return precioActual;
}
