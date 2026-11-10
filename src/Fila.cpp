#include "../include/Fila.h"

Fila::Fila() : primeiro(nullptr), ultimo(nullptr) {}
Fila::~Fila() {
    No* aux = primeiro;
    while (primeiro != nullptr) {
        aux = primeiro;
        primeiro = primeiro->proximo;
        delete aux;
    }
}
void Fila::enfileirar(int id_item, int id_aventureiro) {
    No* novo_no = new No(id_item, id_aventureiro);

    if (vazia()) {
        primeiro = novo_no;
    } else {
        ultimo->proximo = novo_no;
    }
    ultimo = novo_no;
}
bool Fila::desenfileirar(int& id_item, int& id_aventureiro) {
    if (vazia()) { return false; }

    id_item = primeiro->id_item;
    id_aventureiro = primeiro->id_aventureiro;
    No* aux = primeiro;
    primeiro = primeiro->proximo;
    delete aux;
    if (vazia()) {
        ultimo = nullptr;
    }
    return true;
}
bool Fila::vazia() const { return primeiro == nullptr; }
int Fila::tamanho() const {
    int count = 0;
    No* aux = primeiro;
    while (aux != nullptr) {
        count++;
        aux = aux->proximo;
    }
    return count;
}