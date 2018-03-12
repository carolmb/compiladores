# PasC

PasC is a compiled language, still under construction, aimed to teach structured programming for programmers immersed in the Portuguese language. It was developed based on the Pascal language, because it is a more robust language oriented to teaching, and C, due to the presence of structures of repetition better modeled.

### Dependencies

```bash
sudo apt install make
sudo apt install flex
```


## Running Pasc codes

### Build lex executable

```bash
cd lex
make
```

### Running lex
Execute by passing the input code as a parameter

```bash
cd lex
./lex_analyzer.o < codes/test
```

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
