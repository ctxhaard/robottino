#CXXFLAGS =	-std=c++14 -O2 -g -Wall -fmessage-length=0
CXXFLAGS =	-std=c++14 -g -Wall -fmessage-length=0

OBJS =		main.o Pathfinder.o

LIBS =

TARGET =	pathfinder

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
