/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Administrador/admin.hpp
 * Fecha: Enero 2018
 */
#ifndef GESTOR_H
#define GESTOR_H

#include <iostream>
#include <string>

using namespace std;

const int TIEMPO_SUBASTAS=45;
const int TIEMPO_ESTADISTICO=10;

void procesoAdministrador(Control& control);

void estadistico(Control& control);

#endif
