#!/bin/bash

for ts in `seq 673`
do 
	echo "Resolvendo time series:$ts magLagRate:100 timeES: 10"
	./MyProjects/app_HFM ./a $ts 100 5
done

