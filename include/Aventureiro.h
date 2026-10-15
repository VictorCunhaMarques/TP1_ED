#ifndef AVENTUREIRO_H
#define AVENTUREIRO_H
#include <string>
#include "Pilha.h"
#include "Atributo.h"

class Aventureiro {
private:
    static int contador_aventureiros;
    int id;
    std::string nome;
    Pilha build;
    int* vetor_modificadores;
    int num_modificadores;

public:
    Aventureiro(std::string& _nome);
    ~Aventureiro();
    int getId() const;
    const std::string& getNome() const;
    void equipar(int id_item, const int* modificadores_do_item, int tamanho_array_modificadores);
    int removerTopoDaBuild();
    void subtrairModificadores(const int* modificadores_do_item, int tamanho_array_modificadores);
};


#endif