#include "../include/Ordenacao.h"

// Arredondamento para 6 casas decimais exigido pela Seção 1.2
bool compararItemMetrica(const ItemMetrica& a, const ItemMetrica& b) {
    long long rA = std::llround(a.valor * 1000000.0);
    long long rB = std::llround(b.valor * 1000000.0);
    if (rA != rB) {
        return rA > rB; // Maior valor primeiro
    }
    return a.id < b.id; // Desempate por id crescente
}

// Desempate de recomendação final: maior pontuação primeiro, desempate por menor id
bool compararCandidato(const CandidatoRecomendacao& a, const CandidatoRecomendacao& b) {
    long long rA = std::llround(a.pontuacao * 100.0);
    long long rB = std::llround(b.pontuacao * 100.0);
    if (rA != rB) {
        return rA > rB;
    }
    return a.id < b.id;
}

// 1. Insertion Sort (Ótimo para dados quase ordenados)
void insertionSortMetrica(ItemMetrica* arr, int n) {
    for (int i = 1; i < n; i++) {
        ItemMetrica chave = arr[i];
        int j = i - 1;
        while (j >= 0 && compararItemMetrica(chave, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = chave;
    }
}

// 2. Quick Sort
static int particionar(ItemMetrica* arr, int inicio, int fim) {
    ItemMetrica pivo = arr[fim];
    int i = inicio - 1;
    for (int j = inicio; j < fim; j++) {
        if (compararItemMetrica(arr[j], pivo)) {
            i++;
            ItemMetrica temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    ItemMetrica temp = arr[i + 1];
    arr[i + 1] = arr[fim];
    arr[fim] = temp;
    return i + 1;
}

void quickSortMetrica(ItemMetrica* arr, int inicio, int fim) {
    if (inicio < fim) {
        int pi = particionar(arr, inicio, fim);
        quickSortMetrica(arr, inicio, pi - 1);
        quickSortMetrica(arr, pi + 1, fim);
    }
}

// 3. Merge Sort
static void intercalar(ItemMetrica* arr, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    ItemMetrica* L = new ItemMetrica[n1];
    ItemMetrica* R = new ItemMetrica[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[inicio + i];
    for (int j = 0; j < n2; j++) R[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
        if (compararItemMetrica(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }

    delete[] L;
    delete[] R;
}

void mergeSortMetrica(ItemMetrica* arr, int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        mergeSortMetrica(arr, inicio, meio);
        mergeSortMetrica(arr, meio + 1, fim);
        intercalar(arr, inicio, meio, fim);
    }
}

void ordenarCandidatos(CandidatoRecomendacao* arr, int n) {
    // Insertion sort para ranking de candidatos
    for (int i = 1; i < n; i++) {
        CandidatoRecomendacao chave = arr[i];
        int j = i - 1;
        while (j >= 0 && compararCandidato(chave, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = chave;
    }
}
