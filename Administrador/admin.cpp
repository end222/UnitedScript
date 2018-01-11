/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Administrador/admin.hpp
 * Fecha: Enero 2018
 */

#include "../Monitor/monitor.hpp"
#include "admin.hpp"

#define TIEMPO_SUBASTAS 10
#define TIEMPO_ESTADISTICO 10


/*
 * PRE:
 * POST: El proceso se duerme durante un tiempo determinado. Una vez termina
 *		ese tiempo notifica a todos los procesos servidor que se debe proceder
 *		a la terminación ordenada de las subastas, de manera que el servidor
 *		espera a que termine la subasta en curso pero no se realizarán más
 *		subastas (esto no afecta al gestor de vallas, que seguirá ejecutándose
 *		hasta procesar todas las imágenes encoladas).
 */
void procesoAdministrador(Control& control){
	this_thread::sleep_for(chrono::seconds(TIEMPO_SUBASTAS));
	control.avisarFin();
	control.notifyCola();
}

/*
 * PRE:
 * POST: Periódicamente se muestra por pantalla información estadística del
 *		sistema: número de imágenes mostradas, tiempo total y medio que las
 *		imágenes han sido mostradas, número de peticiones encoladas y tiempo
 *		contratado estimado.
 */
void estadistico(Control& control){
	while(!control.finSubastas()){
		this_thread::sleep_for(chrono::seconds(TIEMPO_ESTADISTICO));
		string cadena;
		cadena=control.obtenerInfoSistema();
		control.mostrar(cadena);
	}
}
