#include <iostream>
#include <climits>
using namespace std;

const int DISC_LIMIT = 50;
const int PEG_LIMIT = 10;

// Tablica przechowująca minimalną liczbę przesunięć
long long moveCount[DISC_LIMIT + 1][PEG_LIMIT + 1];
// Tablica przechowująca optymalny punkt podziału
int splitPoint[DISC_LIMIT + 1][PEG_LIMIT + 1];

// Obliczanie minimalnej liczby ruchów dla wszystkich kombinacji
void calculateMinMoves(int maxDiscs, int maxPegs) {
    // Inicjalizacja wszystkich wartości zerami
    for (int d = 0; d <= maxDiscs; d++) {
        for (int p = 0; p <= maxPegs; p++) {
            moveCount[d][p] = 0;
            splitPoint[d][p] = 0;
        }
    }
    
    // Przypadek bazowy: 3 paliki (klasyczny problem Hanoi)
    // Wzór: 2^d - 1
    long long powerOfTwo = 1;
    for (int d = 0; d <= maxDiscs; d++) {
        if (d == 0) {
            moveCount[d][3] = 0;
        } else {
            moveCount[d][3] = powerOfTwo - 1;
        }
        powerOfTwo *= 2;
    }
    
    // Obliczenia dla 4 i więcej palików (algorytm Frame-Stewart)
    for (int p = 4; p <= maxPegs; p++) {
        for (int d = 1; d <= maxDiscs; d++) {
            if (d == 1) {
                moveCount[d][p] = 1;
                splitPoint[d][p] = 1;
                continue;
            }
            
            // Poszukiwanie optymalnego podziału
            long long bestResult = LLONG_MAX;
            int optimalSplit = 1;
            
            for (int split = 1; split < d; split++) {
                // Formuła rekurencyjna: 2*M(split,p) + M(d-split,p-1)
                long long currentResult = 2 * moveCount[split][p] + moveCount[d - split][p - 1];
                if (currentResult < bestResult) {
                    bestResult = currentResult;
                    optimalSplit = split;
                }
            }
            
            moveCount[d][p] = bestResult;
            splitPoint[d][p] = optimalSplit;
        }
    }
}

// Klasyczna rekurencyjna implementacja dla 3 palików
void solveClassic(int numDiscs, int from, int to, int temp) {
    if (numDiscs <= 0) return;
    
    solveClassic(numDiscs - 1, from, temp, to);
    cout << from << " to " << to << "\n";
    solveClassic(numDiscs - 1, temp, to, from);
}

// Rozwiązanie iteracyjne dla 3 palików
void solveIterative(int numDiscs, int source, int destination, int auxiliary) {
    if (numDiscs <= 0) return;
    
    // Tablica do śledzenia liczby dysków na każdym paliku
    int height[3] = {numDiscs, 0, 0};
    // Tablica do przechowywania dysków na każdym paliku
    int pegDiscs[3][51];
    
    // Konfiguracja początkowa - wszystkie dyski na pierwszym paliku
    for (int i = 0; i < numDiscs; i++) {
        pegDiscs[0][i] = numDiscs - i;
    }
    for (int i = 1; i < 3; i++) {
        for (int j = 0; j < 51; j++) {
            pegDiscs[i][j] = 0;
        }
    }
    
    // Określenie sekwencji ruchu najmniejszego dysku
    // Dla parzystej liczby: 0->1->2->0 (zgodnie z ruchem wskazówek zegara)
    // Dla nieparzystej liczby: 0->2->1->0 (przeciwnie do ruchu wskazówek zegara)
    int sequence[3];
    if (numDiscs % 2 == 0) {
        sequence[0] = 0;
        sequence[1] = 1;
        sequence[2] = 2;
    } else {
        sequence[0] = 0;
        sequence[1] = 2;
        sequence[2] = 1;
    }
    
    // Mapowanie indeksów na rzeczywiste numery palików
    int pegNumbers[3] = {source, auxiliary, destination};
    
    // Bieżąca pozycja najmniejszego dysku
    int smallestPosition = 0;
    
    // Całkowita liczba ruchów
    long long totalMoves = (1LL << numDiscs) - 1;
    
    for (long long moveNumber = 1; moveNumber <= totalMoves; moveNumber++) {
        if (moveNumber % 2 == 1) {
            // Ruch nieparzysty: przesuwamy najmniejszy dysk
            int currentPeg = sequence[smallestPosition];
            int nextPosition = (smallestPosition + 1) % 3;
            int nextPeg = sequence[nextPosition];
            
            // Wykonanie przesunięcia
            int disc = pegDiscs[currentPeg][height[currentPeg] - 1];
            height[currentPeg]--;
            pegDiscs[nextPeg][height[nextPeg]] = disc;
            height[nextPeg]++;
            
            cout << pegNumbers[currentPeg] << " to " << pegNumbers[nextPeg] << "\n";
            
            smallestPosition = nextPosition;
        } else {
            // Ruch parzysty: jedyny możliwy ruch między dwoma innymi palikami
            int firstPeg = -1, secondPeg = -1;
            for (int i = 0; i < 3; i++) {
                if (sequence[i] != sequence[smallestPosition]) {
                    if (firstPeg == -1) {
                        firstPeg = sequence[i];
                    } else {
                        secondPeg = sequence[i];
                    }
                }
            }
            
            // Określenie kierunku przesunięcia
            int fromPeg, toPeg;
            if (height[firstPeg] == 0) {
                fromPeg = secondPeg;
                toPeg = firstPeg;
            } else if (height[secondPeg] == 0) {
                fromPeg = firstPeg;
                toPeg = secondPeg;
            } else {
                // Oba paliki mają dyski - przesuwamy mniejszy na większy
                int topFirst = pegDiscs[firstPeg][height[firstPeg] - 1];
                int topSecond = pegDiscs[secondPeg][height[secondPeg] - 1];
                if (topFirst < topSecond) {
                    fromPeg = firstPeg;
                    toPeg = secondPeg;
                } else {
                    fromPeg = secondPeg;
                    toPeg = firstPeg;
                }
            }
            
            // Wykonanie przesunięcia
            int disc = pegDiscs[fromPeg][height[fromPeg] - 1];
            height[fromPeg]--;
            pegDiscs[toPeg][height[toPeg]] = disc;
            height[toPeg]++;
            
            cout << pegNumbers[fromPeg] << " to " << pegNumbers[toPeg] << "\n";
        }
    }
}

// Rozwiązanie rekurencyjne Frame-Stewart dla wielu palików
void solveMultiPeg(int discs, int numPegs, int rods[]) {
    if (discs == 0) return;
    
    // Jeśli zostały 3 paliki - używamy klasycznego algorytmu
    if (numPegs == 3) {
        solveClassic(discs, rods[0], rods[2], rods[1]);
        return;
    }
    
    // Przypadek bazowy: jeden dysk
    if (discs == 1) {
        cout << rods[0] << " to " << rods[numPegs - 1] << "\n";
        return;
    }
    
    // Pobranie optymalnego podziału
    int split = splitPoint[discs][numPegs];
    int startRod = rods[0];
    int endRod = rods[numPegs - 1];
    int intermediateRod = rods[1];
    
    // Etap 1: przesunięcie górnych 'split' dysków na palik pośredni
    int firstConfig[PEG_LIMIT];
    int firstSize = 0;
    firstConfig[firstSize++] = startRod;
    for (int i = 1; i < numPegs; i++) {
        if (rods[i] != startRod) {
            firstConfig[firstSize++] = rods[i];
        }
    }
    // Przestawienie: palik pośredni staje się końcowym
    for (int i = 0; i < firstSize; i++) {
        if (firstConfig[i] == intermediateRod) {
            int swap = firstConfig[i];
            firstConfig[i] = firstConfig[firstSize - 1];
            firstConfig[firstSize - 1] = swap;
            break;
        }
    }
    solveMultiPeg(split, firstSize, firstConfig);
    
    // Etap 2: przesunięcie pozostałych dysków na palik końcowy (bez pośredniego)
    int secondConfig[PEG_LIMIT];
    int secondSize = 0;
    for (int i = 0; i < numPegs; i++) {
        if (rods[i] != intermediateRod) {
            secondConfig[secondSize++] = rods[i];
        }
    }
    solveMultiPeg(discs - split, secondSize, secondConfig);
    
    // Etap 3: przesunięcie 'split' dysków z pośredniego na palik końcowy
    int thirdConfig[PEG_LIMIT];
    int thirdSize = 0;
    thirdConfig[thirdSize++] = intermediateRod;
    for (int i = 0; i < numPegs; i++) {
        if (rods[i] != intermediateRod) {
            thirdConfig[thirdSize++] = rods[i];
        }
    }
    // Przestawienie: palik końcowy staje się ostatnim
    for (int i = 0; i < thirdSize; i++) {
        if (thirdConfig[i] == endRod) {
            int swap = thirdConfig[i];
            thirdConfig[i] = thirdConfig[thirdSize - 1];
            thirdConfig[thirdSize - 1] = swap;
            break;
        }
    }
    solveMultiPeg(split, thirdSize, thirdConfig);
}

int main() {
    int numPegs, numDiscs;
    
    cerr << "Podaj liczbę palików (od 3 do 10): ";
    cin >> numPegs;
    
    cerr << "Podaj liczbę dysków: ";
    cin >> numDiscs;
    
    if (numPegs < 3 || numPegs > 10 || numDiscs < 0 || numDiscs > 50) {
        cerr << "Nieprawidłowe dane.\n";
        return 1;
    }
    
    // Wyświetlenie parametrów
    cout << numPegs << "\n" << numDiscs << "\n";
    
    if (numPegs == 3) {
        // Wybór metody dla 3 palików
        cerr << "Wybierz metodę:\n";
        cerr << "1 - Rekurencyjna\n";
        cerr << "2 - Iteracyjna\n";
        cerr << "Twój wybór: ";
        int method;
        cin >> method;
        
        if (method == 2) {
            solveIterative(numDiscs, 1, 3, 2);
        } else {
            solveClassic(numDiscs, 1, 3, 2);
        }
    } else {
        // Dla 4+ palików używamy algorytmu Frame-Stewart
        calculateMinMoves(numDiscs, numPegs);
        
        int rodArray[PEG_LIMIT];
        for (int i = 0; i < numPegs; i++) {
            rodArray[i] = i + 1;
        }
        
        solveMultiPeg(numDiscs, numPegs, rodArray);
    }
    
    return 0;
}
