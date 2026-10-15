#ifndef ATRIBUTO_H
#define ATRIBUTO_H
#include <string>

class Atributo {
private:
    static int contador_atributos;
    int id;
    std::string nome;

public:
    Atributo(const std::string& _nome);
    int getId() const;
    const std::string& getNome() const;
    static int getTotalAtributos();
};


#endif