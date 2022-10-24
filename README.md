# projekt-z-programowania-obiektowego-cpp

## Symulacja gry w "Biegające kulki"  
Gra toczy się na prostokącie n*k. Każde z pól prostokąta może być puste lub zawierać jeden Odbijacz. Po planszy poruszają się kulki.
Każda kulka znajduje się naraz na jednym polu i ma szybkość pionową lub poziomą lub ukośną (pod
kątem 45 stopni). Kulki, pojawiając się na polach, mogą odbijać się od znajdujących się tam Odbijaczy lub -
w przypadku pustych pól - kontynuować swój ruch. Kulki są wrzucane na planszę z jej boków na początku
symulacji i mogą one wypadać poza planszę.

### Rodzaje odbijaczy
- Ukośny: jest ustawiony pod kątem 45 stopni. Oznaczam przez "U" odbijacz "/" , a przez "u" odbijacz "\"
- Losowy: losowo zmienia kierunek kulki
- Zjadający: usuwa kulkę
- Spowalniający: zmniejsza szybkość kulki o połowę
- Mgłowy - nic nie robi
- Przyspieszający: zwiększa szybkość kulki dwa razy

### Rodzaje kulek
- Zwykła - porusza się zgodnie z regułami
- Taran: usuwa wszystko na swojej drodze
- Wybuchowa: po określonej przez gracza liczbie tur niszczy odbijacz, na który wpada

### Uwagi techniczne
- Symulacja odbywa się w turach. Kulka o szybkości 1 w jednej turze przesuwa się (zgodnie ze
swoim kierunkiem) o jedno pole
- Plik danych jest plikiem tekstowym
- Pierwszy wiersz zawiera 3 liczby oddzielone spacjami: liczbę tur (k), szerokość (m) i
wysokość (n) planszy
- Następnie są n+2 wiersze, każdy z m+2 znakami opisującymi początkową zawartość planszy
- Skrajne wiersze i kolumny opisują kulki które mają się pojawić na planszy
- Początkowo kulki mają szybkość 1, w kierunku wynikającym z ich położenia względem planszy (kulki narożne poruszają się pod kątem 45
stopni, kulki na bokach poruszają się prostopadle do boku, na którym się znajdują w kierunku przeciwległego boku)
- Odbijacze i kulki są zaznaczone pierwszymi literami nazw
- Puste miejsca reprezentowane są spacjami
