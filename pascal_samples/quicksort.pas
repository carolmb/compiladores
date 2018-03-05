program quicksort;

type Vetor = array of integer;

procedure troca(var c1, c2 : integer);
var c:integer; 
begin
	c := c1;
	c1 := c2;
	c2 := c;
end;

function particiona(var minha_lista : Vetor; ini_vet, fim_vet : integer) : integer ;
var i, j, pivo : integer;
begin
	pivo := minha_lista[fim_vet];
	i := ini_vet-1;
	for j := ini_vet to fim_vet-1 do begin
		if minha_lista[j] < pivo then begin
			i := i+1;
			troca(minha_lista[i], minha_lista[j]);
		end;
	end;
	if minha_lista[fim_vet] < minha_lista[i+1] then begin
		troca(minha_lista[fim_vet], minha_lista[i+1]);
	end;

	particiona := i+1;
end;


procedure quicksort(var minha_lista : Vetor; ini_vet, fim_vet : integer) ;
var pivo : integer;
begin
	if ini_vet < fim_vet then begin
		pivo := particiona(minha_lista, ini_vet, fim_vet);
		quicksort(minha_lista, ini_vet, pivo-1);
		quicksort(minha_lista, pivo+1, fim_vet);		
	end;
end;

var i, len : integer;
var lista_ord : Vetor;
begin
	writeln('Insina tamanho do vetor');
	read(len);
	SetLength(lista_ord, len);
	for i := 0 to len-1 do begin
        read(lista_ord[i]);
    end;

    quicksort(lista_ord, 0, len-1);

    {WRITE}
    writeln('Lista ordenada:');
    for i := 0 to len-1 do begin
        write(lista_ord[i], ' ');
    end;
end.
