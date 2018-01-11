//*****************************************************************
// File:    ImageDownloader.hpp
// Author:  PSCD-Unizar
// Date:    11/11/15
// Coms:    Programaci�n de Sistemas Concurrentes y 
//          Distribuidos Curso 2015-2016.
//          Especificaci�n de una librer�a para descarga de im�genes de Internet
//*****************************************************************


#ifndef IMAGE_DOWNLOADER_HPP
#define IMAGE_DOWNLOADER_HPP

class ImageDownloader {

public:

	// CONSTRUCTORES
	// Pre:
	// Post: Inicializa la librer�a de descarga de im�genes
	ImageDownloader();
	
	// DESTRUCTOR
	// Pre: 
	// Post: Libera el espacio utilizado para la descarga de im�genes. 
	//       Se llama de manera autom�tica al cerrar el bloque en que se declar� el objeto	
	~ImageDownloader();
	
	// M�TODOS

	// Pre:  "URL": URL correspondiente a una imagen
	//       "path": Ruta local donde almacenar la imagen
	//		 El m�todo 'cleanup' no ha sido llamado
	// Post: Descarga una imagen de la URL y la almacena en el path local indicado
	// 		 Devuelve 0 en caso de �xito y -1 en caso de error
	int downloadImage(char* URL, char* path);
};
#endif