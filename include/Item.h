#ifndef ITEM_H
#define ITEM_H
#include <string>

class Item {
private:
    int id;
    std::string nome;

public:
    Item(int _id, const std::string& _nome);
    int getId() const;
    const std::string& getNome() const;
};


#endif