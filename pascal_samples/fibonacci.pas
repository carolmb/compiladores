program Fibonacci;

{ Fonte: http://wiki.freepascal.org/Fibonacci_number }
function fibonacciPos(const n: integer): integer;
begin
	{ Recursive case }
	if n > 1 then
		fibonacciPos := (fibonacciPos(n-1) + fibonacciPos(n-2))
	{ Base case }
	else if n = 0 then
		fibonacciPos := 0
	else
		fibonacciPos := 1;
end;

var in_number : integer;

begin
    writeln('Informe a posição da sequência de Fibonacci: ');
    read(in_number);
	writeln('Resultado: ', fibonacciPos(in_number));
end.