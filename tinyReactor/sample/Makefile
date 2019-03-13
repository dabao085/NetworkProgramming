CFLAG = -I../include -g -std=c++11
CLIB = 
TARGET = standardInput
OBJS = standardInput.o
all: ${OBJS}
${OBJS}: %.o: %.cpp
	g++ ${CFLAG} -c $<

all: ${TARGET}
${TARGET}: %: %.o
	g++ -o $@ $< ../src/event_loop.o ${CLIB}
clean:
	rm -f *.o