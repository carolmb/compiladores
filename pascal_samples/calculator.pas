program Calculator;

function sum(var n1, n2 : real) : real ;
begin
	sum := n1 + n2;
end;

function subtraction(var n1, n2 : real) : real ;
begin
	subtraction := n1 - n2;
end;

function multiplication(var n1, n2 : real) : real ;
begin
	multiplication := n1 * n2;
end;

function division(var n1 , n2 : real) : real ;
begin
	division := n1 / n2;
end;

function operation(var n1 : real; op : char; n2 : real) : real ;
begin
	case op of
     '+' : operation := sum(n1,n2);
     '-' : operation := subtraction(n1,n2);
     '*' : operation := multiplication(n1,n2);
     '/' : operation := division(n1,n2);
   end;
end;

var n1,n2 : Real;
var op : Char;

begin
    writeln ('Digite o primeiro valor (Real): ');
    read(n1);
    writeln ('Digite a operação (+, -, *, /): ');
    read(op);
    read(op);
    writeln ('Digite o segundo valor (Real): ');
    read(n2);
    writeln ('Resultado: ', n1:0:2, ' ', ' ', op,' ', n2:0:2, ' = ', operation(n1,op,n2):0:2);
end.