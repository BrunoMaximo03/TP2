#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define TAMANHO_MAX_LINHA 1024
#define TAMANHO_MAX_CATALOGO 10000
#define TAMANHO_MAX_LISTA 50

// Struct Date
typedef struct Date {
    char mes[20];
    int dia;
    int ano;
} Date;

// Struct Show
typedef struct Show {
    char* id;
    char* tipo;
    char* titulo;
    char* diretor;
    char** elenco;       
    int tamanhoElenco;      
    char* pais;
    Date dataAdicionado;   
    int anoLancamento;
    char* classificacao;
    char* duracao;
    char** categorias; 
    int tamanhoCategorias;
    char* descricao;
} Show;

Show* show[TAMANHO_MAX_CATALOGO];
int totalShows = 0;

// Variáveis para o log
int totalComparacoes = 0;
int totalMovimentacoes = 0;

// Remove espaços em branco da string
char* limparTexto(char* str) {
    if (!str) return strdup("NaN");
    while (isspace(*str)) str++;
    char* fim = str + strlen(str) - 1;
    while (fim > str && isspace(*fim)) fim--;
    *(fim + 1) = '\0';
    return strdup(str);
}

// Remove aspas de uma string
char* removerAspasTexto(char* str) {
    if (!str) return NULL;
    char* resultado = malloc(strlen(str) + 1);
    if (!resultado) {
        //fprintf(stderr, "Erro ao alocar memória.\n");
        exit(1);
    }
    int j = 0;  
    for (int i = 0; str[i]; i++) {
        if (str[i] != '"') {
            resultado[j++] = str[i];
        }
    }
    resultado[j] = '\0';
    return resultado;
}

// Processa um campo removendo aspas e espaços
char* removeAspasEspaco(char* campo) {
    if (!campo || strlen(campo) == 0) return strdup("NaN");
    char* semAspas = removerAspasTexto(campo);
    char* resultado = limparTexto(semAspas);
    free(semAspas);
    return resultado;
}

// Processa listas separadas por vírgula e ordena
char** tratarLista(char* str, int* tamanho) {
    char** itens = malloc(TAMANHO_MAX_LISTA * sizeof(char*));
    if (!itens) {
        //fprintf(stderr, "Erro ao alocar memória para a lista.\n");
        exit(1);
    }
    *tamanho = 0;

    if (!str || strlen(str) == 0) {
        itens[(*tamanho)++] = strdup("NaN");
        return itens;
    }

    char* limpo = removerAspasTexto(str);
    char* token = strtok(limpo, ",");
    while (token != NULL && *tamanho < TAMANHO_MAX_LISTA) {
        char* item = removeAspasEspaco(token);
        itens[(*tamanho)++] = item;
        token = strtok(NULL, ",");
    }

    free(limpo);

    // Ordena a lista
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

// Separa os campos de uma linha
int dividirCampos(char* linha, char* campos[], int maxCampos) {
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
            if (!campos[i]) {
                //fprintf(stderr, "Erro ao alocar memória.\n");
                exit(1);
            }
            strncpy(campos[i], inicio, len);
            campos[i][len] = '\0';
            if (*ptr == '"') ptr++;
            if (*ptr == ',') ptr++;
        } else {
            char* inicio = ptr;
            while (*ptr && *ptr != ',') ptr++;
            int len = ptr - inicio;
            campos[i] = malloc(len + 1);
            if (!campos[i]) {
                //fprintf(stderr, "Erro ao alocar memória.\n");
                exit(1);
            }
            strncpy(campos[i], inicio, len);
            campos[i][len] = '\0';
            if (*ptr == ',') ptr++;
        }
        i++;
    }
    return i;
}

// Preenche o catálogo
void readCSV() {
    FILE* arquivo = fopen("/tmp/disneyplus.csv", "r");
    if (arquivo == NULL) {
        //printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[TAMANHO_MAX_LINHA];
    fgets(linha, TAMANHO_MAX_LINHA, arquivo); // Ignora o cabeçalho

    while (fgets(linha, TAMANHO_MAX_LINHA, arquivo)) {
        linha[strcspn(linha, "\n")] = 0; // Remove o caractere de nova linha

        char* campos[15];
        int quantidadeCampos = dividirCampos(linha, campos, 15);

        Show* novoShow = malloc(sizeof(Show));
        if (!novoShow) {
            //  fprintf(stderr, "Erro ao alocar memória para novo show.\n");
            exit(1);
        }
        novoShow->id = removeAspasEspaco(campos[0]);
        novoShow->tipo = removeAspasEspaco(campos[1]);
        novoShow->titulo = removeAspasEspaco(campos[2]);
        novoShow->diretor = removeAspasEspaco(campos[3]);
        novoShow->elenco = tratarLista(campos[4], &novoShow->tamanhoElenco);
        novoShow->pais = removeAspasEspaco(campos[5]);

        // Trata campo de data
        char* dataStr = removeAspasEspaco(campos[6]);
        if (strcmp(dataStr, "NaN") != 0) {
            char* parte = strtok(dataStr, " ");
            strcpy(novoShow->dataAdicionado.mes, parte ? parte : "NaN");
            parte = strtok(NULL, ",");
            novoShow->dataAdicionado.dia = parte ? atoi(parte) : -1;
            parte = strtok(NULL, "");
            novoShow->dataAdicionado.ano = parte ? atoi(parte) : -1;
        } else {
            strcpy(novoShow->dataAdicionado.mes, "March");
            novoShow->dataAdicionado.dia = 1;
            novoShow->dataAdicionado.ano = 1900;
        }
        free(dataStr);

        novoShow->anoLancamento = (quantidadeCampos > 7 && strlen(campos[7]) > 0) ? atoi(campos[7]) : -1;
        novoShow->classificacao = removeAspasEspaco(campos[8]);
        novoShow->duracao = removeAspasEspaco(campos[9]);
        novoShow->categorias = tratarLista(campos[10], &novoShow->tamanhoCategorias);
        novoShow->descricao = removeAspasEspaco(campos[11]);

        for (int i = 0; i < quantidadeCampos; i++) free(campos[i]);
        show[totalShows++] = novoShow;
    }

    fclose(arquivo);
}

// Imprime os detalhes de um show
void printShow(Show* showAtual) {
    printf("=> %s ## %s ## %s ## %s ## [", showAtual->id, showAtual->titulo, showAtual->tipo, showAtual->diretor);
    for (int i = 0; i < showAtual->tamanhoElenco; i++) {
        printf("%s%s", showAtual->elenco[i], (i < showAtual->tamanhoElenco - 1) ? ", " : "");
    }
    printf("] ## %s ## %s %d, %d ## %d ## %s ## %s ## [", showAtual->pais, showAtual->dataAdicionado.mes, showAtual->dataAdicionado.dia,
           showAtual->dataAdicionado.ano, showAtual->anoLancamento, showAtual->classificacao, showAtual->duracao);
    for (int i = 0; i < showAtual->tamanhoCategorias; i++) {
        printf("%s%s", showAtual->categorias[i], (i < showAtual->tamanhoCategorias - 1) ? ", " : "");
    }
    printf("] ##\n");
}

// Verifica se a entrada é "FIM"
bool IsFim(char* str) {
    return strcasecmp(str, "FIM") == 0;
}

// Algoritmo de ordenação por seleção recursiva
void ordenarSelecaoRecursiva(Show* array[], int n, int i) {
    if (i >= n - 1) return;

    int menor = i;
    for (int j = i + 1; j < n; j++) {
        totalComparacoes++;
        if (strcasecmp(array[j]->titulo, array[menor]->titulo) < 0) {
            menor = j;
        }
    }
    if (menor != i) {
        Show* temp = array[i];
        array[i] = array[menor];
        array[menor] = temp;
        totalMovimentacoes++;
    }
    ordenarSelecaoRecursiva(array, n, i + 1);
}

// Main
int main() {
    readCSV(); 

    char entrada[50];
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = 0;

    Show* resultados[TAMANHO_MAX_CATALOGO];
    int tamanhoResultados = 0;

    while (!IsFim(entrada)) {
        for (int i = 0; i < totalShows; i++) {
            if (strcasecmp(show[i]->id, entrada) == 0) {
                resultados[tamanhoResultados++] = show[i];
                break;
            }
        }
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;
    }

    clock_t inicio = clock();

    ordenarSelecaoRecursiva(resultados, tamanhoResultados, 0);

    clock_t fim = clock();

    for (int i = 0; i < tamanhoResultados; i++) {
        printShow(resultados[i]);
    }

    double tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    FILE* log = fopen("matricula_selecao.txt", "w");
    if (log) {
        fprintf(log, "850847\t%d\t%d\t%lf\n", totalComparacoes, totalMovimentacoes, tempoExecucao);
        fclose(log);
    }

    return 0;
}