#!/bin/bash

INSTANCE="windProdComplete solarProdComplete consumptionComplete"

for i in $INSTANCE
do
    		echo "Resolvendo batch:$i "
 		./MyProjects/mainEFP $i QQ$i
done


