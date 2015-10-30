#!/bin/bash

tamano=$1

echo "$# argumentos del programa $0"

if [ "$#" -ne 2 ] 
then
	echo "./$0 <tamano_individuo> <archivo_cadena>"
	exit 1
fi


#if [ "$#" -gt 0 ]
#then
#	echo "Filtrando traza única por dirección MAC: $1"
#	# filtrar la traza ordenada por una MAC address especifica
#	awk -v arg1=$1 'BEGIN{FS=",";}  { if ($2 == arg1) print $1","$2","$3","$4","$5","$6","$7}' fullScanningOrdered.txt > fullScanningOrderedMAC.txt
#
# filtrar la traza ordenada por una MAC address especifica
#awk 'BEGIN{FS=",";}  { if ($2 == MAC) print $1","$2","$3","$4","$5","$6","$7}' fullScanningOrdered.txt > fullScanningOrderedMAC.txt
#	output="fullScanningOrderedMAC.txt"
#
#fi
awk -v tamano=$1 'BEGIN{FS=",";} {print "hola mundo";}

#awk -v tamano=$1 'BEGIN{FS=",";} {print #$1","$2","$3","$5","$6","$7","$9","$10","$11","$13","$14","$15","$17","$18","$19","$21","$22","$23","$25","$26","$27","$29","$30","$31","$33","$34","$35","$37","$38","$39","$41","$42","$43;
#}' $2

exit 0
