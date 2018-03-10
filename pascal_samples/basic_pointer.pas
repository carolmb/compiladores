program basic_pointer;

var
   x : integer;
   p, q : ^integer;
 
begin
   x := 10;
   p := @x;
 
   q := p;
   q^ := p^ + 10;
   writeln('Resultado: ', p^);
end.