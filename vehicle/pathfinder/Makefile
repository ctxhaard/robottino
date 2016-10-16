ifeq ($(NDEBUG),)
CXXFLAGS =	-std=c++14 -g -Wall -fmessage-length=0
else
CXXFLAGS =	-std=c++14 -O2 -g -Wall -fmessage-length=0
endif


CXX = g++

OBJS =		main.o Pathfinder.o

LIBS =

TARGET =	pathfinder

USER = pi
REMOTE = robottino

.cpp.o:
	$(CROSS_COMPILE)g++ $(CXXFLAGS) -o $@ -c $<



$(TARGET):	$(OBJS)
	$(CROSS_COMPILE)g++ -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
	
# loads the command on the remote target
.PHONY: load	
load:
	scp $(TARGET) $(user)@$(REMOTE):
