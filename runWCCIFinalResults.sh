#!/bin/bash


for ts in `seq 72`
do 
	echo "Resolvendo time series:$ts"
	./MyProjects/app_HFM ./a $ts 40 20
done

