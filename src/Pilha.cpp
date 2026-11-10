#include "../include/Pilha.h"

Pilha::Pilha() : topo(nullptr) {}
Pilha::~Pilha() {
    while(!vazia()) {
        desempilhar();
    }
}
void Pilha::empilhar(int id_item) {
    //O novo nó ja nasce apontando para o topo da pilha, e o topo é passado para o novo nó, fazendo com que ele vire o novo topo
    No* aux = new No(id_item, topo);
    topo = aux;
}
int Pilha::desempilhar() {
    if(!vazia()) {
        int id_retorno = topo->id_item;
        No* noAux = topo;

        topo = topo->proximo;
        delete noAux;

        return id_retorno;
    }
    return -1;
}
bool Pilha::vazia() const { return topo == nullptr; }
bool Pilha::contem(int id_item) const {
    No* aux = topo;
    while (aux != nullptr) {
        if (aux->id_item == id_item) {
            return true;
        }
        aux = aux->proximo;
    }
    return false;
}
int Pilha::tamanho() const {
    int count = 0;
    No* aux = topo;
    while (aux != nullptr) {
        count++;
        aux = aux->proximo;
    }
    return count;
}
void Pilha::obterItens(int* array_destino) const {
    int idx = 0;
    No* aux = topo;
    while (aux != nullptr) {
        array_destino[idx++] = aux->id_item;
        aux = aux->proximo;
    }
}