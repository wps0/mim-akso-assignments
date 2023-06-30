Specjalne kopiowanie plików

Zaimplementuj w asemblerze program `scopy`, który przyjmuje dwa parametry będące nazwami plików:

    ./scopy in_file out_file
    

Program sprawdza liczbę parametrów. Jeśli ich liczba jest różna od 2, to program kończy się kodem `1`.

Program próbuje otworzyć do odczytu plik `in_file`. Jeśli się nie udało, to program kończy się kodem `1`.

Program próbuje utworzyć do zapisu nowy plik `out_file` z uprawnieniami `-rw-r--r--`. Jeśli się nie udało, bo na przykład taki plik już istnieje, to program kończy się kodem `1`.

Program czyta z pliku `in_file` i pisze do pliku `out_file`. Jeśli wystąpił błąd odczytu lub zapisu, to program kończy się kodem `1`.

Dla każdego odczytanego z pliku `in_file` bajtu, którego wartość jest kodem ASCII litery `s` lub `S` zapisuje ten bajt do pliku `out_file`.

Dla każdego odczytanego z pliku `in_file` maksymalnego niepustego ciągu bajtów niezawierającego bajtu, którego wartość jest kodem ASCII litery `s` lub `S`, zapisuje do pliku `out_file` 16-bitową liczbę zawierającą liczbę bajtów w tym ciągu modulo 65536. Liczbę tę zapisuje binarnie w porządku cienkokońcówkowym (ang. _little-endian_).

Na koniec program zamyka pliki i jeśli wszystko się udało, kończy się kodem `0`.

Dodatkowe wymagania i wskazówki
-------------------------------

Program powinien korzystać z funkcji systemowych Linuksa: `sys_open`, `sys_read`, `sys_write`, `sys_close`, `sys_exit`. Program powinien sprawdzać poprawność wykonania funkcji systemowych (z wyjątkiem `sys_exit`). Jeśli wywołanie funkcji systemowej zakończy się błędem, to program powinien zakończyć się kodem `1`. W każdej sytuacji program powinien przed zakończeniem jawnie wywołać funkcję `sys_close` dla każdego pliku, który otworzył.

Dla uzyskania zadowalającej szybkości działania programu odczyty i zapisy należy buforować. Należy dobrać optymalne rozmiary buforów, a informację o tym umieścić w komentarzu.

Oddawanie rozwiązania
---------------------

Jako rozwiązanie należy wstawić w Moodle plik o nazwie `scopy.asm`.

Kompilowanie
------------

Rozwiązanie będzie kompilowane poleceniami:

    nasm -f elf64 -w+all -w+error -o scopy.o scopy.asm
    ld --fatal-warnings -o scopy scopy.o
    

Rozwiązanie musi się kompilować i działać w laboratorium komputerowym.

Przykład użycia
---------------

Przykład użycia znajduje się w niżej załączonych plikach `example1.in`, `example1.out`. Zawartość tych plików można obejrzeć poleceniem `hexdump -C`.

Ocenianie
---------

Ocena będzie się składała z dwóch części.

1.  Zgodność rozwiązania ze specyfikacją będzie oceniania za pomocą testów automatycznych, za które można otrzymać maksymalnie 7 punktów. W tym zadaniu priorytetem jest szybkość działania programu, ale rozmiar kodu też jest ważny. Zostanie ustalony próg rozmiaru sekcji `.text`. Przekroczenie tego progu będzie skutkowało odjęciem od oceny wartości proporcjonalnej do wartości tego przekroczenia. Za błędną nazwę pliku odejmiemy jeden punkt.
    
2.  Za jakość kodu i styl programowania można dostać maksymalnie 3 punkty. Tradycyjny styl programowania w asemblerze polega na rozpoczynaniu etykiet od pierwszej kolumny, a mnemoników rozkazów od wybranej ustalonej kolumny. Nie stosuje się innych wcięć. Taki styl mają przykłady pokazywane na zajęciach. Kod powinien być dobrze skomentowany, co oznacza między innymi, że każdy blok kodu powinien być opatrzony informacją, co robi. Należy opisać przeznaczenie rejestrów. Komentarza wymagają wszystkie kluczowe lub nietrywialne linie kodu. W przypadku asemblera nie jest przesadą komentowanie prawie każdej linii kodu, ale należy unikać komentarzy opisujących to, co widać.
    

Wystawienie oceny może zostać uzależnione od osobistego wyjaśnienia szczegółów działania programu prowadzącemu zajęcia.

**Rozwiązania należy implementować samodzielnie pod rygorem niezaliczenia przedmiotu. Zarówno korzystanie z cudzego kodu, jak i prywatne lub publiczne udostępnianie własnego kodu jest zabronione.**
