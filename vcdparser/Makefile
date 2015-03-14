run: VcdFileReader.o VcdParser.o main.o
	g++ -g main.o VcdFileReader.o VcdParser.o

VcdFileReader.o: VcdFileReader.cc
	g++ -g -c VcdFileReader.cc -o VcdFileReader.o

VcdParser.o: VcdParser.cc
	g++ -g -c VcdParser.cc -o VcdParser.o

main.o: main.cc
	g++ -g -c main.cc -o main.o

clean:
	rm -rf main.o VcdFileReader.o VcdParser.o a.out
