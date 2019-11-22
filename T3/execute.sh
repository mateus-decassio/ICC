#!/bin/bash

# AUTOR: Mateus Felipe de Cássio Ferreira
# GRR: 20176123
# DISCIPLINA: CI164 - Introdução à Computação Científica
# Orientador: Daniel Weingaertner

# OBSERVAÇÃO: este script leva em conta que as variáveis de ambiente já estão alteradas, utilizando
# os comandos:
# 	- export PATH=/home/soft/likwid/bin:/home/soft/likwid/sbin:$PATH
#	- export LD_LIBRARY_PATH=/home/soft/likwid/lib:$LD_LIBRARY_PATH
# e que a frequência do processador já está configurado e fixado no máximo, com o comando:
#	- echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor 

# FORMA DE EXECUÇÃO: ./execute.sh <CORE> <PROGRAMA> ,onde
# 	- CORE: núcleo de processamento a ser usado no teste;
# 	- PROGRAMA: matmult ou SistemasLineares.

# O objetivo deste scprit em shell é automatizar os teste de medição de desempenho para os dois
# programas listados acima. O código realiza os testes de:
#
# 1 - MEDIÇÃO DE TEMPO: (utiliza-se a função timestamp()), de uma maneira separada dos demais testes 
# (o código fonte é alterado para marcar o início e o fim de cada função) e é compilado 
# separadamente dos outros testes.
# 2 - BANDA DE MEMÓRIA: utiliza-se o grupo L3 do LIKWID e apresenta o resultado de "Memory bandwidth [MBytes/s]"
# 3 - CACHE MISS L2: utiliza-se o grupo L2CACHE do LIKWID e apresenta o resultado de "Data cache miss ratio";
# 4 - OPERAÇÕES ARITMÉTICAS: utiliza-se o grupo FLOPS_DP do LIKWID e é apresentado o resultado de FLOPS_DP
# e FLOPS_AVX, ambos em [MFLOPS/s].

#PARÂMETRO $1 = CORE
#PARÂMETRO $2 = PROGRAMA
LIKWID_CMD="likwid-perfctr -C $1"

#Ordem das matrizes que serão efetuados os testes para ambos os métodos:
elements=(32 50 64 100 128 200 256 300 400 512 1000 1024 2000 2048 3000 4000 4096 5000 10000)

#Escala de valores em x
x_range="[10:10000]"

#Resolução de saída dos gráficos gerados pelo gnuplot.
image_size="1600,900" #1600x900

#TESTE DE PERFORMANCE AUTOMATIZADO PARA O PROGRAMA: matmult.
if [[ "$2" == "matmult" ]]; then
	cd matmult
	PROGRAM="./matmult"

	mkdir "RESULTADO DO DESEMPENHO"

	cd matmult_time
	echo "Compilando o programa, com make avx, para realizar a medição de tempo."
	sleep 4
	make avx 
	sleep 4
	clear

	group="time"
	magnitude="milissegundos"
	touch "time.tmp"

	#MEDIÇÃO DE TEMPO
	echo "REALIZANDO O TESTE DE MEDIÇÃO DE TEMPO ..."
	for size in ${elements[@]} ; 
		do
			$PROGRAM -n $size >> $group.tmp
		done
		
		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'purple' lt 1 lw 2 pt 5 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para TEMPO' font ",16" 

		set xrange ${x_range}
		set logscale x
		set logscale y

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatPtrVet.png"
		plot '$group.tmp' using 1:2 with linespoints ls 1 title "multMatPtrVet"
	EOF
		
		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'blue' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para TEMPO' font ",16" 

		set xrange ${x_range}
		set logscale x
		set logscale y

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatRowVet.png"
		plot '$group.tmp' using 1:3 with linespoints ls 1 title "multMatRowVet"
	EOF
		
		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'pink' lt 1 lw 2 pt 9 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para TEMPO' font ",16" 

		set xrange ${x_range}
		set logscale x
		set logscale y

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatColVet.png"
		plot '$group.tmp' using 1:4 with linespoints ls 1 title "multMatColVet"
	EOF


	mv *.png ../RESULTADO\ DO\ DESEMPENHO/
	rm *.tmp
	make purge
	clear
	cd ..

	echo "Compilando o programa, com make avx, para realizar os outros testes."
	sleep 4
	make avx 
	sleep 4
	clear

	for group in FLOPS_DP
	do
		key="DP"
		magnitude="MFLOP/s"

		echo "REALIZANDO O TESTE PARA O GRUPO: $group ..."
		
		for size in ${elements[@]} ; 
		do
			${LIKWID_CMD} -f -g ${group} -m $PROGRAM -n $size > temp.tmp

			printf "$((size))"
			printf "$(grep "$key" temp.tmp | awk -F"|" '{print $3}' | tr "\n" " ")\n"

		done > $group.tmp

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'orange' lt 1 lw 2 pt 13 ps 1.0
		set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $group-multMatPtrVet' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatPtrVet.png"
		plot '$group.tmp' using 1:2 with linespoints ls 1 title "DP", \
		'$group.tmp' using 1:3 with linespoints ls 2 title "AVX DP"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'orange' lt 1 lw 2 pt 13 ps 1.0
		set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $group-multMatRowVet' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatRowVet.png"
		plot '$group.tmp' using 1:4 with linespoints ls 1 title "DP", \
		'$group.tmp' using 1:5 with linespoints ls 2 title "AVX DP"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'orange' lt 1 lw 2 pt 13 ps 1.0
		set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $group-multMatColVet' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatColVet.png"
		plot '$group.tmp' using 1:6 with linespoints ls 1 title "DP", \
		'$group.tmp' using 1:7 with linespoints ls 2 title "AVX DP"
	EOF
	done


	for group in L3 L2CACHE; 
	do
		if [[ "$group" == "L3" ]]; then
			key="L3 bandwidth"
			magnitude="[MBytes/s]"
		
		elif [[ "$group" == "L2CACHE" ]]; then
			key="L2 miss ratio"
			magnitude="ratio"
		fi
		
		echo "REALIZANDO O TESTE PARA O GRUPO: $group ..."

		for size in ${elements[@]} ; 
		do
			${LIKWID_CMD} -f -g ${group} -m $PROGRAM -n $size > temp.tmp

			printf "$((size))"
			printf "$(grep "$key" temp.tmp | awk -F"|" '{print $3}' | tr "\n" " ")\n"

		done > $group.tmp

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'purple' lt 1 lw 2 pt 5 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $key' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatPtrVet.png"
		plot '$group.tmp' using 1:2 with linespoints ls 1 title "multMatPtrVet"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'blue' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $key' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatRowVet.png"
		plot '$group.tmp' using 1:3 with linespoints ls 1 title "multMatRowVet"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'pink' lt 1 lw 2 pt 9 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $key' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-multMatColVet.png"
		plot '$group.tmp' using 1:4 with linespoints ls 1 title "multMatColVet"
	EOF

		mv *.png RESULTADO\ DO\ DESEMPENHO/ ;
	done
	rm *.tmp
	make purge
	clear

	echo "TODOS OS TESTES FORAM CONCLUÍDOS!"
	echo "Os gráficos da medição do desempenho de cada função se encontram no subdiretório RESULTADO DO DESEMPENHO dentro do diretório matmult."


#TESTE DE PERFORMANCE AUTOMATIZADO PARA O PROGRAMA: SistemasLineares
elif [[ "$2" == "SistemasLineares" ]]; then
	cd SistemasLineares
	PROGRAM="./iccTrab02"
	
	mkdir "RESULTADO DO DESEMPENHO"

	cd SistemasLineares_time
	echo "Compilando o programa, com make avx, para realizar a medição de tempo."
	sleep 4
	make avx 
	sleep 4
	clear

	group="time"
	magnitude="milissegundos"
	touch "time.tmp"
	
	#MEDIÇÃO DE TEMPO
	echo "REALIZANDO O TESTE DE MEDIÇÃO DE TEMPO ..."
	for size in ${elements[@]} ; 
		do
			$PROGRAM -n $size >> $group.tmp 2> stderr.txt
		done
		
		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'purple' lt 1 lw 2 pt 5 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para TEMPO' font ",16" 

		set xrange ${x_range}
		set logscale x
		set logscale y

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-eliminacaoGauss.png"
		plot '$group.tmp' using 1:2 with linespoints ls 1 title "eliminacaoGauss"
	EOF
		
		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'blue' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para TEMPO' font ",16" 

		set xrange ${x_range}
		set logscale x
		set logscale y

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-gaussJacobi.png"
		plot '$group.tmp' using 1:3 with linespoints ls 1 title "gaussJacobi"
	EOF
		
		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'pink' lt 1 lw 2 pt 9 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para TEMPO' font ",16" 

		set xrange ${x_range}
		set logscale x
		set logscale y

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-gaussSeidel.png"
		plot '$group.tmp' using 1:4 with linespoints ls 1 title "gaussSeidel"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'green' lt 1 lw 2 pt 13 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para TEMPO' font ",16" 

		set xrange ${x_range}
		set logscale x
		set logscale y

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-normaL2Residuo.png"
		plot '$group.tmp' using 1:5 with linespoints ls 1 title "normaL2Residuo"
	EOF


	mv *.png ../RESULTADO\ DO\ DESEMPENHO/
	rm *.tmp
	make purge
	clear
	cd ..

	echo "Compilando o programa, com make avx, para realizar os outros testes."
	sleep 4
	make avx 
	sleep 4
	clear
	
	for group in FLOPS_DP
	do
		key="DP"
		magnitude="MFLOP/s"

		echo "REALIZANDO O TESTE PARA O GRUPO: $group ..."
		
		for size in ${elements[@]} ; 
		do
			${LIKWID_CMD} -f -g ${group} -m $PROGRAM -n $size > temp.tmp 2> stderr.txt

			printf "$((size))"
			printf "$(grep "$key" temp.tmp | awk -F"|" '{print $3}' | tr "\n" " ")\n"

		done > $group.tmp

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'orange' lt 1 lw 2 pt 13 ps 1.0
		set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $group-eliminacaoGauss' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-eliminacaoGauss.png"
		plot '$group.tmp' using 1:2 with linespoints ls 1 title "DP", \
		'$group.tmp' using 1:3 with linespoints ls 2 title "AVX DP"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'orange' lt 1 lw 2 pt 13 ps 1.0
		set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $group-normaL2Residuo' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-normaL2Residuo.png"
		plot '$group.tmp' using 1:4 with linespoints ls 1 title "DP", \
		'$group.tmp' using 1:5 with linespoints ls 2 title "AVX DP"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'orange' lt 1 lw 2 pt 13 ps 1.0
		set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $group-gaussJacobi' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-gaussJacobi.png"
		plot '$group.tmp' using 1:6 with linespoints ls 1 title "DP", \
		'$group.tmp' using 1:7 with linespoints ls 2 title "AVX DP"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'orange' lt 1 lw 2 pt 13 ps 1.0
		set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $group-gaussSeidel' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-gaussSeidel.png"
		plot '$group.tmp' using 1:8 with linespoints ls 1 title "DP", \
		'$group.tmp' using 1:9 with linespoints ls 2 title "AVX DP"
	EOF
	done


	for group in L3 L2CACHE; 
	do
		if [[ "$group" == "L3" ]]; then
			key="L3 bandwidth"
			magnitude="[MBytes/s]"
		
		elif [[ "$group" == "L2CACHE" ]]; then
			key="L2 miss ratio"
			magnitude="ratio"
		fi
		
		echo "REALIZANDO O TESTE PARA O GRUPO: $group ..."

		for size in ${elements[@]} ; 
		do
			${LIKWID_CMD} -f -g ${group} -m $PROGRAM -n $size > temp.tmp 2> stderr.txt

			printf "$((size))"
			printf "$(grep "$key" temp.tmp | awk -F"|" '{print $3}' | tr "\n" " ")\n"

		done > $group.tmp

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'purple' lt 1 lw 2 pt 5 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $key' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-eliminacaoGauss.png"
		plot '$group.tmp' using 1:2 with linespoints ls 1 title "eliminacaoGauss"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'green' lt 1 lw 2 pt 13 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $key' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-normaL2Residuo.png"
		plot '$group.tmp' using 1:3 with linespoints ls 1 title "normaL2Residuo"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'blue' lt 1 lw 2 pt 7 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $key' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-gaussJacobi.png"
		plot '$group.tmp' using 1:4 with linespoints ls 1 title "gaussJacobi"
	EOF

		gnuplot <<- EOF
		reset
		set terminal png size ${image_size} enhanced font 'Verdana,12' 
		set style data linespoints
		set style fill solid 2.00 border 0

		set style line 1 lc rgb 'pink' lt 1 lw 2 pt 9 ps 1.0
		
		set key font ",10" 
		set key horizontal  
		set key spacing 3 
		set key samplen 3 
		set key width 2

		set title 'Medição de desempenho para $key' font ",16" 

		set xrange ${x_range}
		set logscale x

		set xlabel "tamanho da matriz (doubles)" 
		set xlabel font ",13" 
		set ylabel "$magnitude" 
		set ylabel font ",13"
		
		set output "$group-gaussSeidel.png"
		plot '$group.tmp' using 1:5 with linespoints ls 1 title "gaussSeidel"
	EOF

		mv *.png RESULTADO\ DO\ DESEMPENHO/ ;
	done
	rm *.tmp
	make purge
	clear

	echo "TODOS OS TESTES FORAM CONCLUÍDOS!"
	echo "Os gráficos da medição do desempenho de cada função se encontram no subdiretório RESULTADO DO DESEMPENHO dentro do diretório SistemasLineares."

else
	echo "Os parâmetros nessários são: <CORE DA MÁQUINA A SER UTILIZADO> <PROGRAMA>."
	echo "As opções de programas são: matmult ou SistemasLineares."
	echo "Por favor, entre com os parâmetros corretos para que o script possa efetuar os testes."
fi