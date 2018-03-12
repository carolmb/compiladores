# PasC



## Dependencies

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

## Running Pascal codes

Running helloworld.pas

```bash
# Building
fpc helloworld.pas 
# Running
./helloworld
```
