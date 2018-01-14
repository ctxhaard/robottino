# note: gcc < 5.0 bug (https://stackoverflow.com/questions/29320315/error-use-of-deleted-function-stdbasic-ofstream-opencv-and-c11)
#PATH := /home/ctomasin/development/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf/bin:$(PATH)
PATH := /home/ctomasin/development/gcc-linaro-5.1-2015.08-x86_64_arm-linux-gnueabihf/bin:$(PATH)
CROSS_COMPILE ?= arm-linux-gnueabihf-
CXX ?= clang++

ifeq ($(NDEBUG),)
	CXXFLAGS := -std=c++14 -g -Wall -fmessage-length=0
else
	CXXFLAGS := -std=c++14 -O2 -g -Wall -fmessage-length=0
endif

ARCH := arm

OBJS :=	Pathfinder.o ProximitySensor.o MotorController.o PFStatus.o

TARGET := pathfinder

ifeq ($(MAKECMDGOALS),test)
OBJS += test.o
TARGET := pathfinder-test
else
OBJS += main.o
endif

LIBS := -lpthread

USER := root

REMOTE := robottino-wifi

.cpp.o:
	$(CROSS_COMPILE)$(CXX) $(CXXFLAGS) -I /home/ctomasin/development/boost_1_66_0 -o $@ -c $<


$(TARGET): $(OBJS)
	$(CROSS_COMPILE)$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

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
