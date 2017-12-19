OBJS = main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

ttp: $(OBJS)
		$(CC) $(LFLAGS) $(OBJS) -o ttp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	\rm *.o *~ ttp
