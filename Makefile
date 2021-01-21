CXX = clang++
CXXFLAGS = -O3 -Wall
LDFLAGS = -fuse-ld=lld -flto=thin
#i can't download libc++ in this laptop but add -stdlib=libc++

all: main

main: main.o sudoku.o pentominoes.o dlx.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.o sudoku.o pentominoes.o dlx.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

pentominoes.o:
	$(CXX) $(CXXFLAGS) -c pentominoes.cpp

sudoku.o:
	$(CXX) $(CXXFLAGS) -c sudoku.cpp

dlx.o:
	$(CXX) $(CXXFLAGS) -c dlx.cpp

clean:
	rm -rf pentominoes.o sudoku.o dlx.o main.o a.out
