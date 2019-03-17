 .PHONY: man uninstall

 all:
	g++ -g -std=c++17 -o calc main.cpp

man:
	sudo cp man/calc /usr/share/man/man1/calc.1
	sudo gzip /usr/share/man/man1/calc.1

install:
	g++ -g -std=c++17 -o calc main.cpp
	chmod +x ./calc
	sudo mv ./calc /usr/local/bin
	sudo cp man/calc /usr/share/man/man1/calc.1
	sudo gzip /usr/share/man/man1/calc.1

uninstall:
	sudo rm /usr/share/man/man1/calc.1.gz
	sudo rm /usr/local/bin/calc