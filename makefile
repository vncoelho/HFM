all: HFM_EMCC #HFM  #clean #TP 
	@echo "BUILT SUCCESSFULLY!"


#Maybe use CPLEX solver for optimal linear regression
#Deprecated from now
#Time for BBV
CPLEXDIR    = /opt/ibm/ILOG/CPLEX_Studio1251/cplex
CONCERTDIR  = /opt/ibm/ILOG/CPLEX_Studio1251/concert
CCLNFLAGS = -L$(CPLEXDIR)/lib/x86-64_sles10_4.1/static_pic -DIL_STD -lilocplex -lcplex -L$(CONCERTDIR)/lib/x86-64_sles10_4.1/static_pic -lconcert -m64 -lm -pthread
CCINCFLAG = -I$(CPLEXDIR)/include -I$(CONCERTDIR)/include 


RESTSDK_FGLAS = #-lboost_system -lcrypto -lssl -lcpprest

#Used with music forecasting/composition experiments 
LLIBSMP3 = -lsndfile 

#OpenMP Multi-Core Multi-Thread flags
OPENMP_FLAGS = #-fopenmp -lpthread

#Raspberry WiringPi Flags (Package GPIO, Copyright (c) 2012-2017 Gordon Henderson, is required)
LDFLAGS	= -L/usr/local/lib
LDLIBS  = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

GCC_FLAGS = -g -Ofast -Wall --std=c++1z
EMCC_FLAGS = -g -O3 -Wall -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 --std=c++1z 
#-s DISABLE_EXCEPTION_CATCHING=0 
# -s ALLOW_MEMORY_GROWTH=1
EMCC_EXPORTED_FUNCTIONS = -s EXPORTED_FUNCTIONS="['_loadTS','_callTSForecasting','_getNTimeSeries','_getTimeSeriesSize']"
PATH_EMCC = "/media/vitor/DadosEXT4/emscripten/em++"

SCANNERFLAGS = ./OptFrame/Scanner++/Scanner.cpp

HFM:
	g++ $(GCC_FLAGS) $(RESTSDK_FGLAS) $(OPENMP_FLAGS) $(SCANNERFLAGS) ./MyProjects/mainHFM.cpp -o ./MyProjects/app_HFM

	
HFM_EMCC:
	 /media/vitor/DadosEXT4/emscripten/em++ $(EMCC_EXPORTED_FUNCTIONS) $(EMCC_FLAGS) $(RESTSDK_FGLAS) $(OPENMP_FLAGS) $(SCANNERFLAGS) ./MyProjects/mainHFM.cpp -o ./MyProjects/hfm-converted.js
#	 cp ./MyProjects/app_HFM.js /website/js/hfm-converted.js
#	 cp ./MyProjects/app_HFM.js.mem /website/js/hfm-converted.js.mem
	 
TP:
	make -f makeTP.mk
	
clean:
	rm -f ./MyProjects/app_*
	rm -f ./MyProjects/*.o
	
