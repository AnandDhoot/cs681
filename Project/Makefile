OK_COLOR=\x1b[32;01m
NO_COLOR=\033[0m
CO_STRING=$(OK_COLOR)[Compiling..]$(NO_COLOR)
CL_STRING=$(OK_COLOR)[Cleaning Up..]$(NO_COLOR)
all:
	@/bin/echo -e "$(CO_STRING)"
	@g++ -g -c -std=c++11 server.cpp
	@#g++ -g -c client.cpp
	@g++ -o a.out server.o #client.o 
clean:
	@/bin/echo -e "$(CL_STRING)"
	@rm a.out *.o
run: all
	./a.out