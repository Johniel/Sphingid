CC = g++
OUTDIR = ./target
OPT = -std=c++0x -g
OBJS = ./target/parser.o ./target/lexer.o ./target/ast.o ./target/main.o

all: $(OBJS)
	g++ $(OBJS) -o ./target/a.out

clean:
	rm ./target/*

run:
	./target/a.out < ./test/sample.sp

./target/parser.o: ./src/parser/parser.cpp ./src/parser/parser.hpp
	g++ -c ./src/parser/parser.cpp -o $@ $(OPT)

./target/lexer.o: ./src/parser/lexer.cpp ./src/parser/lexer.hpp
	g++ -c ./src/parser/lexer.cpp -o $@ $(OPT)

./target/ast.o: ./src/ast/ast.cpp ./src/ast/ast.hpp
	g++ -c ./src/ast/ast.cpp -o $@ $(OPT)

./target/main.o: ./src/main.cpp
	g++ -c ./src/main.cpp -o $@ $(OPT)
