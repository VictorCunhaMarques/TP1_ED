#ifndef EXCECOES_H
#define EXCECOES_H

#include <exception>
#include <string>

class ExcecaoCustomizada : public std::exception {
protected:
    std::string mensagem;
public:
    ExcecaoCustomizada(const std::string& msg) : mensagem(msg) {}
    virtual const char* what() const noexcept override {
        return mensagem.c_str();
    }
};

class ExcecaoModoInvalido : public ExcecaoCustomizada {
public:
    ExcecaoModoInvalido(const std::string& modo)
        : ExcecaoCustomizada("Modo de armazenamento invalido: " + modo) {}
};

class ExcecaoIndiceInvalido : public ExcecaoCustomizada {
public:
    ExcecaoIndiceInvalido(const std::string& entidade, int id)
        : ExcecaoCustomizada("Indice invalido para " + entidade + ": " + std::to_string(id)) {}
};

class ExcecaoPilhaVazia : public ExcecaoCustomizada {
public:
    ExcecaoPilhaVazia() : ExcecaoCustomizada("Tentativa de desempilhar de uma pilha vazia.") {}
};

class ExcecaoFilaVazia : public ExcecaoCustomizada {
public:
    ExcecaoFilaVazia() : ExcecaoCustomizada("Tentativa de desenfileirar de uma fila vazia.") {}
};

#endif
