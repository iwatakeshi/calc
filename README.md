# calc

A simple and slick calculator for your terminal.

![calc](images/calc.png "calc")

## Usage

```
usage: calc [options]
-h  --help           print this usage and exit
-v  --version        print version information and exit
-r  --repl           use read-eval-print loop mode
-t  --tree           print the syntax tree
-b  --output-binary  print the result in binary
-o  --output-octal   print the result in octal
-x  --output-hex     print the result in hexidecimal
```

## Installation

```bash
# To compile, run make
make
# Change the permissions
chmod +x ./calc
# Move it to the bin folder
sudo mv ./calc /usr/local/bin/
# Install the man page
make man

# or to perform all the tasks above
make install

# To remove the man page and calc, just run:
make uninstall
```

## Description

calc is a simple and slick calculator for your terminal. 
This calculator supports the basic operators such as `+`, `-`, `*`, `/`, as well as
the advanced operators `%`, and `^`. In addition, calc also supports scientific notation like
`1e4`, `2.34e2`, etc. Lastly, calc can also print the abstract syntax tree in Lisp format.

