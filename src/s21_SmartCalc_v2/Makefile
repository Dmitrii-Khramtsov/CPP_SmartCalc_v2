# CXX = g++
# CFLAGS = -Wall -Wextra -Werror -std=c++17 -g
# APP = s21_SmartCalc_v2/*

# ifeq ($(OS), Linux)
# 	CHECK_FLAGS := $(shell pkg-config --libs gtest)
# else
# 	CHECK_FLAGS := -lgtest
# endif

# all: install

# build:
# 	LANG=en_US.UTF-8
# 	export LANG
# 	mkdir s21_SmartCalc_v2
# 	cd s21_SmartCalc_v2 && cmake -B build "../" && make -C build s21_SmartCalc_v2

# install: build
# 	mkdir $(HOME)/Desktop/s21_SmartCalc_v2/
# 	cp -rf $(APP) $(HOME)/Desktop/s21_SmartCalc_v2/
# 	make clean

# uninstall:
# 	rm -rf $(HOME)/Desktop/s21_SmartCalc_v2/

# launch:
# 	LANG=en_US.UTF-8
# 	export LANG
# 	open $(HOME)/Desktop/s21_SmartCalc_v2/build/s21_SmartCalc_v2.app

# dvi:
# 	open ../documentation/html/index.html

# dist:
# 	rm -rf ../Archive_s21_SmartCalc_v2/
# 	mkdir ../Archive_s21_SmartCalc_v2/
# 	cp -r ../s21_SmartCalc_v2/ ../Archive_s21_SmartCalc_v2/
# 	cp -r ../documentation/ ../Archive_s21_SmartCalc_v2/
# 	tar -C ../ -czvf Archive_s21_SmartCalc_v2.tgz Archive_s21_SmartCalc_v2/
# 	mv Archive_s21_SmartCalc_v2.tgz $(HOME)/Desktop/
# 	rm -rf ../Archive_s21_SmartCalc_v2/

# test: clean
# 	$(CXX) $(CFLAGS) tests/*.cc model/*.cc -o test $(CHECK_FLAGS)
# 	./test

# check:
# 	clang-format -style=Google -n model/*.cc model/*.h controller/*.cc controller/*.h view/*.cpp view/*.h tests/*.cc
# 	clang-format -style=Google -i model/*.cc model/*.h controller/*.cc controller/*.h view/*.cpp view/*.h tests/*.cc
# 	clang-format -style=Google -n model/*.cc model/*.h controller/*.cc controller/*.h view/*.cpp view/*.h tests/*.cc
# 	make tests
# 	CK_FORK=no leaks --atExit -- ./test

# style:
# 	clang-format -n -style=Google *.h *.c mainwindow.cpp main.cpp 
	
# fix-style:
# 	clang-format -i -style=Google *.h *.c mainwindow.cpp main.cpp 

# clean:
# 	rm -rf *.o *.a
# 	rm -rf s21_SmartCalc_v2
# 	rm -rf test


CXX = g++
CFLAGS = -Wall -Wextra -Werror -std=c++17 -g
APP = build/s21_SmartCalc_v2.app

ifeq ($(OS), Linux)
    CHECK_FLAGS := $(shell pkg-config --libs gtest)
else
    CHECK_FLAGS := -lgtest
endif

all: install

build:
	LANG=en_US.UTF-8; export LANG; \
	mkdir -p build; \
	cd build && cmake .. && make

install: build
	mkdir -p $(HOME)/Desktop/s21_SmartCalc_v2/
	cp -rf $(APP) $(HOME)/Desktop/s21_SmartCalc_v2/
	make clean

uninstall:
	rm -rf $(HOME)/Desktop/s21_SmartCalc_v2/

launch:
	LANG=en_US.UTF-8; export LANG; \
	open $(HOME)/Desktop/s21_SmartCalc_v2/s21_SmartCalc_v2.app

dvi:
	open ../documentation/html/index.html

dist:
	rm -rf ../Archive_s21_SmartCalc_v2/
	mkdir ../Archive_s21_SmartCalc_v2/
	cp -r ../s21_SmartCalc_v2/ ../Archive_s21_SmartCalc_v2/
	cp -r ../documentation/ ../Archive_s21_SmartCalc_v2/
	tar -C ../ -czvf Archive_s21_SmartCalc_v2.tgz Archive_s21_SmartCalc_v2/
	mv Archive_s21_SmartCalc_v2.tgz $(HOME)/Desktop/
	rm -rf ../Archive_s21_SmartCalc_v2/

test: clean
	$(CXX) $(CFLAGS) tests/*.cc model/*.cc -o test $(CHECK_FLAGS)
	./test

check:
	clang-format -style=Google -n model/*.cc model/*.h controller/*.cc controller/*.h view/*.cpp view/*.h tests/*.cc
	clang-format -style=Google -i model/*.cc model/*.h controller/*.cc controller/*.h view/*.cpp view/*.h tests/*.cc
	clang-format -style=Google -n model/*.cc model/*.h controller/*.cc controller/*.h view/*.cpp view/*.h tests/*.cc
	make tests
	CK_FORK=no leaks --atExit -- ./test

style:
	clang-format -n -style=Google *.h *.c mainwindow.cpp main.cpp

fix-style:
	clang-format -i -style=Google *.h *.c mainwindow.cpp main.cpp

clean:
	rm -rf *.o *.a
	rm -rf build
	rm -rf test
