Sumowanie
---------

Zaimplementuj w asemblerze wołaną z języka C funkcję o następującej deklaracji:

    void sum(int64_t *x, size_t n);
    

Argumentami tej funkcji są wskaźnik `x` na niepustą tablicę 64-bitowych liczb całkowitych w reprezentacji uzupełnieniowej do dwójki oraz rozmiar `n` tej tablicy. Działanie funkcji zdefiniowane jest przez poniższy pseudokod:

    y = 0;
    for (i = 0; i < n; ++i)
      y += x[i] * (2 ** floor(64 * i * i / n));
    x[0, ..., n-1] = y;
    

gdzie `**` oznacza potęgowanie, a `y` jest `(64 * n)`\-bitową liczbą w reprezentacji uzupełnieniowej do dwójki. Funkcja powinna umieścić wynik w tablicy `x` w porządku cienkokońcówkowym (ang. _little-endian_). Funkcja powinna wykonać obliczenie „w miejscu”, korzystając tylko z pamięci wskazywanej przez `x`, i nie powinna używać dodatkowej pamięci.

Wolno założyć, że wskaźnik `x` jest poprawny oraz że `n` ma wartość dodatnią mniejszą niż 2 do potęgi 29.

Oddawanie rozwiązania
---------------------

Jako rozwiązanie należy wstawić w Moodle plik o nazwie `sum.asm`.

Kompilowanie
------------

Rozwiązanie będzie kompilowane poleceniem:

    nasm -f elf64 -w+all -w+error -o sum.o sum.asm
    

Rozwiązanie musi się kompilować w laboratorium komputerowym.

Przykład użycia
---------------

Przykład użycia znajduje się w niżej załączonym pliku `sum_example.c`. Można go skompilować i skonsolidować z rozwiązaniem poleceniami:

    gcc -c -Wall -Wextra -std=c17 -O2 -o sum_example.o sum_example.c
    gcc -z noexecstack -o sum_example sum_example.o sum.o
    

Ocenianie
---------

Ocena będzie się składała z dwóch części.

1.  Zgodność rozwiązania ze specyfikacją będzie oceniania za pomocą testów automatycznych, za które można dostać maksymalnie 7 punktów. Sprawdzane będą też przestrzeganie reguł ABI, poprawność odwołań do pamięci i zajętość pamięci. Od wyniku testów automatycznych zostanie odjęta wartość proporcjonalna do rozmiaru dodatkowej pamięci wykorzystywanej przez rozwiązanie (sekcje `.bss`, `.data`, `.rodata`, stos, sterta). Ponadto zostanie ustalony próg rozmiaru sekcji `.text`. Przekroczenie tego progu będzie skutkowało odjęciem od oceny wartości proporcjonalnej do wartości tego przekroczenia. Dodatkowym kryterium automatycznej oceny rozwiązania będzie szybkość jego działania. Rozwiązanie zbyt wolne nie uzyska maksymalnej oceny. Za błędną nazwę pliku odejmiemy jeden punkt.
    
2.  Za jakość kodu i styl programowania można dostać maksymalnie 3 punkty. Tradycyjny styl programowania w asemblerze polega na rozpoczynaniu etykiet od pierwszej kolumny, a mnemoników rozkazów od wybranej ustalonej kolumny. Nie stosuje się innych wcięć. Taki styl mają przykłady pokazywane na zajęciach. Kod powinien być dobrze skomentowany, co oznacza między innymi, że każdy blok kodu powinien być opatrzony informacją, co robi. Należy opisać przeznaczenie rejestrów. Komentarza wymagają wszystkie kluczowe lub nietrywialne linie kodu. W przypadku asemblera nie jest przesadą komentowanie prawie każdej linii kodu, ale należy unikać komentarzy opisujących to, co widać.
    

Wystawienie oceny może zostać uzależnione od osobistego wyjaśnienia szczegółów działania programu prowadzącemu zajęcia.

**Rozwiązania należy implementować samodzielnie pod rygorem niezaliczenia przedmiotu. Zarówno korzystanie z cudzego kodu, jak i prywatne lub publiczne udostępnianie własnego kodu jest zabronione.**
