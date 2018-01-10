/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: GestorVallas/gestor.hpp
 * Fecha: Enero 2018
 */

#include <iostream>
#include <string>
#include "../Monitor/monitor.hpp"
#include "../Imagen/ImageDownloader.hpp"
#include "../Imagen/CImg.h"

using namespace cimg_library;
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
void procesoGestorVallas(Control& control){
	struct datosValla datos;
	const int VALLA_WIDTH = 800;
	const int VALLA_HEIGHT = 300;
	char path[100] = "image.jpg";
	ImageDownloader downloader;
	while(!control.finSubastas() || !control.colaVacia()){
		control.colaPop(datos);

		downloader.downloadImage(datos.url, path);
		cimg_library::CImg<unsigned char> img_principal(path);
		cimg_library::CImgDisplay valla(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), datos.nombreCliente);
		valla.resize(VALLA_WIDTH,VALLA_HEIGHT);
		valla.move(0,0); // Esquina superior izquierda

		valla.wait(datos.tiempo*1000);
	}

}
