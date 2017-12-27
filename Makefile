#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
# Coms:   Ejemplo de uso de "CImage" y "curl" para el trabajo obligatorio
#         Compilar como "make"
#*****************************************************************

RM = rm -rf

CPP = g++

# ####### ###### # 
#
MAIN=main
target=${MAIN}
imageDownloader=Imagen/ImageDownloader

# ######
#
CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11 -Werror
LDFLAGS=-L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11 

.PHONY:all

all: ${target}

${MAIN}: ${MAIN}.o imageDownloader.o admin.o gestor.o subastador.o monitor.o Socket.o
	${CPP} ImageDownloader.o admin.o gestor.o subastador.o monitor.o Socket.o ${MAIN}.o -o ${MAIN} ${LDFLAGS}

${MAIN}.o: ${MAIN}.cpp 
	${CPP} -c ${CPPFLAGS} ${MAIN}.cpp

imageDownloader.o: ${imageDownloader}.hpp ${imageDownloader}.cpp
	${CPP} -c ${CPPFLAGS} ${imageDownloader}.cpp

gestor.o: GestorVallas/gestor.cpp GestorVallas/gestor.hpp
	${CPP} -c ${CPPFLAGS} GestorVallas/gestor.cpp 

admin.o: Administrador/admin.hpp Administrador/admin.cpp
	${CPP} -c ${CPPFLAGS} Administrador/admin.cpp

subastador.o: Subastador/subastador.hpp Subastador/subastador.cpp
	${CPP} -c ${CPPFLAGS} Subastador/subastador.cpp

monitor.o: Monitor/monitor.hpp Monitor/monitor.cpp
	${CPP} -c ${CPPFLAGS} Monitor/monitor.cpp

Socket.o: Socket/Socket.cpp Socket/Socket.hpp
	${CPP} -c ${CPPFLAGS} Socket/Socket.cpp

cliente: Cliente/cliente.cpp
	${CPP} -c ${CPPFLAGS} Cliente/cliente.cpp
	${CPP} cliente.o -o cliente ${LDFLAGS}

clean:
	$(RM) *.o
	$(RM) ${target}

echo:
	echo ${target}
	echo -n "cppflags: " ${CPPFLAGS}
	echo -n "ldflags: " ${LDFLAGS}
