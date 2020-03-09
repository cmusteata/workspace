objects :=yaml.o bridge.o

CC      :=g++
CFLAGS  :=-std=c++11 -O3 -Wall -Wextra -Werror

all: $(objects)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(objects)

