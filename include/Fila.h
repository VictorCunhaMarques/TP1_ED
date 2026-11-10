#ifndef FILA_H
#define FILA_H

class Fila {
private:
    struct No {
        int id_item;
        int id_aventureiro;
        No* proximo;

        No(int _id_item, int _id_aventureiro, No* _proximo = nullptr) : id_item(_id_item), id_aventureiro(_id_aventureiro), proximo(_proximo) {}
    };
    No* primeiro;
    No* ultimo;

public:
    Fila();
    ~Fila();
    void enfileirar(int id_item, int id_aventureiro);
    bool desenfileirar(int& id_item, int& id_aventureiro);
    bool vazia() const;
    int tamanho() const;
};

#endif