# PasC

PasC is a compiled language, still under construction, aimed to teach structured programming for programmers immersed in the Portuguese language. It was developed based on the Pascal language, because it is a more robust language oriented to teaching, and C, due to the presence of structures of repetition better modeled.

### Dependencies

```bash
sudo apt install g++
sudo apt install make
sudo apt install flex

# http://techapple.net/2014/10/install-lex-yacc-ubuntu-linuxmint-easily-works-versions/
sudo apt-get install bison
sudo apt-get install byacc
sudo apt-get install bison++
```


## Running Pasc codes

### Running syntax parser
Execute by passing the input code as a parameter

```bash
 ./run.sh codes/basic 
```
#### Where:

```basic`` is the name of the PasC code that you want to run.

## Pascal codes

### Dependencies

```bash
sudo apt-get install fp-compiler
```
### Running helloworld.pas

```bash
# Building
fpc helloworld.pas 
# Running
./helloworld
```
