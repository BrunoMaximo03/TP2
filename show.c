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

// Remove espaços, aspas, \r e \n do início e fim da string
void trim(char* str) {
    while (*str == ' ' || *str == '"' || *str == '\n' || *str == '\r') str++;
    char* start = str;
    memmove(str, start, strlen(start) + 1);

    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '"' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

// Divide uma string com valores separados por vírgulas
void split_list(char* input, char list[MAX_LIST][MAX_FIELD], int* total_itens) {
    *total_itens = 0;
    char* item = strtok(input, ",");
    while (item != NULL && *total_itens < MAX_LIST) {
        while (*item == ' ') item++;
        trim(item);
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
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[2000];
    fgets(line, sizeof(line), file); // Pula cabeçalho

    while (fgets(line, sizeof(line), file) && show_count < MAX_SHOWS) {
        char* campos[12] = { NULL };
        int campo_atual = 0;
        int dentro_de_aspas = 0;
        char* cursor = line;
        campos[0] = cursor;

        while (*cursor) {
            if (*cursor == '"') {
                memmove(cursor, cursor + 1, strlen(cursor));
                continue;
            } else if (*cursor == ',' && !dentro_de_aspas) {
                *cursor = '\0';
                campos[++campo_atual] = cursor + 1;
            }
            cursor++;
        }

        Show* esp = &shows[show_count];

        for (int i = 0; i <= campo_atual; i++) {
            trim(campos[i]);
        }

        strcpy(esp->show_id, campos[0]);
        strcpy(esp->type, campos[1]);
        strcpy(esp->title, campos[2]);

        strcpy(campos[3], strlen(campos[3]) ? campos[3] : "NaN");
        strcpy(campos[4], strlen(campos[4]) ? campos[4] : "NaN");
        strcpy(campos[5], strlen(campos[5]) ? campos[5] : "NaN");
        strcpy(campos[6], strlen(campos[6]) ? campos[6] : "March 1, 1900");
        strcpy(campos[8], strlen(campos[8]) ? campos[8] : "NaN");
        strcpy(campos[9], strlen(campos[9]) ? campos[9] : "NaN");
        strcpy(campos[10], strlen(campos[10]) ? campos[10] : "NaN");

        split_list(campos[3], esp->director, &esp->director_count);
        split_list(campos[4], esp->cast, &esp->cast_count);
        split_list(campos[5], esp->country, &esp->country_count);

        strcpy(esp->date, campos[6]);
        esp->release_year = atoi(campos[7]);
        strcpy(esp->rating, campos[8]);
        strcpy(esp->duration, campos[9]);
        split_list(campos[10], esp->listen_in, &esp->listen_count);

        sort_list(esp->cast, esp->cast_count);
        sort_list(esp->listen_in, esp->listen_count);

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

int main() {
    read_csv("/tmp/disneyplus.csv");

    char entrada[100];
    while (1) {
        fgets(entrada, sizeof(entrada), stdin);
        trim(entrada);
        if (strcmp(entrada, "FIM") == 0) break;

        Show* show = buscar(entrada);
        if (show != NULL) {
            print_show(show);
        }
    }

    return 0;
}
