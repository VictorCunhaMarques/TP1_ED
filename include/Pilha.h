#ifndef PILHA_H
#define PILHA_H

class Pilha {
private:
    struct No {
        int id_item;
        No* proximo;

        No(int _id_item, No* _proximo = nullptr) : id_item(_id_item), proximo(_proximo) {}
    };

    No* topo;

public:
    Pilha();
    ~Pilha();
    void empilhar(int id_item);
    int desempilhar();
    bool vazia() const;
};

#endif