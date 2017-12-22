ifeq ($(NDEBUG),)
CXXFLAGS =	-std=c++14 -g -Wall -fmessage-length=0
else
CXXFLAGS =	-std=c++14 -O2 -g -Wall -fmessage-length=0
endif

ARCH = arm

OBJS =	Pathfinder.o

TARGET = pathfinder

ifeq ($(MAKECMDGOALS),test)
CROSS_COMPILE :=
OBJS += test.o
TARGET = pathfinder-test
else
CROSS_COMPILE = /home/ctomasin/development/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
OBJS += main.o
endif

CXX = g++

LIBS =

USER = pi

REMOTE = robottino

.cpp.o:
	$(CROSS_COMPILE)g++ $(CXXFLAGS) -I /opt/boost_1_62_0 -o $@ -c $<


$(TARGET): $(OBJS)
	$(CROSS_COMPILE)g++ -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)

.PHONY: test
test: $(TARGET)
	./$(TARGET)
	
.PHONY: clean
clean:
	rm -f $(OBJS) main.o $(TARGET) pathfinder-test
	
# loads the command on the remote target
.PHONY: load	
load:
	scp $(TARGET) $(user)@$(REMOTE):
