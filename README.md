# RBG reasoners

Reasoner (silnik) danej gry to moduł, który implementuje reguły gry i udostępnia interfejs do obliczania legalnych ruchów, aplikowania ich, sprawdzania wyników. Zawarte w tym projekcie reasonery zostały zaimplementowane pod ogólnym interfejsem systemu General Game Playing o nazwie Regular Boardgames ([kompilator RBG](https://github.com/uicus/rbg2cpp)).

Wszystkie podane polecenia należy wywoływać w katalogu głównym projektu.

Żeby zbudować program do pomiaru wydajności należy uruchomić skrypt benchmark.sh z argumentem będącym nazwą gry, na przykład:
```
./benchmark.sh tictactoe
```
Jeżeli chcemy, aby dodatkowo przy pomiarze wydajności została zaaplikowana logika ,,monotonic'' należy uruchomić polecenie z argumentem ,,monotonic'':
```
./benchmark.sh tictactoe monotonic
```
Następnie w celu określenia podstawowych parametrów dotyczących implementacji gry takich jak liczba losowych gier na sekundę czy średnia głębokość gier, uruchamiamy komendę benchmark z argumentem będącym liczbą sekund. Liczba ta odnosi się do czasu w jakim program testowy będzie wykonywał losowe gry:
```
./benchmark 1000
```
Żeby zbudować program do testów należy uruchomić skrypt perft.sh z argumentem będącym nazwą gry, na przykład:
```
./perft.sh chess
```
Następnie, jeżeli chcemy policzyć wszystkie węzły liści drzewa gry na określonej głębokości, należy wywołać komendę perft z argumentem będącym głębokością dla której chcemy poznać wynik.
```
./perft 3
```
