TARGET=test
OBJS=test.o
INC=/usr/keygoe/inc
LIBPATH=/usr/keygoe/dll

%.o:%.cpp
	g++ -c -g -I$(INC) -I. $< -o $@ 

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -L$(LIBPATH) -lncurses -lDJITPCom -lDJAcsAPI -o $(TARGET)

.PHONY:clean

clean:
	rm *.o $(TARGET)
	
	
