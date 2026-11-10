#include "../include/Catalogo.h"
#include <cmath>
#include <cstring>

Catalogo::Catalogo(char modo_inicial)
    : modo(modo_inicial), total_celulas(0),
      cabecas_linhas(nullptr), cabecas_colunas(nullptr),
      cap_linhas(0), cap_colunas(0),
      tripletas(nullptr), cap_tripletas(0),
      csr_valores(nullptr), csr_colunas(nullptr),
      csr_linha_ptr(nullptr), csr_num_linhas(0), csr_cap_celulas(0),
      valores_atributos(nullptr), qtd_valores_atributos(nullptr),
      cap_valores_atributos(nullptr), cap_estrutura_atributos(0) {

    if (modo != 'L' && modo != 'T' && modo != 'C') {
        throw ExcecaoModoInvalido(std::string(1, modo));
    }

    if (modo == 'L') {
        cap_linhas = 16;
        cap_colunas = 16;
        cabecas_linhas = new CelulaCruzada*[cap_linhas];
        cabecas_colunas = new CelulaCruzada*[cap_colunas];
        for (int i = 0; i < cap_linhas; i++) cabecas_linhas[i] = nullptr;
        for (int j = 0; j < cap_colunas; j++) cabecas_colunas[j] = nullptr;
    } else if (modo == 'T') {
        cap_tripletas = 16;
        tripletas = new Tripleta[cap_tripletas];
    } else if (modo == 'C') {
        csr_num_linhas = 16;
        csr_linha_ptr = new int[csr_num_linhas + 1];
        for (int i = 0; i <= csr_num_linhas; i++) csr_linha_ptr[i] = 0;
        csr_cap_celulas = 16;
        csr_valores = new int[csr_cap_celulas];
        csr_colunas = new int[csr_cap_celulas];
    }

    // Inicializa estrutura de índices por atributo para consulta LF
    cap_estrutura_atributos = 16;
    valores_atributos = new EntradaAtributoValor*[cap_estrutura_atributos];
    qtd_valores_atributos = new int[cap_estrutura_atributos];
    cap_valores_atributos = new int[cap_estrutura_atributos];
    for (int a = 0; a < cap_estrutura_atributos; a++) {
        valores_atributos[a] = nullptr;
        qtd_valores_atributos[a] = 0;
        cap_valores_atributos[a] = 0;
    }
}

Catalogo::~Catalogo() {
    liberarMemoriaL();
    liberarMemoriaT();
    liberarMemoriaCSR();
    liberarEstruturaAtributos();
}

void Catalogo::liberarMemoriaL() {
    if (cabecas_linhas != nullptr) {
        for (int i = 0; i < cap_linhas; i++) {
            CelulaCruzada* atual = cabecas_linhas[i];
            while (atual != nullptr) {
                CelulaCruzada* prox = atual->prox_coluna;
                delete atual;
                atual = prox;
            }
        }
        delete[] cabecas_linhas;
        cabecas_linhas = nullptr;
        cap_linhas = 0;
    }
    if (cabecas_colunas != nullptr) {
        delete[] cabecas_colunas;
        cabecas_colunas = nullptr;
        cap_colunas = 0;
    }
}

void Catalogo::liberarMemoriaT() {
    if (tripletas != nullptr) {
        delete[] tripletas;
        tripletas = nullptr;
        cap_tripletas = 0;
    }
}

void Catalogo::liberarMemoriaCSR() {
    if (csr_valores != nullptr) {
        delete[] csr_valores;
        csr_valores = nullptr;
    }
    if (csr_colunas != nullptr) {
        delete[] csr_colunas;
        csr_colunas = nullptr;
    }
    if (csr_linha_ptr != nullptr) {
        delete[] csr_linha_ptr;
        csr_linha_ptr = nullptr;
    }
    csr_num_linhas = 0;
    csr_cap_celulas = 0;
}

void Catalogo::liberarEstruturaAtributos() {
    if (valores_atributos != nullptr) {
        for (int a = 0; a < cap_estrutura_atributos; a++) {
            if (valores_atributos[a] != nullptr) {
                delete[] valores_atributos[a];
            }
        }
        delete[] valores_atributos;
        delete[] qtd_valores_atributos;
        delete[] cap_valores_atributos;
        valores_atributos = nullptr;
        qtd_valores_atributos = nullptr;
        cap_valores_atributos = nullptr;
        cap_estrutura_atributos = 0;
    }
}

void Catalogo::redimensionarLinhasL(int novo_tamanho) {
    CelulaCruzada** novas_cabecas = new CelulaCruzada*[novo_tamanho];
    for (int i = 0; i < cap_linhas; i++) {
        novas_cabecas[i] = cabecas_linhas[i];
    }
    for (int i = cap_linhas; i < novo_tamanho; i++) {
        novas_cabecas[i] = nullptr;
    }
    delete[] cabecas_linhas;
    cabecas_linhas = novas_cabecas;
    cap_linhas = novo_tamanho;
}

void Catalogo::redimensionarColunasL(int novo_tamanho) {
    CelulaCruzada** novas_cabecas = new CelulaCruzada*[novo_tamanho];
    for (int j = 0; j < cap_colunas; j++) {
        novas_cabecas[j] = cabecas_colunas[j];
    }
    for (int j = cap_colunas; j < novo_tamanho; j++) {
        novas_cabecas[j] = nullptr;
    }
    delete[] cabecas_colunas;
    cabecas_colunas = novas_cabecas;
    cap_colunas = novo_tamanho;
}

void Catalogo::redimensionarT(int nova_capacidade) {
    Tripleta* novas_tripletas = new Tripleta[nova_capacidade];
    for (int i = 0; i < total_celulas; i++) {
        novas_tripletas[i] = tripletas[i];
    }
    delete[] tripletas;
    tripletas = novas_tripletas;
    cap_tripletas = nova_capacidade;
}

void Catalogo::redimensionarCSRLinhas(int novo_num_linhas) {
    int* novo_linha_ptr = new int[novo_num_linhas + 1];
    for (int i = 0; i <= csr_num_linhas; i++) {
        novo_linha_ptr[i] = csr_linha_ptr[i];
    }
    int ult = csr_linha_ptr[csr_num_linhas];
    for (int i = csr_num_linhas + 1; i <= novo_num_linhas; i++) {
        novo_linha_ptr[i] = ult;
    }
    delete[] csr_linha_ptr;
    csr_linha_ptr = novo_linha_ptr;
    csr_num_linhas = novo_num_linhas;
}

void Catalogo::redimensionarCSRCelulas(int nova_cap) {
    int* novos_vals = new int[nova_cap];
    int* novas_cols = new int[nova_cap];
    for (int i = 0; i < total_celulas; i++) {
        novos_vals[i] = csr_valores[i];
        novas_cols[i] = csr_colunas[i];
    }
    delete[] csr_valores;
    delete[] csr_colunas;
    csr_valores = novos_vals;
    csr_colunas = novas_cols;
    csr_cap_celulas = nova_cap;
}

void Catalogo::redimensionarEstruturaAtributos(int novo_tam) {
    EntradaAtributoValor** novos_val = new EntradaAtributoValor*[novo_tam];
    int* novas_qtd = new int[novo_tam];
    int* novas_cap = new int[novo_tam];

    for (int a = 0; a < cap_estrutura_atributos; a++) {
        novos_val[a] = valores_atributos[a];
        novas_qtd[a] = qtd_valores_atributos[a];
        novas_cap[a] = cap_valores_atributos[a];
    }
    for (int a = cap_estrutura_atributos; a < novo_tam; a++) {
        novos_val[a] = nullptr;
        novas_qtd[a] = 0;
        novas_cap[a] = 0;
    }

    delete[] valores_atributos;
    delete[] qtd_valores_atributos;
    delete[] cap_valores_atributos;

    valores_atributos = novos_val;
    qtd_valores_atributos = novas_qtd;
    cap_valores_atributos = novas_cap;
    cap_estrutura_atributos = novo_tam;
}

void Catalogo::atualizarIndiceValor(int item, int atributo, int novo_valor, int valor_antigo) {
    if (atributo >= cap_estrutura_atributos) {
        int novo_tam = atributo + 1 > cap_estrutura_atributos * 2 ? atributo + 1 : cap_estrutura_atributos * 2;
        redimensionarEstruturaAtributos(novo_tam);
    }

    // Se já havia um valor antigo, remover a entrada
    if (valor_antigo != 0) {
        int qtd = qtd_valores_atributos[atributo];
        EntradaAtributoValor* arr = valores_atributos[atributo];
        int pos = -1;
        for (int i = 0; i < qtd; i++) {
            if (arr[i].item == item) {
                pos = i;
                break;
            }
        }
        if (pos != -1) {
            for (int i = pos; i < qtd - 1; i++) {
                arr[i] = arr[i + 1];
            }
            qtd_valores_atributos[atributo]--;
        }
    }

    // Se o novo valor for diferente de zero, inserir ordenadamente por valor
    if (novo_valor != 0) {
        int qtd = qtd_valores_atributos[atributo];
        int cap = cap_valores_atributos[atributo];
        if (qtd >= cap) {
            int nova_cap = cap > 0 ? cap * 2 : 4;
            EntradaAtributoValor* novo_arr = new EntradaAtributoValor[nova_cap];
            for (int i = 0; i < qtd; i++) novo_arr[i] = valores_atributos[atributo][i];
            delete[] valores_atributos[atributo];
            valores_atributos[atributo] = novo_arr;
            cap_valores_atributos[atributo] = nova_cap;
        }

        EntradaAtributoValor* arr = valores_atributos[atributo];
        // Busca binária pelo ponto de inserção por valor
        int inicio = 0, fim = qtd - 1;
        int idx = qtd;
        while (inicio <= fim) {
            int meio = inicio + (fim - inicio) / 2;
            if (arr[meio].valor >= novo_valor) {
                idx = meio;
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }

        for (int i = qtd; i > idx; i--) {
            arr[i] = arr[i - 1];
        }
        arr[idx].item = item;
        arr[idx].valor = novo_valor;
        qtd_valores_atributos[atributo]++;
    }
}

int Catalogo::buscarIndiceT(int item, int atributo, bool& encontrado) const {
    int inicio = 0;
    int fim = total_celulas - 1;
    encontrado = false;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        if (tripletas[meio].item == item && tripletas[meio].atributo == atributo) {
            encontrado = true;
            return meio;
        }
        if (tripletas[meio].item < item ||
            (tripletas[meio].item == item && tripletas[meio].atributo < atributo)) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return inicio;
}

void Catalogo::setModo(char novo_modo) {
    if (novo_modo != 'L' && novo_modo != 'T' && novo_modo != 'C') {
        throw ExcecaoModoInvalido(std::string(1, novo_modo));
    }
    if (novo_modo == modo) return;

    // Extrair todas as células em ordem (item, atributo)
    Tripleta* todas = nullptr;
    int n_celulas = total_celulas;
    if (n_celulas > 0) {
        todas = new Tripleta[n_celulas];
        int count = 0;

        if (modo == 'L') {
            for (int i = 0; i < cap_linhas; i++) {
                CelulaCruzada* atual = cabecas_linhas[i];
                while (atual != nullptr) {
                    todas[count].item = atual->item;
                    todas[count].atributo = atual->atributo;
                    todas[count].valor = atual->valor;
                    count++;
                    atual = atual->prox_coluna;
                }
            }
        } else if (modo == 'T') {
            for (int k = 0; k < total_celulas; k++) {
                todas[k] = tripletas[k];
            }
        } else if (modo == 'C') {
            for (int i = 0; i < csr_num_linhas; i++) {
                for (int p = csr_linha_ptr[i]; p < csr_linha_ptr[i + 1]; p++) {
                    todas[count].item = i;
                    todas[count].atributo = csr_colunas[p];
                    todas[count].valor = csr_valores[p];
                    count++;
                }
            }
        }
    }

    // Liberar modo anterior
    if (modo == 'L') liberarMemoriaL();
    else if (modo == 'T') liberarMemoriaT();
    else if (modo == 'C') liberarMemoriaCSR();

    modo = novo_modo;
    total_celulas = 0;

    // Configurar novo modo a partir das tripletas coletadas
    if (modo == 'T') {
        cap_tripletas = n_celulas > 16 ? n_celulas * 2 : 16;
        tripletas = new Tripleta[cap_tripletas];
        for (int k = 0; k < n_celulas; k++) {
            tripletas[k] = todas[k];
        }
        total_celulas = n_celulas;
    } else if (modo == 'L') {
        cap_linhas = 16;
        cap_colunas = 16;
        for (int k = 0; k < n_celulas; k++) {
            if (todas[k].item >= cap_linhas) cap_linhas = todas[k].item + 1;
            if (todas[k].atributo >= cap_colunas) cap_colunas = todas[k].atributo + 1;
        }
        cabecas_linhas = new CelulaCruzada*[cap_linhas];
        cabecas_colunas = new CelulaCruzada*[cap_colunas];
        for (int i = 0; i < cap_linhas; i++) cabecas_linhas[i] = nullptr;
        for (int j = 0; j < cap_colunas; j++) cabecas_colunas[j] = nullptr;

        for (int k = 0; k < n_celulas; k++) {
            alterarModificador(todas[k].item, todas[k].atributo, todas[k].valor);
        }
    } else if (modo == 'C') {
        int max_item = 15;
        for (int k = 0; k < n_celulas; k++) {
            if (todas[k].item > max_item) max_item = todas[k].item;
        }
        csr_num_linhas = max_item + 1;
        csr_linha_ptr = new int[csr_num_linhas + 1];
        for (int i = 0; i <= csr_num_linhas; i++) csr_linha_ptr[i] = 0;

        csr_cap_celulas = n_celulas > 16 ? n_celulas * 2 : 16;
        csr_valores = new int[csr_cap_celulas];
        csr_colunas = new int[csr_cap_celulas];

        // Contar por linha
        for (int k = 0; k < n_celulas; k++) {
            csr_linha_ptr[todas[k].item + 1]++;
        }
        // Prefix sum
        for (int i = 0; i < csr_num_linhas; i++) {
            csr_linha_ptr[i + 1] += csr_linha_ptr[i];
        }
        // Preencher
        for (int k = 0; k < n_celulas; k++) {
            csr_valores[k] = todas[k].valor;
            csr_colunas[k] = todas[k].atributo;
        }
        total_celulas = n_celulas;
    }

    if (todas != nullptr) delete[] todas;
}

char Catalogo::getModo() const {
    return modo;
}

int Catalogo::getTotalCelulas() const {
    return total_celulas;
}

void Catalogo::alterarModificador(int id_item, int id_atributo, int valor) {
    if (id_item < 0 || id_atributo < 0) {
        throw ExcecaoIndiceInvalido("item ou atributo", id_item < 0 ? id_item : id_atributo);
    }

    int valor_antigo = consultarModificador(id_item, id_atributo);
    atualizarIndiceValor(id_item, id_atributo, valor, valor_antigo);

    if (modo == 'L') {
        if (id_item >= cap_linhas) {
            int novo_tam = (id_item + 1 > cap_linhas * 2) ? id_item + 1 : cap_linhas * 2;
            redimensionarLinhasL(novo_tam);
        }
        if (id_atributo >= cap_colunas) {
            int novo_tam = (id_atributo + 1 > cap_colunas * 2) ? id_atributo + 1 : cap_colunas * 2;
            redimensionarColunasL(novo_tam);
        }

        CelulaCruzada* ant_col = nullptr;
        CelulaCruzada* curr_col = cabecas_linhas[id_item];
        while (curr_col != nullptr && curr_col->atributo < id_atributo) {
            ant_col = curr_col;
            curr_col = curr_col->prox_coluna;
        }

        if (curr_col != nullptr && curr_col->atributo == id_atributo) {
            if (valor == 0) {
                if (ant_col == nullptr) cabecas_linhas[id_item] = curr_col->prox_coluna;
                else ant_col->prox_coluna = curr_col->prox_coluna;

                CelulaCruzada* ant_lin = nullptr;
                CelulaCruzada* curr_lin = cabecas_colunas[id_atributo];
                while (curr_lin != nullptr && curr_lin != curr_col) {
                    ant_lin = curr_lin;
                    curr_lin = curr_lin->prox_linha;
                }
                if (curr_lin == curr_col) {
                    if (ant_lin == nullptr) cabecas_colunas[id_atributo] = curr_col->prox_linha;
                    else ant_lin->prox_linha = curr_col->prox_linha;
                }

                delete curr_col;
                total_celulas--;
            } else {
                curr_col->valor = valor;
            }
        } else {
            if (valor == 0) return;

            CelulaCruzada* nova = new CelulaCruzada(id_item, id_atributo, valor);
            nova->prox_coluna = curr_col;
            if (ant_col == nullptr) cabecas_linhas[id_item] = nova;
            else ant_col->prox_coluna = nova;

            CelulaCruzada* ant_lin = nullptr;
            CelulaCruzada* curr_lin = cabecas_colunas[id_atributo];
            while (curr_lin != nullptr && curr_lin->item < id_item) {
                ant_lin = curr_lin;
                curr_lin = curr_lin->prox_linha;
            }
            nova->prox_linha = curr_lin;
            if (ant_lin == nullptr) cabecas_colunas[id_atributo] = nova;
            else ant_lin->prox_linha = nova;

            total_celulas++;
        }
    } else if (modo == 'T') {
        bool encontrado = false;
        int idx = buscarIndiceT(id_item, id_atributo, encontrado);

        if (encontrado) {
            if (valor == 0) {
                for (int k = idx; k < total_celulas - 1; k++) {
                    tripletas[k] = tripletas[k + 1];
                }
                total_celulas--;
            } else {
                tripletas[idx].valor = valor;
            }
        } else {
            if (valor == 0) return;

            if (total_celulas >= cap_tripletas) {
                int nova_cap = cap_tripletas > 0 ? cap_tripletas * 2 : 16;
                redimensionarT(nova_cap);
            }

            for (int k = total_celulas; k > idx; k--) {
                tripletas[k] = tripletas[k - 1];
            }
            tripletas[idx].item = id_item;
            tripletas[idx].atributo = id_atributo;
            tripletas[idx].valor = valor;
            total_celulas++;
        }
    } else if (modo == 'C') {
        if (id_item >= csr_num_linhas) {
            int novo_n = id_item + 1 > csr_num_linhas * 2 ? id_item + 1 : csr_num_linhas * 2;
            redimensionarCSRLinhas(novo_n);
        }

        int start = csr_linha_ptr[id_item];
        int end = csr_linha_ptr[id_item + 1];

        int pos = -1;
        int insert_pos = end;
        for (int p = start; p < end; p++) {
            if (csr_colunas[p] == id_atributo) {
                pos = p;
                break;
            }
            if (csr_colunas[p] > id_atributo) {
                insert_pos = p;
                break;
            }
        }

        if (pos != -1) {
            if (valor == 0) {
                for (int p = pos; p < total_celulas - 1; p++) {
                    csr_valores[p] = csr_valores[p + 1];
                    csr_colunas[p] = csr_colunas[p + 1];
                }
                total_celulas--;
                for (int i = id_item + 1; i <= csr_num_linhas; i++) {
                    csr_linha_ptr[i]--;
                }
            } else {
                csr_valores[pos] = valor;
            }
        } else {
            if (valor == 0) return;

            if (total_celulas >= csr_cap_celulas) {
                int nova_cap = csr_cap_celulas > 0 ? csr_cap_celulas * 2 : 16;
                redimensionarCSRCelulas(nova_cap);
            }

            for (int p = total_celulas; p > insert_pos; p--) {
                csr_valores[p] = csr_valores[p - 1];
                csr_colunas[p] = csr_colunas[p - 1];
            }
            csr_valores[insert_pos] = valor;
            csr_colunas[insert_pos] = id_atributo;
            total_celulas++;
            for (int i = id_item + 1; i <= csr_num_linhas; i++) {
                csr_linha_ptr[i]++;
            }
        }
    }
}

int Catalogo::consultarModificador(int id_item, int id_atributo) const {
    if (id_item < 0 || id_atributo < 0) return 0;

    if (modo == 'L') {
        if (id_item >= cap_linhas) return 0;
        CelulaCruzada* curr = cabecas_linhas[id_item];
        while (curr != nullptr && curr->atributo <= id_atributo) {
            if (curr->atributo == id_atributo) return curr->valor;
            curr = curr->prox_coluna;
        }
        return 0;
    } else if (modo == 'T') {
        bool encontrado = false;
        int idx = buscarIndiceT(id_item, id_atributo, encontrado);
        if (encontrado) return tripletas[idx].valor;
        return 0;
    } else {
        if (id_item >= csr_num_linhas) return 0;
        int start = csr_linha_ptr[id_item];
        int end = csr_linha_ptr[id_item + 1];
        for (int p = start; p < end; p++) {
            if (csr_colunas[p] == id_atributo) return csr_valores[p];
            if (csr_colunas[p] > id_atributo) break;
        }
        return 0;
    }
}

void Catalogo::removerModificador(int id_item, int id_atributo) {
    alterarModificador(id_item, id_atributo, 0);
}

int Catalogo::obterModificadoresItem(int id_item, int*& atributos, int*& valores) const {
    if (id_item < 0) {
        atributos = nullptr;
        valores = nullptr;
        return 0;
    }

    if (modo == 'L') {
        if (id_item >= cap_linhas || cabecas_linhas[id_item] == nullptr) {
            atributos = nullptr;
            valores = nullptr;
            return 0;
        }
        int count = 0;
        CelulaCruzada* curr = cabecas_linhas[id_item];
        while (curr != nullptr) {
            count++;
            curr = curr->prox_coluna;
        }
        atributos = new int[count];
        valores = new int[count];
        curr = cabecas_linhas[id_item];
        int idx = 0;
        while (curr != nullptr) {
            atributos[idx] = curr->atributo;
            valores[idx] = curr->valor;
            idx++;
            curr = curr->prox_coluna;
        }
        return count;
    } else if (modo == 'T') {
        int inicio = 0, fim = total_celulas - 1;
        int primeiro = -1;
        while (inicio <= fim) {
            int meio = inicio + (fim - inicio) / 2;
            if (tripletas[meio].item == id_item) {
                primeiro = meio;
                fim = meio - 1;
            } else if (tripletas[meio].item < id_item) {
                inicio = meio + 1;
            } else {
                fim = meio - 1;
            }
        }

        if (primeiro == -1) {
            atributos = nullptr;
            valores = nullptr;
            return 0;
        }

        int count = 0;
        while (primeiro + count < total_celulas && tripletas[primeiro + count].item == id_item) {
            count++;
        }

        atributos = new int[count];
        valores = new int[count];
        for (int k = 0; k < count; k++) {
            atributos[k] = tripletas[primeiro + k].atributo;
            valores[k] = tripletas[primeiro + k].valor;
        }
        return count;
    } else {
        if (id_item >= csr_num_linhas) {
            atributos = nullptr;
            valores = nullptr;
            return 0;
        }
        int start = csr_linha_ptr[id_item];
        int end = csr_linha_ptr[id_item + 1];
        int count = end - start;
        if (count <= 0) {
            atributos = nullptr;
            valores = nullptr;
            return 0;
        }
        atributos = new int[count];
        valores = new int[count];
        for (int p = 0; p < count; p++) {
            atributos[p] = csr_colunas[start + p];
            valores[p] = csr_valores[start + p];
        }
        return count;
    }
}

int Catalogo::obterItensAtributo(int id_atributo, int*& itens, int*& valores) const {
    if (id_atributo < 0) {
        itens = nullptr;
        valores = nullptr;
        return 0;
    }

    if (modo == 'L') {
        if (id_atributo >= cap_colunas || cabecas_colunas[id_atributo] == nullptr) {
            itens = nullptr;
            valores = nullptr;
            return 0;
        }
        int count = 0;
        CelulaCruzada* curr = cabecas_colunas[id_atributo];
        while (curr != nullptr) {
            count++;
            curr = curr->prox_linha;
        }
        itens = new int[count];
        valores = new int[count];
        curr = cabecas_colunas[id_atributo];
        int idx = 0;
        while (curr != nullptr) {
            itens[idx] = curr->item;
            valores[idx] = curr->valor;
            idx++;
            curr = curr->prox_linha;
        }
        return count;
    } else if (modo == 'T') {
        int count = 0;
        for (int k = 0; k < total_celulas; k++) {
            if (tripletas[k].atributo == id_atributo) count++;
        }
        if (count == 0) {
            itens = nullptr;
            valores = nullptr;
            return 0;
        }
        itens = new int[count];
        valores = new int[count];
        int idx = 0;
        for (int k = 0; k < total_celulas; k++) {
            if (tripletas[k].atributo == id_atributo) {
                itens[idx] = tripletas[k].item;
                valores[idx] = tripletas[k].valor;
                idx++;
            }
        }
        return count;
    } else {
        int count = 0;
        for (int i = 0; i < csr_num_linhas; i++) {
            for (int p = csr_linha_ptr[i]; p < csr_linha_ptr[i + 1]; p++) {
                if (csr_colunas[p] == id_atributo) count++;
            }
        }
        if (count == 0) {
            itens = nullptr;
            valores = nullptr;
            return 0;
        }
        itens = new int[count];
        valores = new int[count];
        int idx = 0;
        for (int i = 0; i < csr_num_linhas; i++) {
            for (int p = csr_linha_ptr[i]; p < csr_linha_ptr[i + 1]; p++) {
                if (csr_colunas[p] == id_atributo) {
                    itens[idx] = i;
                    valores[idx] = csr_valores[p];
                    idx++;
                }
            }
        }
        return count;
    }
}

double Catalogo::calcularPoder(int id_item) const {
    int* atribs = nullptr;
    int* vals = nullptr;
    int n = obterModificadoresItem(id_item, atribs, vals);
    double poder = 0.0;
    for (int k = 0; k < n; k++) poder += vals[k];
    if (atribs) delete[] atribs;
    if (vals) delete[] vals;
    return poder;
}

double Catalogo::calcularEquilibrio(int id_item, int total_atributos) const {
    if (total_atributos <= 0) return 1.0;

    int* atribs = nullptr;
    int* vals = nullptr;
    int n = obterModificadoresItem(id_item, atribs, vals);

    double poder = 0.0;
    for (int k = 0; k < n; k++) poder += vals[k];
    double mu = poder / (double)total_atributos;

    double soma_quad = 0.0;
    for (int k = 0; k < n; k++) {
        double diff = (double)vals[k] - mu;
        soma_quad += diff * diff;
    }
    int num_zeros = total_atributos - n;
    if (num_zeros > 0) {
        soma_quad += (double)num_zeros * (mu * mu);
    }

    double sigma = std::sqrt(soma_quad / (double)total_atributos);

    if (atribs) delete[] atribs;
    if (vals) delete[] vals;

    return 1.0 / (1.0 + sigma);
}

double Catalogo::calcularSinergia(int id_item, const int* build_aventureiro, int total_atributos) const {
    if (build_aventureiro == nullptr || total_atributos <= 0) return 0.0;

    int* atribs = nullptr;
    int* vals = nullptr;
    int n = obterModificadoresItem(id_item, atribs, vals);

    if (n == 0) {
        if (atribs) delete[] atribs;
        if (vals) delete[] vals;
        return 0.0;
    }

    double soma_b2 = 0.0;
    for (int a = 0; a < total_atributos; a++) {
        soma_b2 += (double)build_aventureiro[a] * (double)build_aventureiro[a];
    }
    if (soma_b2 <= 0.0) {
        if (atribs) delete[] atribs;
        if (vals) delete[] vals;
        return 0.0;
    }

    double soma_prod = 0.0;
    double soma_v2 = 0.0;
    for (int k = 0; k < n; k++) {
        int a = atribs[k];
        double v = (double)vals[k];
        soma_v2 += v * v;
        if (a < total_atributos) {
            soma_prod += v * (double)build_aventureiro[a];
        }
    }

    if (soma_v2 <= 0.0) {
        if (atribs) delete[] atribs;
        if (vals) delete[] vals;
        return 0.0;
    }

    double denominador = std::sqrt(soma_v2) * std::sqrt(soma_b2);
    if (denominador <= 0.0) {
        if (atribs) delete[] atribs;
        if (vals) delete[] vals;
        return 0.0;
    }

    if (atribs) delete[] atribs;
    if (vals) delete[] vals;

    return soma_prod / denominador;
}

double Catalogo::produtoInterno(int id_item, const int* vetor, int tamanho_vetor) const {
    if (vetor == nullptr || tamanho_vetor <= 0) return 0.0;

    int* atribs = nullptr;
    int* vals = nullptr;
    int n = obterModificadoresItem(id_item, atribs, vals);

    double resultado = 0.0;
    for (int k = 0; k < n; k++) {
        int a = atribs[k];
        if (a < tamanho_vetor) {
            resultado += (double)vals[k] * (double)vetor[a];
        }
    }

    if (atribs) delete[] atribs;
    if (vals) delete[] vals;
    return resultado;
}

// Ponto Extra: Consulta LF por faixa de valores
int Catalogo::consultarFaixa(int id_atributo, int valor_min, int valor_max, int total_itens,
                             int*& itens_saida, int*& valores_saida) const {
    if (id_atributo < 0 || total_itens <= 0) {
        itens_saida = nullptr;
        valores_saida = nullptr;
        return 0;
    }

    bool zero_na_faixa = (valor_min <= 0 && valor_max >= 0);

    if (zero_na_faixa) {
        // Itens sem célula possuem valor 0 e entram na faixa
        // Para cada item de 0 até total_itens - 1, verifica o valor no atributo
        int cap = total_itens;
        int* temp_itens = new int[cap];
        int* temp_vals = new int[cap];
        int count = 0;

        for (int i = 0; i < total_itens; i++) {
            int v = consultarModificador(i, id_atributo);
            if (v >= valor_min && v <= valor_max) {
                temp_itens[count] = i;
                temp_vals[count] = v;
                count++;
            }
        }

        if (count == 0) {
            delete[] temp_itens;
            delete[] temp_vals;
            itens_saida = nullptr;
            valores_saida = nullptr;
            return 0;
        }

        itens_saida = new int[count];
        valores_saida = new int[count];
        for (int k = 0; k < count; k++) {
            itens_saida[k] = temp_itens[k];
            valores_saida[k] = temp_vals[k];
        }
        delete[] temp_itens;
        delete[] temp_vals;
        return count;
    } else {
        // 0 não está na faixa. Apenas itens com células explícitas podem participar.
        // Utiliza busca binária sobre o vetor ordenado de valores do atributo.
        if (id_atributo >= cap_estrutura_atributos || qtd_valores_atributos[id_atributo] == 0) {
            itens_saida = nullptr;
            valores_saida = nullptr;
            return 0;
        }

        int qtd = qtd_valores_atributos[id_atributo];
        EntradaAtributoValor* arr = valores_atributos[id_atributo];

        // Busca binária para achar o primeiro com valor >= valor_min
        int inicio = 0, fim = qtd - 1;
        int idx_inicio = qtd;
        while (inicio <= fim) {
            int meio = inicio + (fim - inicio) / 2;
            if (arr[meio].valor >= valor_min) {
                idx_inicio = meio;
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }

        // Busca binária para achar o último com valor <= valor_max
        inicio = 0; fim = qtd - 1;
        int idx_fim = -1;
        while (inicio <= fim) {
            int meio = inicio + (fim - inicio) / 2;
            if (arr[meio].valor <= valor_max) {
                idx_fim = meio;
                inicio = meio + 1;
            } else {
                fim = meio - 1;
            }
        }

        if (idx_inicio > idx_fim || idx_inicio >= qtd || idx_fim < 0) {
            itens_saida = nullptr;
            valores_saida = nullptr;
            return 0;
        }

        int count = idx_fim - idx_inicio + 1;
        // Coletar itens encontrados
        EntradaAtributoValor* encontrados = new EntradaAtributoValor[count];
        for (int k = 0; k < count; k++) {
            encontrados[k] = arr[idx_inicio + k];
        }

        // Ordenar os itens encontrados por id_item crescente (requisito da consulta LF)
        for (int i = 1; i < count; i++) {
            EntradaAtributoValor chave = encontrados[i];
            int j = i - 1;
            while (j >= 0 && encontrados[j].item > chave.item) {
                encontrados[j + 1] = encontrados[j];
                j--;
            }
            encontrados[j + 1] = chave;
        }

        itens_saida = new int[count];
        valores_saida = new int[count];
        for (int k = 0; k < count; k++) {
            itens_saida[k] = encontrados[k].item;
            valores_saida[k] = encontrados[k].valor;
        }
        delete[] encontrados;
        return count;
    }
}
