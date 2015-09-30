#!/bin/bash

NTR="5 10 50 100"
ACF="-1 -5 0 5 8 9"
TEMPVAR="0 1 2 3"


for i in `seq 3`
do 
	  for ntr in $NTR
	  do
	 	  for acf in $ACF
		  do
 			  for nTempVar in $TEMPVAR
		          do
		    		echo "Resolvendo batch:$i ntr:$ntr acfLimitTemp:$acf nTempVar:$nTempVar"
		    		./MyProjects/mainEFP $ntr $acf $nTempVar
			  done
		  done
	  done
done

