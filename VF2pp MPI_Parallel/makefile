# 
# Course: High Performance Computing 2023/2024
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Student:
# Pepe Lorenzo        0622702121      l.pepe29@studenti.unisa.it          
# 
#
#               REQUIREMENTS OF THE ASSIGNMENT:
# Design, Implement, and Analyze a parallel version of VF2-pp based on MPI
#
#
# Copyright (C) 2024 - All Rights Reserved
#
# This program is free software: you can redistribute it and/or modify it under the terms of 
# the GNU General Public License as published by the Free Software Foundation, either version 
# 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with ContestOMP. 
# If not, see <http://www.gnu.org/licenses/>.
#

CC = mpicc
BASE_CFLAGS = -Wall -Wextra -I${INC_DIR}
LDFLAGS = -lm

OPTIMIZATION ?= -O2

EXTRA_CFLAGS ?=
EXTRA_LDFLAGS ?=

MPI_CFLAGS = 
MPI_LDFLAGS = 

CFLAGS = $(BASE_CFLAGS) $(MPI_CFLAGS) $(OPTIMIZATION) $(EXTRA_CFLAGS)
LDFLAGS += $(MPI_LDFLAGS) $(EXTRA_LDFLAGS)

SRC_DIR = src
INC_DIR = include
GRAPH_DIR = Graphs
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

TARGET = VF2pp_parallel

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: all
	@if [ ! -d "$(GRAPH_DIR)" ]; then \
		echo "Error: Graph directory not found!"; \
		exit 1; \
	fi
	./$(TARGET)

.PHONY: all clean run

debug: OPTIMIZATION = -O0 -g
debug: all

release: OPTIMIZATION = -O3
release: all

profile: OPTIMIZATION = -O2 -pg
profile: LDFLAGS += -pg
profile: all
