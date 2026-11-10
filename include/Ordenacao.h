#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <cmath>

struct ItemMetrica {
    int id;
    double valor;
};

struct CandidatoRecomendacao {
    int id;
    double pontuacao;
};

// Comparações de ordenação
bool compararItemMetrica(const ItemMetrica& a, const ItemMetrica& b);
bool compararCandidato(const CandidatoRecomendacao& a, const CandidatoRecomendacao& b);

// Algoritmos de ordenação para ItemMetrica (atendendo ao requisito dos 3 algoritmos da disciplina)
void insertionSortMetrica(ItemMetrica* arr, int n);
void quickSortMetrica(ItemMetrica* arr, int inicio, int fim);
void mergeSortMetrica(ItemMetrica* arr, int inicio, int fim);

// Algoritmo para ranking final de recomendação
void ordenarCandidatos(CandidatoRecomendacao* arr, int n);

#endif
