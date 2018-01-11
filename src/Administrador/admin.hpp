/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Administrador/admin.hpp
 * Fecha: Enero 2018
 */
#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>

using namespace std;

/*
 * PRE:
 * POST: El proceso se duerme durante un tiempo determinado. Una vez termina
 *		ese tiempo notifica a todos los procesos servidor que se debe proceder
 *		a la terminación ordenada de las subastas, de manera que el servidor
 *		espera a que termine la subasta en curso pero no se realizarán más
 *		subastas (esto no afecta al gestor de vallas, que seguirá ejecutándose
 *		hasta procesar todas las imágenes encoladas).
 */
void procesoAdministrador(Control& control);

/*
 * PRE:
 * POST: Periódicamente se muestra por pantalla información estadística del
 *		sistema: número de imágenes mostradas, tiempo total y medio que las
 *		imágenes han sido mostradas, número de peticiones encoladas y tiempo
 *		contratado estimado.
 */
void estadistico(Control& control);

#endif
