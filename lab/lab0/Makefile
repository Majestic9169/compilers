SHELL:=/bin/bash

.PHONY: all run build
all: build run

build: ass0.cpp
	g++ -Wall -Werror -Weffc++ ass0.cpp -o line

run: line
	@echo "[test]: running sample test case 1"
	./line < 1.in
	@echo "[test]: running final test case 1"
	./line < ./A0-Guidelines/input.txt

clean: 
	rm line *.out
