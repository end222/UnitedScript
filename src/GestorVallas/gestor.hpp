/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: GestorVallas/gestor.hpp
 * Fecha: Enero 2018
 */

#ifndef GESTOR_H
#define GESTOR_H

#include <iostream>
#include <string>

using namespace std;

/*
 * PRE:
 * POST: Mientras se estén realizando subastas, procesa peticiones de poner
 *		imágenes en vallas que se encuentren en cola. Al procesar una petición,
 *		muestra durante un tiempo determinado en una ventana la imagen que se
 *		encuentra en la URL de la petición. Si el tiempo de subastas ya ha
 *		acabado pero se encuentran peticiones en cola, continúa ejecutándose
 *		hasta procesar todas ellas.
 */
void procesoGestorVallas(Control& control);

#endif
