CFLAG = -I../include -g
CLIB = -lpthread
TARGET = clientpresstest
OBJS = clientpresstest.o
all: ${OBJS}
${OBJS}: %.o: %.cpp
	g++ ${CFLAG} -c $<

all: ${TARGET}
${TARGET}: %: %.o
	g++ -o $@ $<
clean:
	rm -f ${TARGET} *.o