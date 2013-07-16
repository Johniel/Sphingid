CC = g++
OUTDIR = ./target
OPT = -std=c++0x -g
OBJS = ./target/parser.o ./target/lexer.o ./target/ast.o ./target/interpreter.o ./target/environment.o ./target/main.o

all: $(OBJS)
	g++ $(OBJS) -o ./target/a.out

clean:
	rm ./target/*

run:
#	./target/a.out < ./test/sample.sp
	./target/a.out < ./test/fib.sp

gdb:
	gdb ./target/a.out

./target/interpreter.o: ./src/interpreter/interpreter.cpp ./src/interpreter/interpreter.hpp
	g++ -c ./src/interpreter/interpreter.cpp -o $@ $(OPT)

./target/environment.o: ./src/interpreter/environment.cpp ./src/interpreter/environment.hpp
	g++ -c ./src/interpreter/environment.cpp -o $@ $(OPT)

./target/parser.o: ./src/parser/parser.cpp ./src/parser/parser.hpp
	g++ -c ./src/parser/parser.cpp -o $@ $(OPT)

./target/lexer.o: ./src/parser/lexer.cpp ./src/parser/lexer.hpp
	g++ -c ./src/parser/lexer.cpp -o $@ $(OPT)

./target/ast.o: ./src/ast/ast.cpp ./src/ast/ast.hpp
	g++ -c ./src/ast/ast.cpp -o $@ $(OPT)

./target/main.o: ./src/main.cpp
	g++ -c ./src/main.cpp -o $@ $(OPT)
