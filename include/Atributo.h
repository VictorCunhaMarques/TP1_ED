#ifndef ATRIBUTO_H
#define ATRIBUTO_H
#include <string>

class Atributo {
private:
    int id;
    std::string nome;

public:
    Atributo(int _id, const std::string& _nome);
    int getId() const;
    const std::string& getNome() const;
};


#endif