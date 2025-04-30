#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SHOWS 2000
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
    *total_itens = 0;
    char* item = strtok(input, ",");
    while (item != NULL && *total_itens < MAX_LIST) {
        while (*item == ' ') item++;
        strcpy(list[*total_itens], item);
        (*total_itens)++;
        item = strtok(NULL, ",");
    }
}

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

void read_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
       // perror("Erro ao abrir o arquivo");
       // printf("Caminho relativo atual não conseguiu abrir o arquivo.\n");
        exit(1);
    }

    char line[2000];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && show_count < MAX_SHOWS) {
        char* fields[12];
        int field_index = 0;
        int inside_quotes = 0;
        char* p = line;
        char* start = line;

        fields[field_index++] = start;

        while (*p) {
            if (*p == '"') {
                inside_quotes = !inside_quotes;
                memmove(p, p + 1, strlen(p));
                continue;
            } else if (*p == ',' && !inside_quotes) {
                *p = '\0';
                if (field_index < 12) {
                    fields[field_index++] = p + 1;
                }
            }
            p++;
        }

        while (field_index < 12) {
            fields[field_index++] = "";
        }

        Show* show = &shows[show_count];
        removeN_R(fields[11]);

        strcpy(show->show_id, strlen(fields[0]) > 0 ? fields[0] : "NaN");
        strcpy(show->type, strlen(fields[1]) > 0 ? fields[1] : "NaN");
        strcpy(show->title, strlen(fields[2]) > 0 ? fields[2] : "NaN");

        split_list(strlen(fields[3]) > 0 ? fields[3] : "NaN", show->director, &show->director_count);
        split_list(strlen(fields[4]) > 0 ? fields[4] : "NaN", show->cast, &show->cast_count);
        split_list(strlen(fields[5]) > 0 ? fields[5] : "NaN", show->country, &show->country_count);

        strcpy(show->date, strlen(fields[6]) > 0 ? fields[6] : "March 1, 1900");
        show->release_year = strlen(fields[7]) > 0 ? atoi(fields[7]) : 0;

        strcpy(show->rating, strlen(fields[8]) > 0 ? fields[8] : "NaN");
        strcpy(show->duration, strlen(fields[9]) > 0 ? fields[9] : "NaN");

        split_list(strlen(fields[10]) > 0 ? fields[10] : "NaN", show->listen_in, &show->listen_count);

        sort_list(show->cast, show->cast_count);
        sort_list(show->listen_in, show->listen_count);

        show_count++;
    }

    fclose(file);
}

Show* buscar(const char* id) {
    for (int i = 0; i < show_count; i++) {
        if (strcmp(shows[i].show_id, id) == 0) {
            return &shows[i];
        }
    }
    return NULL;
}

void insertion_sort(Show array[], int n) {
    for (int i = 1; i < n; i++) {
        Show temp = array[i];
        int j = i - 1;
        while (j >= 0 && strcmp(array[j].show_id, temp.show_id) > 0) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = temp;
    }
}

int pesquisa_binaria(Show array[], int n, const char* id, int* comparacoes) {
    int inicio = 0, fim = n - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(array[meio].show_id, id);
        if (cmp == 0) return 1;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return 0;
}

int main() {
    
    
    read_csv("disneyplus.csv");

    printf("Ola\n");
    setbuf(stdout, NULL);  // desativa o buffer da saída padrão
    
    char entrada[100];
    Show array[300];
    int posArrayShow = 0;

    // Lendo show_id para montar o array
    while (1) {
        fgets(entrada, sizeof(entrada), stdin);
        removeN_R(entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        Show* show = buscar(entrada);
        if (show != NULL) {
            array[posArrayShow++] = *show;
        }
    }

    // Ordenar o array pelo título
    insertion_sort(array, posArrayShow);

    // --- Começar medição de tempo ---
    clock_t inicio = clock();

    // Lendo os títulos e pesquisando
    while (1) {
        fgets(entrada, sizeof(entrada), stdin);
        removeN_R(entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        int comparacoes = 0;
        int encontrado = pesquisa_binaria(array, posArrayShow, entrada, &comparacoes);

        printf("%s\n", encontrado ? "SIM" : "NAO");
    }

    clock_t fim = clock();
    // --- Fim da medição de tempo ---

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    double tempo_mili = tempo * 1000.0;

    // Criar arquivo de log
    FILE* log = fopen("850847_binaria.txt", "w");
    if (log != NULL) {
        fprintf(log, "850847\t%.6f\t%.2f\n", tempo, tempo_mili);
        fclose(log);
    } else {
        printf("Erro ao criar arquivo de log!\n");
    }

    return 0;
}

