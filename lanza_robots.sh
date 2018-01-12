#!/bin/bash

# Hay un sleep entre medio para que la el comportamiento aleatorio de los clientes
# siga una semilla diferente
echo "Lanzando clientes en terminales separadas"
xterm -e "./bin/cliente localhost 3000 automatico" &
sleep 1
xterm -e "./bin/cliente localhost 3000 automatico" &
sleep 1
xterm -e "./bin/cliente localhost 3000 automatico" &
sleep 1
xterm -e "./bin/cliente localhost 3000 automatico" &
