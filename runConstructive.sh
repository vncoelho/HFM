#!/bin/bash

TIMESERIES="0 1 2 3"
ACF="-1 -0.3 -0.1 0 0.1 0.3 0.5 0.8 1"
ITERGRASP="10 100 500 1000 3000"
NRULES="10 50 100 500 1000"

for i in `seq 10`
do 
	for ts in $TIMESERIES
	do
		  for iter in $ITERGRASP
		  do
		 	  for rules in $NRULES
			  do
	 			  for acf in $ACF
				  do
			    		echo "Resolvendo time series batch: $i :$ts iterGrasp:$iter  c:ACF nRules:$rules acfLimitTemp:$acf"
			    		./MyProjects/app_HFM ./a $ts $iter 2 $rules $acf
				  done
			    	
				  echo "Resolvendo time series batch:$i  $ts iterGrasp:$iter  c:random nRules:$rules acfLimitTemp:$acf"
			    		./MyProjects/app_HFM ./a $ts $iter 0 $rules $acf
			  done
		  done
	done
done

