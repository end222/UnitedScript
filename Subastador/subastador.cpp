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

/*
 * PRE:
 * POST: Construye una subasta, de tal forma que se establece un precio de salida
 *		aleatorio entre 10 y 100, un tiempo (segundos que se mostrará la imagen)
 *		equivalentes a una décima parte del precio de salida, un precio de reserva
 *		el triple que el precio de salida, un precio de incremento la mitad del
 *		precio de salida, y un precio actual correspondiente al precio de salida.
 */
subasta::subasta(){
	precioSalida=rand()%91+10; //Precio entre 10 y 100
	tiempo=precioSalida/10;
	precioDeReserva=precioSalida*3;
	precioDeIncremento=precioSalida/2;
	precioActual=precioSalida;
}

/*
 * PRE:
 * POST: Se rehace la subasta construida previamente, de tal forma que se
 *		establece un precio de salida aleatorio entre 10 y 100, un tiempo
 *		(segundos que se mostrará la imagen) equivalentes a una décima parte del
 *		precio de salida, un precio de reserva el triple que el precio de salida,
 *		un precio de incremento la mitad del precio de salida, y un precio actual
 *		correspondiente al precio de salida.
 */
void subasta::rehacer(){
	precioSalida=rand()%91+10; //Precio entre 10 y 100
	tiempo=precioSalida/10;
	precioDeReserva=precioSalida*3;
	precioDeIncremento=precioSalida/2;
	precioActual=precioSalida;
}

/*
 * PRE: Se ha construido la subasta
 * POST: Devuelve el precio de salida de la subasta.
 */
int subasta::obtenerPrecioSalida(){
	return precioSalida;
}

/*
 * PRE: Se ha construido la subasta
 * POST: Devuelve el tiempo que se mostrará la imagen si se gana la subasta.
 */
int subasta::obtenerTiempo(){
	return tiempo;
}

/*
 * PRE: Se ha construido la subasta
 * POST: Devuelve el precio de reserva de la subasta.
 */
int subasta::obtenerPrecioDeReserva(){
	return precioDeReserva;
}

/*
 * PRE: Se ha construido la subasta
 * POST: Devuelve el precio actual de la subasta.
 */
int subasta::obtenerPrecioActual(){
	return precioActual;
}

/*
 * PRE: Se ha construido la subasta
 * POST: Devuelve el precio de incremento de la subasta.
 */
int subasta::obtenerPrecioDeIncremento(){
	return precioDeIncremento;
}

/*
 * PRE: Se ha construido la subasta
 * POST: Incrementa el precio actual de la subasta sumándole el precio de
 *		 incremento y devuelve el precio actual actualizado.
 */
int subasta::incrementarPrecio(){
	precioActual+=precioDeIncremento;
	return precioActual;
}
