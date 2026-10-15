#ifndef ITEM_H
#define ITEM_H
#include <string>

class Item {
private:
    static int contador_itens;
    int id;
    std::string nome;

public:
    Item(const std::string& _nome);
    int getId() const;
    const std::string& getNome() const;
};


#endif