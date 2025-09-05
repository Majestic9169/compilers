SHELL:=/bin/bash

.PHONY: all run build
all: build run

build: ass0.cpp
	g++ -Wall -Werror -Weffc++ ass0.cpp -o line

run: line
	./line < 1.in

clean: 
	rm line *.out
