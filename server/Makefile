CFLAG = -I../include -g
CLIB = -lpthread
TARGET = serversimple serverwithfork serverwithepoll serverwiththread serverwithselect serverwithunifiedeventsource
OBJS = serversimple.o serverwithfork.o serverwithepoll.o serverwiththread.o network.o serverwithselect.o serverwithunifiedeventsource.o
all: ${OBJS}
${OBJS}: %.o: %.cpp
	g++ ${CFLAG} -c $<

all: ${TARGET}
${TARGET}: %: %.o
	g++ -o $@ $< network.o ${CLIB}
clean:
	rm -f ${TARGET} *.o