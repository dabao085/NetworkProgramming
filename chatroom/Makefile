TARGET = chatroomclient chatroomserver
OBJS = chatroomclient.o chatroomserver.o chatroom.o
CFLAG = 
all: ${OBJS}
${OBJS}: %.o: %.cpp
	g++ ${CFLAG} -c $<

all: ${TARGET}
${TARGET}: %: %.o
	g++ -o $@ $< chatroom.o

clean:
	rm -f ${TARGET} *.o