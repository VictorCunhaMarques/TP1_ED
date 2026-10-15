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
        int idRetorno = topo->id_item;
        No* noAux = topo;

        topo = topo->proximo;
        delete noAux;

        return idRetorno;
    }
    return -1;
}
bool Pilha::vazia() const { return topo == nullptr; }