#!/bin/bash

numTarefas=$1
ficheiro=$2
ficheiroRES="${ficheiro}.res"
ficheiroOLD="${ficheiroRES}.old"
ficheiroSpeedupsCSV="${ficheiro}.speedups.csv"

if [ numTarefas > 0 ]
then

	rm -f ${ficheiroRES}
	rm -f ${ficheiroOLD}
	
	echo "#threads,exec_time,speedup" >> ${ficheiroSpeedupsCSV}

	cd CircuitRouter-SeqSolver 
	./CircuitRouter-SeqSolver ../${ficheiro}
	cd ..

	seqTime=$(grep -Eo '[+-]?([0-9]*\.[0-9]+|[0-9]+){4,4}' ./${ficheiroRES})
	speedup=$(echo "scale=6; ${seqTime}/${seqTime}" | bc)


	echo "1S,${seqTime},${speedup}" >> ${ficheiroSpeedupsCSV}

	for ((i=1;i<=numTarefas;i++))
	do
		cd CircuitRouter-ParSolver
		./CircuitRouter-ParSolver -t $i ../${ficheiro}
		cd ..

		parTime=$(grep -Eo '[+-]?([0-9]*\.[0-9]+|[0-9]+){4,4}' ./${ficheiroRES})
		
		speedup=$(echo "scale=6; ${seqTime}/${parTime}" | bc)
		
		echo "$i,${parTime},${speedup}" >> ${ficheiroSpeedupsCSV}

	done

fi

rm -f 0