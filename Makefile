CXX=g++
CXXFLAGS=-Wall -std=c++17
RM=rm -rf

EXEC=main

$(EXEC) : main.o agenda.o event.o date.o
	$(CXX) $(CXXFLAGS) -o $(EXEC) main.o agenda.o event.o date.o

date.o : includes/date.cpp includes/date.hpp
	$(CXX) $(CXXFLAGS) -c includes/date.cpp

event.o : includes/event.cpp includes/event.hpp includes/date.hpp
	$(CXX) $(CXXFLAGS) -c includes/event.cpp

agenda.o : includes/agenda.cpp includes/agenda.hpp includes/event.hpp
	$(CXX) $(CXXFLAGS) -c includes/agenda.cpp

main.o : main.cpp includes/agenda.hpp includes/event.hpp includes/date.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean :
	$(RM) $(EXEC) main *.o
	