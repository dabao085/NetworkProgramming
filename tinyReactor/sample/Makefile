CFLAG = -I../include -g -std=c++11
CLIB = 
TARGET = standardInput simpleServer
OBJS = standardInput.o simpleServer.o
all: ${OBJS}
${OBJS}: %.o: %.cpp
	g++ ${CFLAG} -c $<

all: ${TARGET}
${TARGET}: %: %.o
	g++ -o $@ $< ../src/event_loop.o ../src/network.o ${CLIB}
clean:
	rm -f *.o