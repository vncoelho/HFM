#!/bin/bash

TIMESERIES="0 1 2 3"
MAXLAGRATE="0 5 10 20 25 40 50 60 70 80 90 95 100"

for ts in `seq 72`
do 
	for mlr in $MAXLAGRATE
	do
		    		echo "Resolvendo time series:$ts magLagRate:$mlr timeES: 5"
		    		./MyProjects/app_HFM ./a $ts $mlr 5
	done
done

