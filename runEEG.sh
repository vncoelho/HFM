#!/bin/bash

TIME="5 10 20"
VOLUNTEERS="10 20 50 109"
FH="1 10 20 80 160"
MAXNM="1 5 10 30"


for time in $TIME
do
	for  v in $VOLUNTEERS
	do 
		for fh in $FH
		do
			for nm in $MAXNM
			do
		    		echo "Checking exp: 1 fh:$fh nm:$nm time:$time nVolunteers:$v"
		    		./MyProjects/app_HFM 1 $fh $nm $time $v
			done
		done
	done
done

