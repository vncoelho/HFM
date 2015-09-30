#!/bin/bash

TIMESERIES="0 1 2 3"
ACF="-1 -0.3 -0.1 0 0.1 0.3 0.5 0.8 1"
NRULES="10 50 100 500 1000"

for i in `seq 10`
do 
	for ts in $TIMESERIES
	do
		 for rules in $NRULES
		 do
	 		  for acf in $ACF
			  do
		    		echo "Resolvendo time series batch: $i :$ts 0  c:ACF nRules:$rules acfLimitTemp:$acf"
		    		./MyProjects/app_HFM ./a $ts 0 2 $rules $acf
			  done
		    	
			  echo "Resolvendo time series batch:$i  $ts 0  c:random nRules:$rules acfLimitTemp:$acf"
			  ./MyProjects/app_HFM ./a $ts 0 0 $rules $acf
		  done
	done
done

