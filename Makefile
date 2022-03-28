all: main.cpp heap.cpp ratio.cpp bst.cpp hash.cpp range.cpp
	g++ -std=c++11 -o earnings main.cpp heap.cpp ratio.cpp bst.cpp hash.cpp range.cpp

clean : 
	rm *.exe