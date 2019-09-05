#!/bin/bash

echo "BEM VINDO AO PROGRAMA $0"

echo "Qual a sua idade?"
read IDADE

if [[${IDADE} -ge 18 ]];
then 
	echo "Você tem 18 anos ou mais."


elif [[ ${IDADE} -ge 16 ]]; 
then
	echo "Você tem entre 16 e 17"

else
	echo "Você ainda não tem 18 anos."
fi


#man test



#teste se o usuário é root:
if [[ ${UID} -eq 0 ]]; then
	echo "Você tem permissão de root"
	echo "Quer parar o servidor apache?"
	echo "S / N"
	read RESPOSTA
	if [[ ${RESPOSTA} = "S" ]]; then
		echo "Você pediu para parar"
	else
		echo "Você pediu para NÃO parar"
	fi
else
	echo "Por favor, vire root para usar esse programa"
fi