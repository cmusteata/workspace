target  :=intro
objects :=main.o algo.o yaml.o bridge.o

CC      :=g++
CFLAGS  :=-std=c++11 -O3 -Wall -Wextra -Werror -I ../introlib

all: $(target)

intro: $(objects)
	$(CC) $^ -o $@

main.o: main.cpp algo.h
	$(CC) $(CFLAGS) -c $<

algo.o: algo.cpp algo.h ../introlib/yaml.h ../introlib/bridge.h
	$(CC) $(CFLAGS) -c $<

%.o: ../introlib/%.cpp ../introlib/%.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(objects) $(target)

