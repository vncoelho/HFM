all: HFM
	@echo "BUILT SUCCESSFULLY!"

CPLEXDIR    = /opt/ibm/ILOG/CPLEX_Studio1251/cplex
CONCERTDIR  = /opt/ibm/ILOG/CPLEX_Studio1251/concert
CCLNFLAGS = -L$(CPLEXDIR)/lib/x86-64_sles10_4.1/static_pic -DIL_STD -lilocplex -lcplex -L$(CONCERTDIR)/lib/x86-64_sles10_4.1/static_pic -lconcert -m64 -lm -pthread
CCINCFLAG = -I$(CPLEXDIR)/include -I$(CONCERTDIR)/include 


#OpenMP Multi-Core Multi-Thread flags
OPENMP_FLAGS = -fopenmp -lpthread

RESTSDK_FGLAS = #-lboost_system -lcrypto -lssl -lcpprest

#Raspberry WiringPi Flags (Package GPIO, Copyright (c) 2012-2017 Gordon Henderson, is required)
LDFLAGS	= -L/usr/local/lib
LDLIBS  = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

LLIBSMP3 = -lsndfile 
GCC_FLAGS = -g -Ofast -Wall  --std=c++11  

SCANNERFLAGS = ./OptFrame/Scanner++/Scanner.cpp
#GCC_FLAGS = -Ofast -g --std=c++11

#-Wall 
HFM:
	g++ $(GCC_FLAGS) $(RESTSDK_FGLAS) $(OPENMP_FLAGS) $(SCANNERFLAGS) ./MyProjects/mainHFM.cpp -o ./MyProjects/app_HFM
	
clean:
	#make clean -C ./MyProjects/
	
