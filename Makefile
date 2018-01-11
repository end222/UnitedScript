#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
# Coms:   Ejemplo de uso de "CImage" y "curl" para el trabajo obligatorio
#         Compilar como "make"
#*****************************************************************

RM = rm -rf
UNAME := $(shell uname)
CC=g++
ifeq ($(UNAME), FreeBSD) #eeePC
	CC = c++
endif
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
	${CC} ImageDownloader.o admin.o gestor.o subastador.o monitor.o Socket.o ${MAIN}.o -o bin/${MAIN} ${LDFLAGS}

${MAIN}.o: src/${MAIN}.cpp 
	${CC} -c ${CPPFLAGS} src/${MAIN}.cpp

imageDownloader.o: src/${imageDownloader}.hpp src/${imageDownloader}.cpp
	${CC} -c ${CPPFLAGS} src/${imageDownloader}.cpp

gestor.o: src/GestorVallas/gestor.cpp src/GestorVallas/gestor.hpp
	${CC} -c ${CPPFLAGS} src/GestorVallas/gestor.cpp 

admin.o: src/Administrador/admin.hpp src/Administrador/admin.cpp
	${CC} -c ${CPPFLAGS} src/Administrador/admin.cpp

subastador.o: src/Subastador/subastador.hpp src/Subastador/subastador.cpp
	${CC} -c ${CPPFLAGS} src/Subastador/subastador.cpp

monitor.o: src/Monitor/monitor.hpp src/Monitor/monitor.cpp
	${CC} -c ${CPPFLAGS} src/Monitor/monitor.cpp

Socket.o: src/Socket/Socket.cpp src/Socket/Socket.hpp
	${CC} -c ${CPPFLAGS} src/Socket/Socket.cpp

cliente: Socket.o src/Cliente/cliente.cpp
	${CC} -c ${CPPFLAGS} src/Cliente/cliente.cpp
	${CC} cliente.o Socket.o -o bin/cliente ${LDFLAGS}

clean:
	$(RM) *.o
	$(RM) bin/*

echo:
	echo ${target}
	echo -n "cppflags: " ${CPPFLAGS}
	echo -n "ldflags: " ${LDFLAGS}
