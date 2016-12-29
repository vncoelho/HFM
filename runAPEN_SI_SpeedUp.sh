#!/bin/bash

PS=" ./REED/channel_1.dat
"

SAMPLESTRAININGSET="1000000
500000
200000
100000
50000
10000
5000
"

FORECASTINGHORIZONMIN="
60
360
720
1800
3600
7200
"

THREADS=" 1
2
3
4
5
6
7
"


for batch in `seq 2`
do
	for probl in $PS
	do
		for fh in $FORECASTINGHORIZONMIN
		do
	
		  for nSamples in $SAMPLESTRAININGSET
		  do 
		  		  for tr in $THREADS
		  			do
		     totalNSamples=$(($nSamples + $fh))
		     echo "Resolvendo o problema $probl forecastingHorizon: $fh and number of samples: $nSamples /  $totalNSamples ,  tr: $tr"		
		    ./MyProjects/app_HFM $probl ./teste $totalNSamples $fh $tr
	       		done
		  done
	  done
	done
done


