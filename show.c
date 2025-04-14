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

void split_list(char* input, char list[MAX_LIST][MAX_FIELD], int* total_itens) {
    //Contador elementos da lista, iniciado em 0
    *count = 0;
    
    //Pega o primeiro pedaco da string, até a virgula   
    char* item = strtok(input, ",");
    while (item != NULL && *total_itens < MAX_LIST) {
        while (*item == ' ') item++;  // remove espaços inicias

        strcpy(list[*total_itens], item);
        (*total_itens)++;
        
        //Pega o proximo item da string
        item = strtok(NULL, ","); 
    }
}

//Ordenacao
void sort_list(char list[MAX_LIST][MAX_FIELD], int count) {
    for (int i = 0; i < count - 1; i++) {
        int min = i;
        for (int j = i + 1; j < count; j++) {
            if (strcmp(list[j], list[min]) < 0) {
                min = j;
            }
        }
        if (min != i) {
            char temp[MAX_FIELD];
            strcpy(temp, list[i]);
            strcpy(list[i], list[min]);
            strcpy(list[min], temp);
        }
    }
}

//Print o show todo
void print_show(const Show* s) {
    printf("=> %s ## %s ## %s ## ", s->show_id, s->title, s->type);
    for (int i = 0; i < s->director_count; i++) {
        printf("%s", s->director[i]);
        if (i < s->director_count - 1) printf(", ");
    }
    printf(" ## [");
    for (int i = 0; i < s->cast_count; i++) {
        printf("%s", s->cast[i]);
        if (i < s->cast_count - 1) printf(", ");
    }
    printf("] ## ");
    for (int i = 0; i < s->country_count; i++) {
        printf("%s", s->country[i]);
        if (i < s->country_count - 1) printf(", ");
    }
    printf(" ## %s ## %d ## %s ## %s ## [", s->date[0] ? s->date : "March 1, 1900",
        s->release_year ? s->release_year : 0, s->rating, s->duration);
    for (int i = 0; i < s->listen_count; i++) {
        printf("%s", s->listen_in[i]);
        if (i < s->listen_count - 1) printf(", ");
    }
    printf("] ##\n");
}

//Le o arquivo CSV
void read_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[2000];
    fgets(line, sizeof(line), file); // pular cabeçalho

    while (fgets(linha, sizeof(linha), arquivo) && show_count < MAX_SHOWS) {
        char* campos[12];        // Vetor para armazenar os 11 campos do CSV + 1 extra se precisar
        int campo_atual = 0;     // Índice do campo atual sendo processado
        int dentro_de_aspas =    // Flag para verificar se está dentro de aspas (campos com vírgulas)
        char* cursor = linha     // Ponteiro para percorrer os caracteres da linha
    
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
        strcpy(espaço->type, campos[1]);
        strcpy(espetaculo->title, campos[2]);
    
        // Separar listas (diretor, elenco, países, línguas)
        split_list(campos[3], espetaculo->director, & espetaculo->director_count);
        split_list(campos[4], espetaculo->cast, & espetaculo->cast_count);
        split_list(campos[5], espetaculo->country, & espetaculo->country_count);
    
        // Outros campos
        strcpy(espetaculo->date, campos[6]);
        espetaculo->release_year = atoi(campos[7]); // Converte ano para inteiro
        strcpy(espetaculo->rating, campos[8]);
        strcpy(espetaculo->duration, campos[9]);
    
        split_list(campos[10], espetaculo->listen_in, & espetaculo->listen_count);
    
        // Ordena cast e línguas
        sort_list(espetaculo->cast, espetaculo->cast_count);
        sort_list(espetaculo->listen_in, espetaculo->listen_count);
    
        show_count++; // Próximo show
    }
    
    fclose(arquivo);
    
}


//Busca pelo id fornecido
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
        trim_newline(entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        Show* show = buscar(entrada);
        if (show != NULL) {
            print_show(s);
        }
    }

    return 0;
}
