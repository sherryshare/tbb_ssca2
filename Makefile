# Copyright 2005-2013 Intel Corporation.  All Rights Reserved.
#
# This file is part of Threading Building Blocks.
#
# Threading Building Blocks is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
#
# Threading Building Blocks is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Threading Building Blocks; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# As a special exception, you may use this file as part of a free software
# library without restriction.  Specifically, if other files instantiate
# templates or use macros or inline functions from this file, or you compile
# this file and link it with other files to produce an executable, this
# file does not by itself cause the resulting executable to be covered by
# the GNU General Public License.  This exception does not however
# invalidate any other reasons why the executable file might be covered by
# the GNU General Public License.

# GNU Makefile that builds and runs example.
run_cmd=
PROG=tbbssca
#ARGS=4
#PERF_RUN_ARGS=4

# The C++ compiler
CXX=g++
CXXFLAGS=-std=c++11
CC=gcc
CFLAGS=-O3
#`wx-config --libs` `wx-config --cxxflags`
SOURCES = bc_seq.cpp main.cpp read_data.cpp util.cpp bc_tbb.cpp monitor.cpp

ifeq ($(shell uname), Linux)
ifeq ($(target), android)
LIBS+= --sysroot=$(SYSROOT)
run_cmd=../../common/android.linux.launcher.sh
else
LIBS+= -lrt -lboost_program_options
LIBS+= -Lsprng2.0/lib -lm -lsprng -lpthread
#LIB = -Lsprng2.0/lib -lm -lsprng
INC = -Isprng2.0/include
endif
endif

#.c.o: defs.h Makefile
#        $(CC) $(INC) $(CFLAGS) -c -fopenmp $<

all: sprng release

release: $(SOURCES)
	$(CXX) -O2 -DNDEBUG $(INC) $(CXXFLAGS) -o $(PROG) $^ -ltbb $(LIBS)

debug: $(SOURCES)
	$(CXX) -O0 -g -DTBB_USE_DEBUG $(CXXFLAGS) -o $(PROG) $^ -ltbb_debug $(LIBS)

sprng: 
	(cd sprng2.0; $(MAKE); cd ..)

cleansprng: 
	(cd sprng2.0; $(MAKE) clean; cd ..)

clean: cleansprng
	$(RM) $(PROG) *.o *.d *~ *core* out* *.json *.txt

test:
	$(run_cmd) ./$(PROG) $(ARGS)

perf_build: $(SOURCES)
	$(CXX) -O2 -msse2 -DNDEBUG $(CXXFLAGS) -o $(PROG) $^ -ltbb $(LIBS)

# -std=c++0x -I. -c
# perf_run:
# 	./$(PROG) $(PERF_RUN_ARGS)




# all:
#	g++ run_canny.cpp canny_edge_detector.cpp `wx-config --libs` `wx-config --cxxflags` -o EdgeApp

