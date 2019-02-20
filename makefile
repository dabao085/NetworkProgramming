CFLAG = -Iinclude -g
TARGET = client/clientpresstest server/serversimple server/serverwithfork

all: ${TARGET}

client/clientpresstest: client/clientpresstest.o
	g++ -o $@ $^ ${CFLAG}
server/serversimple: server/serversimple.o
	g++ -o $@ $^ ${CFLAG}
server/serverwithfork: server/serverwithfork.o
	g++ -o $@ $^ ${CFLAG}

client/clientpresstest.o: client/clientpresstest.cpp include/network.h
	g++ -c -o $@ $< ${CFLAG} 
server/serversimple.o:	server/serversimple.cpp include/network.h
	g++ -c -o $@ $< ${CFLAG} 
server/serverwithfork.o: server/serverwithfork.cpp include/network.h
	g++ -c -o $@ $< ${CFLAG} 

clean:
	rm -f ${TARGET} */*.o *.o