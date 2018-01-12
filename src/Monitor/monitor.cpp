/*
 * Trabajo de PSCD
 * Autores:
 * - Daniel Naval
 * - Victor Peñasco
 * - Pablo Orduna
 * Fichero: Monitor/monitor.hpp
 * Fecha: Enero 2018
 */

#include "monitor.hpp"
using namespace std;

/*
 * PRE:
 * POST: Construye la clase Control que se usa como monitor. Inicializa parte de
 * 		las variables internas de Control. Pone los booleanos fin y finGestor a falso
 *		y aceptarPujadores a true. También inicializa a 0 los enteros numPujadoresActivos,
 *		numPujadoresJugando, aceptarPujadores, numPujadoresAceptan, numPujadoresRechazan,
 *		contador, tiempoMostrado y tiempoEstimado.
 */
Control::Control(){
	fin = false;
	finGestor = false;
	numPujadoresActivos = 0;
	numPujadoresJugando = 0;
	aceptarPujadores = true;
	numPujadoresAceptan = 0;
	numPujadoresRechazan = 0;
	contador = 0;
	tiempoMostrado = 0;
	tiempoEstimado = 0;
}

/*
 * PRE: Se ha construido Control.
 * POST: Cambia el valor del booleano finGestor a true, para que de esta manera
 *		el gestor de vallas conozca cuando debe terminar.
 */
void Control::avisarFinGestor(){
	unique_lock<recursive_mutex> lck(colaMtx);
	finGestor = true;
}

/*
 * PRE: Se ha construido Control.
 * POST: Mientras el gestor de vallas no debe terminar aún y la cola se
 *		encuentre vacía, espera a que la cola deje de estar vacía. Una vez
 *		esto suceda, comprueba si la cola está vacía. Si contiene algún
 *		elemento, asocia a datos el primer elemento de la cola, suma a
 *		tiempoMostrado el tiempo asociado a ese elemento, hace pop de ese
 *		elemento de la cola y finalmente devuelve true. En cambio, si en la
 *		comprobación detecta que la cola sigue vacía, se limita a devolver
 *		false. Los datos son devueltos por referencia.
 */
bool Control::colaPop(datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	if(cola.empty() && !finGestor){
		cv_cola.wait(lck);
	}
	if(!cola.empty()){
		datos = cola.front();
		tiempoMostrado += datos.tiempo;
		cola.pop();
		return 1;
	}
	else{
		return 0;
	}
}

/*
 * PRE: Se ha construido Control.
 * POST: Devuelve el número de pujadores activos de la subasta
 */
int Control::totalPujadores(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresActivos;
}

/*
 * PRE: Se ha construido Control.
 * POST: Asocia los diferentes datos que se proporcian por referencia a la
 *		variable datos, de tal manera que la url de datos es url, el
 *		cliente es numCliente, el tiempo es tiempo, nombreCliente es
 *		cliente y precio es precio. Devuelve datos por referencia.
 */
void Control::generaDatos(datosValla& datos, int numCliente, int tiempo, int precio, char* url){
	strcpy(datos.url,url);
	string cliente = to_string(numCliente);
	datos.tiempo = tiempo;
	strcpy(datos.nombreCliente, cliente.c_str());
	datos.precio = precio;
}

/*
 * PRE: Se ha construido Control.
 * POST: Introduce un nuevo elemento datos a la cola, incrementa en una
 *		unidad en el número de encolados, incrementa tiempoEstimado en el
 *		tiempo asociado a los datos y notifica a las funciones que estén
 *		esperando con la variable condición cv_cola.
 */
void Control::colaPush(datosValla& datos){
	unique_lock<recursive_mutex> lck(colaMtx);
	cola.push(datos);
	totalEncolados++;
	tiempoEstimado += datos.tiempo;
	cv_cola.notify_all(); // La cola ya no esta vacia
}

/*
 * PRE: Se ha construido Control.
 * POST: Notifica a las funciones que estén	esperando con la variable
 *		condición cv_cola.
 */
void Control::notifyCola(){
	unique_lock<recursive_mutex> lck(colaMtx);
	cv_cola.notify_all();
}

/*
 * PRE: Se ha construido Control.
 * POST: Devuelve true si se ha indicado que las subastas deben acabar y
 *		false en caso contrario.
 */
bool Control::haTerminado(){
	unique_lock<recursive_mutex> lck(finMtx);
	return fin;
}

/*
 * PRE: Se ha construido Control.
 * POST: Indica que las subastas deben acabar.
 */
void Control::avisarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	fin=true;
}

/*
 * PRE: Se ha construido Control.
 * POST: Incrementa en una unidad el número de pujadores activos y el
 *		número de pujadores jugando.
 */
void Control::annadirPujador(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresActivos++;
	numPujadoresJugando++;
}

/*
 * PRE: Se ha construido Control.
 * POST: Después de dormirse durante un tiempo determinado, indica que se
 *		debe dejar de aceptar pujadores y notifica a todos los procesos
 *		que estuvieran esperando la variable condición cv_comenzar.
 */
void Control::iniciarInscripcion(){
	this_thread::sleep_for(chrono::seconds(RETARDO));
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	aceptarPujadores=false;
	cv_comenzar.notify_all();
}

/*
 * PRE: Se ha construido Control.
 * POST: Espera hasta que se notifique a la variable condición
 *		cv_comenzar.
 */
void Control::esperarComienzo(){
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	cv_comenzar.wait(lck);
}

/*
 * PRE: Se ha construido Control.
 * POST: Devuelve true si se deben seguir aceptando pujadores a la subasta
 *		y false en caso contrario.
 */
bool Control::seguirAceptando(){
	unique_lock<recursive_mutex> lck(inscripcionMtx);
	return aceptarPujadores;
}

/*
 * PRE: Se ha construido Control.
 * POST: Incrementa en una unidad el número de pujadores que rechazan el
 *		precio sugerido por el subastador. Si el número de pujadores que
 *		han aceptado es igual al número de pujadores que han rechazado,
 *		es decir, que todos los pujadores han respondido a la sugerencia.
 *		notifica a la variable condición cv_finRonda. En caso contrario,
 *		espera hasta que se notifique a la variable condición cv_finRonda.
 */
void Control::anadirRechaza(subasta& subastaActual){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresRechazan++;
	if(numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos){
		cv_finRonda.notify_all();
	}
	else{
		cv_finRonda.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control.
 * POST: Incrementa en una unidad el número de pujadores que aceptan el
 *		precio sugerido por el subastador. Si el número de pujadores que
 *		han aceptado es igual al número de pujadores que han rechazado,
 *		es decir, que todos los pujadores han respondido a la sugerencia.a
 *		notifica a la variable condición cv_finRonda. En caso contrario,
 *		espera hasta que se notifique a la variable condición cv_finRonda.
 */
void Control::anadirAcepta(subasta& subastaActual){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresAceptan++;
	if(numPujadoresAceptan+numPujadoresRechazan==numPujadoresActivos){
		cv_finRonda.notify_all();
	}
	else{
		cv_finRonda.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control.
 * POST: Si es el último pujador de la ronda que ejecuta esta función,
 *		asigna al número de pujadores jugando el número de pujadores que
 *		han aceptado en la ronda. Incrementa el precio de la subasta para
 *		la siguiente ronda. Si solo ha aceptado un pujador ha aceptado en
 *		la ronda o ninguno la ha aceptado, el numero de pujadores jugando
 *		pasa a ser el número de pujadores activos, se pone a 0 el número de
 *		pujadores que han rechazado y se rehace la subasta.
 *		Independientemente de si se cumple esta última condición, se pone a
 *		0 el numero de pujadores que han aceptado, se notifica a la
 *		variable condición cv_finRonda y se pone a 0 el contador de
 *		jugadores que han participado en la ronda.
 *		En cambio, si el pujador que ejecuta esta función no es último,
 *		suma uno al contador de pujadores que han participado en la ronda,
 *		y se mantiene esperando a que se notifique a la variable condición
 *		cv_finRonda.
 */
void Control::terminaRonda(subasta& subastaActual){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	if(contadorRonda + 1 == numPujadoresJugando){
		numPujadoresJugando = numPujadoresAceptan;
		subastaActual.incrementarPrecio();
		if(numPujadoresAceptan <= 1){
			numPujadoresJugando = numPujadoresActivos;
			numPujadoresRechazan = 0;
			subastaActual.rehacer();
		}
		numPujadoresAceptan = 0;
		cv_finRonda.notify_all();
		contadorRonda = 0;
	}
	else{
		contadorRonda++;
		cv_finRonda.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control.
 * POST: Devuelve el número de pujadores que han aceptado durante la ronda.
 */
int Control::numeroPujadoresAceptan(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	return numPujadoresAceptan;
}

/*
 * PRE: Se ha construido Control.
 * POST: Si es el último pujador de la ronda que ejecuta esta función,
 *		se notifica a la variable condición cv_finRonda y se pone a 0 el
 *		contador. En caso contrario, aumenta en una unidad el contador y
 *		se mantiene esperando a que se notifique a la variable condición
 *		cv_finRonda.
 */
void Control::esperarFinSubasta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	if(contador + 1 == numPujadoresActivos){
		cv_finSubasta.notify_all();
		contador = 0;
	}
	else{
		contador++;
		cv_finSubasta.wait(lck);
	}
}

/*
 * PRE: Se ha construido Control.
 * POST: Muestra por pantalla el string texto que se pasa por referencia.
 */
void Control::mostrar(string texto){
	unique_lock<recursive_mutex> lck(textoMtx);
	cout << texto << endl;
}

/*
 * PRE: Se ha construido Control
 * POST: Devuelve un string que contiene distinta información estadística:
 *		el número de imágenes mostradas, el tiempo total de imágenes
 *		mostradas, el tiempo medio que se han mostrado las imágenes, el
 *		número de peticiones que se encuentran encoladas en el momento que
 *		se ejecuta la función y el tiempo contratado estimado.
 */
string Control::obtenerInfoSistema(){
	unique_lock<recursive_mutex> lck(colaMtx);
	string informacion;
	informacion="=============================\nInformacion historica del sistema\n\n";
	informacion+="Numero de imagenes mostradas: "+to_string(totalEncolados-cola.size());
	informacion+="\nTiempo total de imagenes mostradas: "+to_string(tiempoMostrado);
	informacion+="\nTiempo medio de imagenes mostradas: "
	+to_string((double)(totalEncolados-cola.size())/(double)tiempoMostrado);
	informacion+="\n-----------------------------\nInformacion del estado del sistema\n\n";
	informacion+="Numero de peticiones encoladas: "+to_string(cola.size());
	informacion+="\nTiempo contratado estimado: "+to_string(tiempoEstimado);
	informacion+="\n=============================";
	return informacion;
}

/*
 * PRE: Se ha construido Control.
 * POST: Comprueba si el administrador ha indicado que se deben finalizar
 *		las subastas.
 */
void Control::comprobarFin(){
	unique_lock<recursive_mutex> lck(finMtx);
	finSubasta = fin;
}

/*
 * PRE: Se ha construido Control.
 * POST: Devuelve el true si se deben finalizar las subastas y false en
 *		caso contrario.
 */
bool Control::finSubastas(){
	unique_lock<recursive_mutex> lck(finMtx);
	return finSubasta;
}

/*
 * PRE: Se ha construido Control.
 * POST: Devuelve el tamaño de la cola.
 */
int Control::tamanoCola(){
	return cola.size();
}

/*
 * PRE: Se ha construido Control
 * POST:
 */
void Control::saleAcepta(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresAceptan--;
	numPujadoresActivos--;
}

void Control::saleRechaza(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresRechazan--;
	numPujadoresActivos--;
}

void Control::saleSinElegir(){
	unique_lock<recursive_mutex> lck(pujadoresMtx);
	numPujadoresActivos--;
}
