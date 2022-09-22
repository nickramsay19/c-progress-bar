CC = gcc

default: dist/main

dist/main: dist build/main.o
	gcc -o dist/main build/main.o

build/main.o: build 
	gcc -o build/main.o -c src/main.c

dist:
	mkdir dist

build:
	mkdir build

.PHONY: clean
clean: 
	rm -Rf dist build
