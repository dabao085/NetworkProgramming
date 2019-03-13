CFLAG = -I../include -g -std=c++11
CLIB = 
#TARGET = standardInput #serverwithfork serverwithepoll serverwiththread serverwithselect serverwithunifiedeventsource
OBJS = event_base.o event_loop.o
all: ${OBJS}
${OBJS}: %.o: %.cpp
	g++ ${CFLAG} -c $<

# all: ${TARGET}
# ${TARGET}: %: %.o
# 	g++ -o $@ $< standardInput.o ${CLIB}
clean:
	rm -f *.o