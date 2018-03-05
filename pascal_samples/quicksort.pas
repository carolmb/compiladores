program quicksort;
uses SysUtils;

type
  Vetor = array of integer;

function quicksort(
        minha_lista : Vetor; n, ini_vet, fim_vet : integer) :
        Vetor ;
    
    var i, j, pivo, aux, id : integer;
    
begin
   i := ini_vet;
   j := fim_vet;
   
   id := Trunc((ini_vet + fim_vet) / 2);
   pivo := minha_lista[id];
        while i < j do begin
            while minha_lista[i] < pivo do begin
                i := i + 1;
            end;
            while minha_lista[j] > pivo do begin
                j := j - 1;
            end;
            if i < j then begin
                aux := minha_lista[i];
                minha_lista[i] := minha_lista[j];
                minha_lista[j] := aux;
                i := i + 1;
                j := j - 1;
            end;
        end;
        
        if ini_vet < j then begin
            quicksort := quicksort(minha_lista, n, ini_vet, j);
        end;
        
        if i < fim_vet then begin
            quicksort := quicksort(minha_lista, n, i, fim_vet);
        end;
end;

var i : integer;
var lista_ord : Vetor;
begin
    SetLength(lista_ord, 6);
    for i := 0 to 5 do begin
        lista_ord[i] := random(10);
        writeln(lista_ord[i]);
    end;
    lista_ord := quicksort(lista_ord, 6, 0, 5);
    
    for i := 0 to 6 do begin
        writeln(lista_ord[i]);
    end;
end.