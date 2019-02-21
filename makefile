CFLAG = -Iinclude -g
TARGET = client/clientpresstest server/serversimple server/serverwithfork server/network server/serverwithepoll server/serverwiththread

DIR_OBJS = 

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

OBJS := $(addprefix $(DIR_OBJS)/,$(OBJS))
EXEC := $(addprefix $(DIR_EXEC)/,$(EXEC))

all: ${TARGET}

client/clientpresstest.o: client/clientpresstest.cpp include/network.h
	g++ -c $@ $< ${CFLAG} 
server/serversimple.o:	server/serversimple.cpp include/network.h
	g++ -c $@ $< ${CFLAG} 
server/serverwithfork.o: server/serverwithfork.cpp include/network.h
	g++ -c $@ $< ${CFLAG} 
server/network.o: server/network.cpp include/network.h
	g++ -c $@ $^ ${CFLAG}
server/serverwithepoll.o: server/serverwithepoll.cpp include/network.h
	g++ -c $@ $^ ${CFLAG}
server/serverwiththread.o: server/serverwiththread.cpp include/network.h
	g++ -c $@ $^ ${CFLAG}

client/clientpresstest: client/clientpresstest.o server/network.o
	g++ -o $@ $^ ${CFLAG}
server/serversimple: server/serversimple.o server/network.o
	g++ -o $@ $^ ${CFLAG}
server/serverwithfork: server/serverwithfork.o server/network.o
	g++ -o $@ $^ ${CFLAG}
server/serverwithepoll: server/serverwithepoll.o server/network.o
	g++ -o $@ $^ ${CFLAG}
server/serverwiththread: server/serverwiththread.o server/network.o
	g++ -o $@ $^ ${CFLAG}

clean:
	rm -f ${TARGET} */*.o *.o