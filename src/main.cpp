#include <iostream>
#include <string>
#include <iomanip>

#include "../include/Atributo.h"
#include "../include/Item.h"
#include "../include/Aventureiro.h"
#include "../include/Pilha.h"
#include "../include/Fila.h"
#include "../include/Catalogo.h"
#include "../include/Vetor.h"
#include "../include/Ordenacao.h"
#include "../include/Excecoes.h"

struct EncomendaAtendida {
    int id_aventureiro;
    int id_item;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    Catalogo catalogo('L');
    Fila fila_encomendas;

    Vetor<Atributo*> atributos;
    Vetor<Item*> itens;
    Vetor<Aventureiro*> aventureiros;

    try {
        std::string comando;
        while (std::cin >> comando) {
        if (comando == "E") {
            char modo;
            std::cin >> modo;
            catalogo.setModo(modo);
            std::cout << "E " << modo << "\n";
        } else if (comando == "A") {
            std::string nome;
            std::cin >> nome;
            Atributo* novo = new Atributo(nome);
            atributos.adicionar(novo);
            std::cout << "A " << novo->getId() << "\n";
        } else if (comando == "I") {
            std::string nome;
            std::cin >> nome;
            Item* novo = new Item(nome);
            itens.adicionar(novo);
            std::cout << "I " << novo->getId() << "\n";
        } else if (comando == "H") {
            std::string nome;
            std::cin >> nome;
            Aventureiro* novo = new Aventureiro(nome);
            aventureiros.adicionar(novo);
            std::cout << "H " << novo->getId() << "\n";
        } else if (comando == "M") {
            int id_item, id_atributo, valor;
            std::cin >> id_item >> id_atributo >> valor;
            catalogo.alterarModificador(id_item, id_atributo, valor);
            std::cout << "M " << itens[id_item]->getNome() << " "
                      << atributos[id_atributo]->getNome() << " " << valor << "\n";
        } else if (comando == "F") {
            int id_aventureiro, id_item;
            std::cin >> id_aventureiro >> id_item;
            fila_encomendas.enfileirar(id_item, id_aventureiro);
            std::cout << "F " << fila_encomendas.tamanho() << "\n";
        } else if (comando == "Z") {
            int k;
            std::cin >> k;
            int forjadas = 0;
            Vetor<EncomendaAtendida> atendidas;

            while (forjadas < k && !fila_encomendas.vazia()) {
                int id_item = -1, id_aventureiro = -1;
                fila_encomendas.desenfileirar(id_item, id_aventureiro);
                aventureiros[id_aventureiro]->empilharItem(id_item);

                EncomendaAtendida enc;
                enc.id_aventureiro = id_aventureiro;
                enc.id_item = id_item;
                atendidas.adicionar(enc);
                forjadas++;
            }

            std::cout << "Z " << forjadas << "\n";
            if (forjadas > 0) {
                for (int i = 0; i < atendidas.tamanho(); i++) {
                    if (i > 0) std::cout << " ";
                    std::cout << "encomenda_" << (i + 1) << " "
                              << atendidas[i].id_aventureiro << " " << atendidas[i].id_item;
                }
                std::cout << "\n";
            }
        } else if (comando == "D") {
            int id_aventureiro;
            std::cin >> id_aventureiro;
            Aventureiro* av = aventureiros[id_aventureiro];
            int id_desequipado = av->removerTopoDaBuild();
            if (id_desequipado == -1) {
                std::cout << "D " << av->getNome() << " VAZIO\n";
            } else {
                std::cout << "D " << av->getNome() << " " << itens[id_desequipado]->getNome() << "\n";
            }
        } else if (comando == "LI") {
            int id_item;
            std::cin >> id_item;
            int* atribs = nullptr;
            int* vals = nullptr;
            int n = catalogo.obterModificadoresItem(id_item, atribs, vals);
            if (n == 0) {
                std::cout << "LI " << itens[id_item]->getNome() << " VAZIO\n";
            } else {
                std::cout << "LI " << itens[id_item]->getNome();
                for (int i = 0; i < n; i++) {
                    std::cout << " atributo_" << (i + 1) << " " << atribs[i] << " " << vals[i];
                }
                std::cout << "\n";
            }
            if (atribs) delete[] atribs;
            if (vals) delete[] vals;
        } else if (comando == "LA") {
            int id_atributo;
            std::cin >> id_atributo;
            int* itens_arr = nullptr;
            int* vals = nullptr;
            int n = catalogo.obterItensAtributo(id_atributo, itens_arr, vals);
            if (n == 0) {
                std::cout << "LA " << atributos[id_atributo]->getNome() << " VAZIO\n";
            } else {
                std::cout << "LA " << atributos[id_atributo]->getNome();
                for (int i = 0; i < n; i++) {
                    std::cout << " item_" << (i + 1) << " " << itens_arr[i] << " " << vals[i];
                }
                std::cout << "\n";
            }
            if (itens_arr) delete[] itens_arr;
            if (vals) delete[] vals;
        } else if (comando == "LH") {
            int id_aventureiro;
            std::cin >> id_aventureiro;
            Aventureiro* av = aventureiros[id_aventureiro];
            int total_atrib = atributos.tamanho();
            av->recalcularBuild(catalogo, total_atrib);
            const int* build_vec = av->getVetorModificadores();

            int count_nao_nulos = 0;
            for (int a = 0; a < total_atrib; a++) {
                if (build_vec[a] != 0) count_nao_nulos++;
            }

            if (count_nao_nulos == 0) {
                std::cout << "LH " << av->getNome() << " VAZIO\n";
            } else {
                std::cout << "LH " << av->getNome();
                int idx_label = 1;
                for (int a = 0; a < total_atrib; a++) {
                    if (build_vec[a] != 0) {
                        std::cout << " atributo_" << idx_label << " " << a << " " << build_vec[a];
                        idx_label++;
                    }
                }
                std::cout << "\n";
            }
        } else if (comando == "Q") {
            int id_aventureiro, n, m;
            std::cin >> id_aventureiro >> n >> m;

            Aventureiro* av = aventureiros[id_aventureiro];
            int total_atrib = atributos.tamanho();
            int total_itens = itens.tamanho();

            av->recalcularBuild(catalogo, total_atrib);
            const int* build_vec = av->getVetorModificadores();

            double* pontuacao_acumulada = new double[total_itens];
            for (int i = 0; i < total_itens; i++) {
                pontuacao_acumulada[i] = 0.0;
            }

            for (int j = 0; j < m; j++) {
                std::string nome_metrica;
                double peso;
                std::cin >> nome_metrica >> peso;

                ItemMetrica* itens_metrica = new ItemMetrica[total_itens];
                for (int i = 0; i < total_itens; i++) {
                    itens_metrica[i].id = i;
                    if (nome_metrica == "PODER") {
                        itens_metrica[i].valor = catalogo.calcularPoder(i);
                    } else if (nome_metrica == "EQUILIBRIO") {
                        itens_metrica[i].valor = catalogo.calcularEquilibrio(i, total_atrib);
                    } else if (nome_metrica == "SINERGIA") {
                        itens_metrica[i].valor = catalogo.calcularSinergia(i, build_vec, total_atrib);
                    }
                }

                // Ordenar por métrica
                quickSortMetrica(itens_metrica, 0, total_itens - 1);

                // Pontuação por posição
                for (int pos = 0; pos < total_itens; pos++) {
                    int id_it = itens_metrica[pos].id;
                    double pontos = (double)(total_itens - pos) * peso;
                    pontuacao_acumulada[id_it] += pontos;
                }

                delete[] itens_metrica;
            }

            // Descartar itens que o aventureiro atualmente possui equipados
            int count_candidatos = 0;
            for (int i = 0; i < total_itens; i++) {
                if (!av->possuiItemEquipado(i)) {
                    count_candidatos++;
                }
            }

            if (count_candidatos > 0) {
                CandidatoRecomendacao* candidatos = new CandidatoRecomendacao[count_candidatos];
                int c_idx = 0;
                for (int i = 0; i < total_itens; i++) {
                    if (!av->possuiItemEquipado(i)) {
                        candidatos[c_idx].id = i;
                        candidatos[c_idx].pontuacao = pontuacao_acumulada[i];
                        c_idx++;
                    }
                }

                ordenarCandidatos(candidatos, count_candidatos);

                int qtd_listar = (n < count_candidatos) ? n : count_candidatos;
                for (int pos = 0; pos < qtd_listar; pos++) {
                    std::cout << "Q " << (pos + 1) << " " << candidatos[pos].id << " "
                              << std::fixed << std::setprecision(2) << candidatos[pos].pontuacao << "\n";
                }

                delete[] candidatos;
            }

            delete[] pontuacao_acumulada;
        } else if (comando == "LF") {
            int id_atributo, valor_min, valor_max;
            std::cin >> id_atributo >> valor_min >> valor_max;
            int* itens_arr = nullptr;
            int* vals = nullptr;
            int n = catalogo.consultarFaixa(id_atributo, valor_min, valor_max, itens.tamanho(), itens_arr, vals);
            if (n == 0) {
                std::cout << "LF " << atributos[id_atributo]->getNome() << " VAZIO\n";
            } else {
                std::cout << "LF " << atributos[id_atributo]->getNome();
                for (int i = 0; i < n; i++) {
                    std::cout << " item_" << (i + 1) << " " << itens_arr[i] << " " << vals[i];
                }
                std::cout << "\n";
            }
            if (itens_arr) delete[] itens_arr;
            if (vals) delete[] vals;
        }
    }
    } catch (const ExcecaoCustomizada& e) {
        std::cerr << "Erro customizado: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Excecao padrao: " << e.what() << "\n";
    }

    // Liberação de memória dos TADs
    for (int i = 0; i < atributos.tamanho(); i++) delete atributos[i];
    for (int i = 0; i < itens.tamanho(); i++) delete itens[i];
    for (int i = 0; i < aventureiros.tamanho(); i++) delete aventureiros[i];

    return 0;
}
