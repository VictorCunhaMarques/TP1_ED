#include "../include/Aventureiro.h"

int contador_aventureiros = 0;

Aventureiro::Aventureiro(std::string& _nome) : id(contador_aventureiros++), nome(_nome) {
    num_modificadores = Atributo::getTotalAtributos();
    vetor_modificadores = new int[num_modificadores];

    for (int i = 0; i < num_modificadores; i++) {
        vetor_modificadores[i] = 0;
    }
}
Aventureiro::~Aventureiro() { delete[] vetor_modificadores; }
int Aventureiro::getId() const { return id; }
const std::string& Aventureiro::getNome() const { return nome; }
void Aventureiro::equipar(int id_item, const int* modificadores_do_item, int tamanho_array_modificadores) {
    build.empilhar(id_item);
    if (tamanho_array_modificadores > num_modificadores) {
        int* vetor_aux = new int[tamanho_array_modificadores];
        int i = 0;
        while (i < num_modificadores) {
            vetor_aux[i] = vetor_modificadores[i];
            i++;
        }
        while (i < tamanho_array_modificadores) {
            vetor_aux[i] = 0;
            i++;
        }
        delete[] vetor_modificadores;
        vetor_modificadores = vetor_aux;
        num_modificadores = tamanho_array_modificadores;
    }
    int j = 0;
    while (j < tamanho_array_modificadores) {
        vetor_modificadores[j] += modificadores_do_item[j];
        j++;
    }
}
int Aventureiro::removerTopoDaBuild() { return build.desempilhar(); }
void Aventureiro::subtrairModificadores(const int* modificadores_do_item, int tamanho_array_modificadores) {
    int i = 0;
    while (i < tamanho_array_modificadores) {
        vetor_modificadores[i] -= modificadores_do_item[i];
        i++;
    }
}