# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: rest

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
rest: bin/Table.o bin/Restaurant.o bin/Dish.o bin/Main.o bin/Customer.o bin/Action.o
	@echo 'Building target: hello'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/rest bin/Table.o bin/Restaurant.o bin/Dish.o bin/Main.o bin/Customer.o bin/Action.o $(LFLAGS)
	@echo 'Finished building target: hello'
	@echo ' '

# Depends on the source and header files
bin/Table.o: src/Table.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp

# Depends on the source and header files 
bin/Restaurant.o: src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

# Depends on the source and header files 
bin/Dish.o: src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp

# Depends on the source and header files 
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files 
bin/Customer.o: src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp


# Depends on the source and header files 
bin/Action.o: src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp

#Clean the build directory
clean: 
	rm -f bin/*

