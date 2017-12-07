all: app_TP

SRC_TP = $(wildcard ./MyProjects/TP/*.cpp) ./MyProjects/mainTP.cpp

app_TP: $(SRC_TP:%.cpp=%.o) ./TPScanner.o
	g++ $(CPPFLAGS) -o $@ $^

TPScanner.o: ./OptFrame/Scanner++/Scanner.cpp
	g++ -c $(CPPFLAGS) ./OptFrame/Scanner++/Scanner.cpp -o TPScanner.o

-include $(SRC_TP:%.cpp=%.d)

CPPFLAGS = --std=c++11 -Wall -Ofast  -g
#CPPFLAGS += -MD -MP

clean: 
	rm -f app_*
	rm -f *.o *.d
	rm -f TP/*.o TP/*.d
