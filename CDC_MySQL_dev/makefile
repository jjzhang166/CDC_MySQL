
#########################################################################
###
###  DESCRIPTION:
###    Common definitions for all Makefiles in UAS linux project.
###
#########################################################################

SRC_DIR := .

COMM_DIR := .

## Name and type of the target for this Makefile

APP_TARGET := test_dev

## Define debugging symbols
DEBUG = 0
LINUX_COMPILER=_LINUX_
PWLIB_SUPPORT = 0

CFLAGS += -fno-omit-frame-pointer
CFLAGS += -D_LINUX

## Object files that compose the target(s)

## SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
## OBJS := $(patsubst %.cpp,%, $(SRCS)) $(patsubst %.c,%, $(SRCS))

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst %.cpp,%, $(SRCS))

## Libraries to include in shared object file

LIBS :=  CDC_MySQL cjson
        

## Add driver-specific include directory to the search path

INC_PATH += . ./MySQL5.7

LIB_PATH := . ../lib

INSTALL_APP_PATH = ../bin

include $(COMM_DIR)/common.mk

clean:
	rm -f $(SRC_DIR)/*.o
