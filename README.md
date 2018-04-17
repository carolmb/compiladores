# PasC

PasC is a compiled language, still under construction, aimed to teach structured programming for programmers immersed in the Portuguese language. It was developed based on the Pascal language, because it is a more robust language oriented to teaching, and C, due to the presence of structures of repetition better modeled.

### Dependencies

```bash
sudo apt install make
sudo apt install flex
```


## Running Pasc codes

### Build compiler executable

```bash
make
```

### Running compiler
Execute by passing the input code as a parameter

```bash
./compiler.o codes/test -syntaxparser
```
#### Where:

```test_code``` is the name of the PasC code that you want to run.

```-syntaxparser``` is the choosen parser that you want to use. Can be ```-rec``` for the predictive recursive or ```-table``` for the predictive table.

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
