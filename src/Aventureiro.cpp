#include "../include/Aventureiro.h"
#include "../include/Catalogo.h"

int Aventureiro::contador_aventureiros = 0;

Aventureiro::Aventureiro(const std::string& _nome) : id(contador_aventureiros++), nome(_nome) {
    num_modificadores = Atributo::getTotalAtributos();
    vetor_modificadores = new int[num_modificadores > 0 ? num_modificadores : 1];

    for (int i = 0; i < num_modificadores; i++) {
        vetor_modificadores[i] = 0;
    }
}
Aventureiro::~Aventureiro() { delete[] vetor_modificadores; }
int Aventureiro::getId() const { return id; }
const std::string& Aventureiro::getNome() const { return nome; }
void Aventureiro::empilharItem(int id_item) {
    build.empilhar(id_item);
}
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
    int limite = (tamanho_array_modificadores < num_modificadores) ? tamanho_array_modificadores : num_modificadores;
    int i = 0;
    while (i < limite) {
        vetor_modificadores[i] -= modificadores_do_item[i];
        i++;
    }
}
void Aventureiro::recalcularBuild(const Catalogo& catalogo, int total_atributos) {
    if (total_atributos > num_modificadores || vetor_modificadores == nullptr) {
        delete[] vetor_modificadores;
        num_modificadores = total_atributos;
        vetor_modificadores = new int[num_modificadores > 0 ? num_modificadores : 1];
    }
    for (int a = 0; a < total_atributos; a++) {
        vetor_modificadores[a] = 0;
    }
    int tam_pilha = build.tamanho();
    if (tam_pilha == 0) return;

    int* itens = new int[tam_pilha];
    build.obterItens(itens);
    for (int k = 0; k < tam_pilha; k++) {
        int id_it = itens[k];
        int* atribs = nullptr;
        int* vals = nullptr;
        int n = catalogo.obterModificadoresItem(id_it, atribs, vals);
        for (int m = 0; m < n; m++) {
            if (atribs[m] < total_atributos) {
                vetor_modificadores[atribs[m]] += vals[m];
            }
        }
        if (atribs) delete[] atribs;
        if (vals) delete[] vals;
    }
    delete[] itens;
}
const int* Aventureiro::getVetorModificadores() const { return vetor_modificadores; }
int Aventureiro::getNumModificadores() const { return num_modificadores; }
bool Aventureiro::possuiItemEquipado(int id_item) const { return build.contem(id_item); }
const Pilha& Aventureiro::getBuild() const { return build; }
int Aventureiro::getTotalAventureiros() { return contador_aventureiros; }