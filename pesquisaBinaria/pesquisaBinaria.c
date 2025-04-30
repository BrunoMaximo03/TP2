#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINHA 1024
#define MAX_PROGRAMAS 10000
#define MAX_ITENS 50

// Struct Data
typedef struct Data {
    char mes[20];
    int dia;
    int ano;
} Data;

// Struct Programa
typedef struct Show {
    char* id_programa;
    char* tipo;
    char* titulo;
    char* diretor;
    char** elenco;       
    int tamanhoElenco;      
    char* pais;
    Data data_adicionado;   // Pega struct Data
    int ano_lancamento;
    char* classificacao;
    char* duracao;
    char** categorias; 
    int tamanhoCategorias;
    char* descricao;
} Show;

Show* catalogo[MAX_PROGRAMAS];
int totalProgramas = 0;

// Variáveis para o log
int comparacoes = 0;

// Protótipos das funções
char* limparString(char* str);
char* removerAspas(char* str);
char* processarCampo(char* campo);
char** processarLista(char* str, int* tamanho);
int dividirLinha(char* linha, char* campos[], int maxCampos);
void lerCSV();
bool pesquisaBinaria(Show* array[], int n, char* chave);
void ordenarTitulo(Show* array[], int n);
bool isFim(char* str);

char* limparString(char* str) {
    if (!str) return strdup("NaN");
    while (isspace(*str)) str++;
    char* fim = str + strlen(str) - 1;
    while (fim > str && isspace(*fim)) fim--;
    *(fim + 1) = '\0';
    return strdup(str);
}

char* removerAspas(char* str) {
    if (!str) return NULL;
    char* resultado = malloc(strlen(str) + 1);
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] != '"') {
            resultado[j++] = str[i];
        }
    }
    resultado[j] = '\0';
    return resultado;
}

char* processarCampo(char* campo) {
    if (!campo || strlen(campo) == 0) return strdup("NaN");
    char* semAspas = removerAspas(campo);
    char* resultado = limparString(semAspas);
    free(semAspas);
    return resultado;
}

char** processarLista(char* str, int* tamanho) {
    char** itens = malloc(MAX_ITENS * sizeof(char*));
    *tamanho = 0;

    if (!str || strlen(str) == 0) {
        itens[(*tamanho)++] = strdup("NaN");
        return itens;
    }

    char* limpo = removerAspas(str);
    char* token = strtok(limpo, ",");
    while (token != NULL && *tamanho < MAX_ITENS) {
        char* item = processarCampo(token);
        itens[(*tamanho)++] = item;
        token = strtok(NULL, ",");
    }

    free(limpo);

    // Bubble Sort para ordenar
    for (int i = 0; i < *tamanho - 1; i++) {
        for (int j = 0; j < *tamanho - i - 1; j++) {
            if (strcasecmp(itens[j], itens[j + 1]) > 0) {
                char* temp = itens[j];
                itens[j] = itens[j + 1];
                itens[j + 1] = temp;
            }
        }
    }

    if (*tamanho == 0) {
        itens[(*tamanho)++] = strdup("NaN");
    }
    return itens;
}

int dividirLinha(char* linha, char* campos[], int maxCampos) {
    int i = 0;
    char* ptr = linha;
    while (*ptr && i < maxCampos) {
        if (*ptr == '"') {
            ptr++;
            char* inicio = ptr;
            while (*ptr && !(*ptr == '"' && (*(ptr + 1) == ',' || *(ptr + 1) == '\0'))) {
                ptr++;
            }
            int len = ptr - inicio;
            campos[i] = malloc(len + 1);
            strncpy(campos[i], inicio, len);
            campos[i][len] = '\0';
            if (*ptr == '"') ptr++;
            if (*ptr == ',') ptr++;
        } else {
            char* inicio = ptr;
            while (*ptr && *ptr != ',') ptr++;
            int len = ptr - inicio;
            campos[i] = malloc(len + 1);
            strncpy(campos[i], inicio, len);
            campos[i][len] = '\0';
            if (*ptr == ',') ptr++;
        }
        i++;
    }
    return i;
}

void lerCSV() {
    FILE* arquivo = fopen("/tmp/disneyplus.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[MAX_LINHA];
    fgets(linha, MAX_LINHA, arquivo); // Pula a primeira linha

    while (fgets(linha, MAX_LINHA, arquivo)) {
        linha[strcspn(linha, "\n")] = 0; // Remove o caractere de nova linha

        char* campos[15];
        int quantidadeCampos = dividirLinha(linha, campos, 15);

        Show* novoPrograma = malloc(sizeof(Show));
        
        novoPrograma->id_programa = processarCampo(campos[0]);
        novoPrograma->tipo = processarCampo(campos[1]);
        novoPrograma->titulo = processarCampo(campos[2]);
        novoPrograma->diretor = processarCampo(campos[3]);
        novoPrograma->elenco = processarLista(campos[4], &novoPrograma->tamanhoElenco);
        novoPrograma->pais = processarCampo(campos[5]);

        // Trata campo de data
        char* dataStr = processarCampo(campos[6]);
        if (strcmp(dataStr, "NaN") != 0) {
            char* parte = strtok(dataStr, " ");
            strcpy(novoPrograma->data_adicionado.mes, parte ? parte : "NaN");
            parte = strtok(NULL, ",");
            novoPrograma->data_adicionado.dia = parte ? atoi(parte) : -1;
            parte = strtok(NULL, "");
            novoPrograma->data_adicionado.ano = parte ? atoi(parte) : -1;
        } else {
            strcpy(novoPrograma->data_adicionado.mes, "March");
            novoPrograma->data_adicionado.dia = 1;
            novoPrograma->data_adicionado.ano = 1900;
        }
        free(dataStr);

        novoPrograma->ano_lancamento = (quantidadeCampos > 7 && strlen(campos[7]) > 0) ? atoi(campos[7]) : -1;
        novoPrograma->classificacao = processarCampo(campos[8]);
        novoPrograma->duracao = processarCampo(campos[9]);
        novoPrograma->categorias = processarLista(campos[10], &novoPrograma->tamanhoCategorias);
        novoPrograma->descricao = processarCampo(campos[11]);

        for (int i = 0; i < quantidadeCampos; i++) free(campos[i]);
        catalogo[totalProgramas++] = novoPrograma;
    }

    fclose(arquivo);
}

bool pesquisaBinaria(Show* array[], int n, char* chave) {
    int esquerda = 0, direita = n - 1;
    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        int cmp = strcasecmp(array[meio]->titulo, chave);
        comparacoes++;
        if (cmp == 0) {
            return true;
        } else if (cmp < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return false; 
}

void ordenarTitulo(Show* array[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcasecmp(array[j]->titulo, array[j + 1]->titulo) > 0) {
                Show* temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

bool isFim(char* str) {
    return strcasecmp(str, "FIM") == 0;
}

int main() {
    lerCSV(); 

    char entrada[50];
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;

    Show* resultados[MAX_PROGRAMAS];
    int tamanhoResultados = 0;

    while (!isFim(entrada)) {
        for (int i = 0; i < totalProgramas; i++) {
            if (strcasecmp(catalogo[i]->id_programa, entrada) == 0) {
                resultados[tamanhoResultados++] = catalogo[i];
                break;
            }
        }
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;
    }

    ordenarTitulo(resultados, tamanhoResultados);

    clock_t inicio = clock();

    char pesquisa[50];
    fgets(pesquisa, sizeof(pesquisa), stdin);
    pesquisa[strcspn(pesquisa, "\n")] = 0;

    while (!isFim(pesquisa)) {
        if (pesquisaBinaria(resultados, tamanhoResultados, pesquisa)) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
        fgets(pesquisa, sizeof(pesquisa), stdin);
        pesquisa[strcspn(pesquisa, "\n")] = 0;
    }

    clock_t fim = clock();

    double tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    FILE* log = fopen("matricula_pesquisa_binaria.txt", "w");
    if (log) {
        fprintf(log, "850847\t%d\t%lf\n", comparacoes, tempoExecucao);
        fclose(log);
    }

    return 0;
}