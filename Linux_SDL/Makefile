LIBSDL =  -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL  -lpthread

PROG=proconsumer
CC=g++
INCLS	=  -I/usr/include 

#source codes
SRCS = proconsumer.cpp
#substitute .c by .o to obtain object filenames
OBJS = $(SRCS:.cpp=.o)  
#$< evaluates to the target's dependencies,
#$@ evaluates to the target
 
$(PROG): $(OBJS) 
	echo  $(OBJS)  
	$(CC) -o proconsumer proconsumer.o $(LIBSDL)  

$(OBJS):
	$(CC) -c  $*.cpp $(INCLS) 
                                                                                
clean:
	rm $(OBJS)
