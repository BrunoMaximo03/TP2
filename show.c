#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHOWS 1000
#define MAX_FIELD 300
#define MAX_LIST 20

typedef struct {
    char show_id[MAX_FIELD];
    char type[MAX_FIELD];
    char title[MAX_FIELD];
    char director[MAX_LIST][MAX_FIELD];
    int director_count;
    char cast[MAX_LIST][MAX_FIELD];
    int cast_count;
    char country[MAX_LIST][MAX_FIELD];
    int country_count;
    char date[MAX_FIELD];
    int release_year;
    char rating[MAX_FIELD];
    char duration[MAX_FIELD];
    char listen_in[MAX_LIST][MAX_FIELD];
    int listen_count;
} Show;

Show shows[MAX_SHOWS];
int show_count = 0;

void removeN_R(char* str) {
    int len = strlen(str);
    if (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r'))
        str[len-1] = '\0';
}

// Divide uma string com valores separados por vírgulas
void split_list(char* input, char list[MAX_LIST][MAX_FIELD], int* total_itens) {
    *total_itens = 0;
    
    // Pega o primeiro pedaço da string, até a vírgula   
    char* item = strtok(input, ",");
    while (item != NULL && *total_itens < MAX_LIST) {
        while (*item == ' ') item++;  // remove espaços iniciais

        strcpy(list[*total_itens], item);
        (*total_itens)++;
        
        // Pega o próximo item da string
        item = strtok(NULL, ","); 
    }
}

// Ordenação
void sort_list(char list[MAX_LIST][MAX_FIELD], int items) {
    for (int i = 0; i < items - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < items; j++) {
            if (strcmp(list[j], list[menor]) < 0) {
                menor = j;
            }
        }
        if (menor != i) {
            char temp[MAX_FIELD];
            strcpy(temp, list[i]);
            strcpy(list[i], list[menor]);
            strcpy(list[menor], temp);
        }
    }
}

// Print do show todo
void print_show(const Show* show) {
    printf("=> %s ## %s ## %s ## ", show->show_id, show->title, show->type);
    for (int i = 0; i < show->director_count; i++) {
        printf("%s", show->director[i]);
        if (i < show->director_count - 1) printf(", ");
    }
    printf(" ## [");
    for (int i = 0; i < show->cast_count; i++) {
        printf("%s", show->cast[i]);
        if (i < show->cast_count - 1) printf(", ");
    }
    printf("] ## ");
    for (int i = 0; i < show->country_count; i++) {
        printf("%s", show->country[i]);
        if (i < show->country_count - 1) printf(", ");
    }
    printf(" ## %s ## %d ## %s ## %s ## [", show->date[0] ? show->date : "March 1, 1900",
        show->release_year ? show->release_year : 0, show->rating, show->duration);
    for (int i = 0; i < show->listen_count; i++) {
        printf("%s", show->listen_in[i]);
        if (i < show->listen_count - 1) printf(", ");
    }
    printf("] ##\n");
}

// Lê o arquivo CSV
void read_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[2000];
    fgets(line, sizeof(line), file); // pular cabeçalho

    while (fgets(line, sizeof(line), file) && show_count < MAX_SHOWS) {
        char* campos[12];        // Vetor para armazenar os 11 campos do CSV + 1 extra se precisar
        int campo_atual = 0;     // Índice do campo atual sendo processado
        int dentro_de_aspas = 0; // Flag para verificar se está dentro de aspas (campos com vírgulas)
        char* cursor = line;    // Ponteiro para percorrer os caracteres da linha
    
        campos[0] = cursor;      // Primeiro campo começa no início da linha
    
        // Separar os campos com base em vírgulas fora de aspas
        while (*cursor) {
            if (*cursor == '"') {
                dentro_de_aspas = !dentro_de_aspas; // Alternar estado ao encontrar aspas
            }
            else if (*cursor == ',' && !dentro_de_aspas) {
                *cursor = '\0';                        // Finaliza a string do campo atual
                campos[++campo_atual] = cursor + 1;    // Próximo campo começa depois da vírgula
            }
            cursor++;
        }
    
        Show* espetaculo = &shows[show_count]; // Referência para o próximo espaço disponível no array de shows
    
        removeN_R(campos[campo_atual]); // Remove '\n' ou '\r' do último campo
    
        // Copiar dados simples
        strcpy(espetaculo->show_id, campos[0]);
        strcpy(espetaculo->type, campos[1]);
        strcpy(espetaculo->title, campos[2]);
    
        // Separar listas (diretor, elenco, países, línguas)
        split_list(campos[3], espetaculo->director, &espetaculo->director_count);
        split_list(campos[4], espetaculo->cast, &espetaculo->cast_count);
        split_list(campos[5], espetaculo->country, &espetaculo->country_count);
    
        // Outros campos
        strcpy(espetaculo->date, campos[6]);
        espetaculo->release_year = atoi(campos[7]); // Converte ano para inteiro
        strcpy(espetaculo->rating, campos[8]);
        strcpy(espetaculo->duration, campos[9]);
    
        split_list(campos[10], espetaculo->listen_in, &espetaculo->listen_count);
    
        // Ordena cast e línguas
        sort_list(espetaculo->cast, espetaculo->cast_count);
        sort_list(espetaculo->listen_in, espetaculo->listen_count);
    
        show_count++; // Próximo show
    }
    
    fclose(file);
}

// Busca pelo id fornecido
Show* buscar(const char* id) {
    for (int i = 0; i < show_count; i++) {
        if (strcmp(shows[i].show_id, id) == 0) {
            return &shows[i];
        }
    }
    return NULL;
}

int main() {
    read_csv("/tmp/disneyplus.csv");

    char entrada[100];
    while (1) {
        fgets(entrada, sizeof(entrada), stdin);
        removeN_R(entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        Show* show = buscar(entrada);
        if (show != NULL) {
            print_show(show);
        }
    }

    return 0;
}
