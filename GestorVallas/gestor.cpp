/*
 * Trabajo de PSCD
 * Daniel Naval
 * Victor Peñasco
 * Pablo Orduna
 * Fichero: GestorVallas/gestor.hpp
 */

#include <iostream>
#include <string>
#include "../Monitor/monitor.hpp"
#include "../Imagen/ImageDownloader.hpp"
#include "../Imagen/CImg.h"

using namespace cimg_library;
using namespace std;

void procesoGestorVallas(){

	struct datosValla datos;
	const int VALLA_WIDTH = 800;
	const int VALLA_HEIGHT = 800;
	char path[100] = "image.jpg";
	ImageDownloader downloader;
	while(!control.haTerminado()){
		control.colaPop(datos);

		downloader.downloadImage(datos.url, path);
		cimg_library::CImg<unsigned char> img_principal(path);
		cimg_library::CImgDisplay valla(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), datos.nombreCliente);
		valla.resize(VALLA_WIDTH,VALLA_HEIGHT);
		valla.move(0,0); // Esquina superior izquierda

		valla.wait(datos.tiempo*1000);
	}

}
