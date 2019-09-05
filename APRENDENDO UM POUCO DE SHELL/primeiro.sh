#!/bin/bash
echo "olá, mundo"


NOME="MATEUS"
echo "olá, ${NOME}"


echo "Digite seu nome"
#read MEU_NOME
#echo "olá, ${MEU_NOME}"


#comentários se fazem assim. PODE ADICIONAR ESPAÇO A VONTADE E TAMBÉM FORA DE TABULAÇÃO.



#Mudar de diretório e criar um arquivo
#cd /T3
#touch arquivo1
#ls -l /T3/arquivo1
#pwd

DATA=$(date +%m-%y)
echo ${DATA}

#cria um arquivo que tem anexado a data junto
touch arquivo-${DATA}.txt


#manipulação de argumentos
echo "Argumento $1"
echo "Argumento $2"

#número de argumentos passados
echo "Foram passados $# argumentos"

#lista de todos os argumentos passados
echo "A lista de argumentos passados foi: $@"

