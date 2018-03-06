program mergesort;

type Vetor = array of integer;

procedure merge(var minha_lista : Vetor; ini_vet, pivo, fim_vet : integer) ;
var n1, n2, i, j, k : integer;
var esquerda, direita : Vetor;
begin
	n1 := pivo - ini_vet + 1;
	n2 := fim_vet - pivo;

	SetLength(esquerda, n1);
	SetLength(direita, n2);
	for i := 0 to n1-1 do begin
		esquerda[i] := minha_lista[ini_vet+i];
	end;

	for i := 0 to n2-1 do begin
		direita[i] := minha_lista[pivo+i+1];
	end;

	i := 0;
	j := 0;

	for k := ini_vet to fim_vet do begin
		if i >= n1 then begin
			minha_lista[k] := direita[j];
			j := j+1;
			continue;
		end;

		if j >= n2 then begin
			minha_lista[k] := esquerda[i];
			i := i+1;
			continue;
		end;

		if (esquerda[i] <= direita[j]) and (i < n1) and (j < n2) then begin
			minha_lista[k] := esquerda[i];
			i := i+1;
		end else begin
			minha_lista[k] := direita[j];
			j := j+1;
		end;
	end;
end;

procedure mergesort(var minha_lista : Vetor; ini_vet, fim_vet : integer) ;
var pivo : integer;
begin
	if ini_vet < fim_vet then begin
		pivo := Trunc((ini_vet + fim_vet)/2);
		mergesort(minha_lista, ini_vet, pivo);
		mergesort(minha_lista, pivo+1, fim_vet);
		merge(minha_lista, ini_vet, pivo, fim_vet);		
	end;
end;

var i, len : integer;
var lista_ord : Vetor;
begin
	writeln('Insira tamanho do vetor:');
	read(len);
	SetLength(lista_ord, len);
	for i := 0 to len-1 do begin
        read(lista_ord[i]);
    end;

    mergesort(lista_ord, 0, len-1);

    writeln('Lista ordenada:');
    for i := 0 to len-1 do begin
        write(lista_ord[i], ' ');
    end;
end.