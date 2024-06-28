#include <stdio.h> //Importa uma função padrão de entrada e saída
#include <stdlib.h>//Importa uma função padrão de funções utilitárias
#include <string.h>//Importa uma função padrão de manipulação

#define MEMORIA_MAXIMA 100  //Define tamanho máximo de memória 
#define COMMAND_SIZE 50 //Define o tamanho máximo de um comando

//Definição de um bloco de memória
typedef struct Block {
    int size;           //Tamanho do bloco
    int is_free;        //Indica se o bloco está livre (1) ou ocupado (0)
    struct Block *next; //Ponteiro para o próximo bloco
} Block;

Block *head = NULL; //Ponteiro global no início da lista

// Função para inicializar a memória com um bloco único de espaço MEMORIA_MAXIMA e livre
void initialize_memory() {
    head = (Block *)malloc(sizeof(Block));  //Aloca memória para o bloco inicial
    head->size = MEMORIA_MAXIMA;            //Define o tamanho do bloco inicial
    head->is_free = 1;                      //Marca o bloco inicial como livre
    head->next = NULL;                      //Define que não há próximo bloco
}

//Função que exibe todas as funções existentes para o usuário
void help() {
    printf("Comandos disponiveis:\n");
    printf("  help                 - Exibe esta mensagem de ajuda.\n");
    printf("  adicionar <size>     - Adiciona um bloco de memoria de tamanho <tamanho>.\n");
    printf("  remover <size>       - Remove um bloco de memoria de tamanho <tamanho>.\n");
    printf("  realocar             - Realoca a memoria para otimizar espaco.\n");
    printf("  limpar               - Limpa toda a memoria.\n");
    printf("  visualizar           - Exibe o estado atual dos blocos de memoria.\n");
    printf("  sair                 - Sai do programa.\n");
}

//Função que adiciona um bloco de memória 
void adicionar(int size) {
    Block *current = head;  // Ponteiro que percorre a lista de blocos
    Block *best_fit = NULL; // Ponteiro para o melhor bloco encontrado que pode acomodar o novo bloco

// Procura o bloco livre com Best Fit (menor tamanho suficiente)
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
        }
        current = current->next;
    }
    
// Se encontrou um bloco adequado, realiza a alocação
    if (best_fit != NULL) {
        if (best_fit->size > size) {

            // Se o bloco é maior que o necessário, cria um novo bloco com o espaço restante
            Block *new_block = (Block *)malloc(sizeof(Block));
            new_block->size = best_fit->size - size;
            new_block->is_free = 1;
            new_block->next = best_fit->next;
            best_fit->next = new_block;
        }
        best_fit->size = size; // Ajusta o tamanho do bloco para o tamanho solicitado
        best_fit->is_free = 0; // Marca o bloco como ocupado (0)
        printf("Memoria de tamanho %d adicionada.\n", size);
    } else {
        // Se não encontrou um bloco adequado, informa que não há espaço suficiente
        printf("Nao foi possivel adicionar memoria de tamanho %d. Sem espaco suficiente.\n", size);
    }
}

// Função para remover um bloco de memória
void remover(int size) {
    Block *current = head;
    while (current != NULL) {
        if (!current->is_free && current->size == size) {
            current->is_free = 1; // Marca o bloco como livre(1)
            printf("Memória de tamanho %d removida.\n", size);
            return;
        }
        current = current->next;
    }
    // Se não encontrou um bloco do tamanho especificado, informa ao usuário
    printf("Nao foi encontrado bloco de memoria de tamanho %d.\n", size);
}

//Função de realocar memória, acha todos os blocos livres e os une em um só
void realocar() {
    Block *current = head;
    int total_free_size = 0;

    // Calcula o tamanho total de todos os blocos livres e remove-os
    Block *prev = NULL;
    while (current != NULL) {
        if (current->is_free) {
            total_free_size += current->size;
            if (prev != NULL) {
                prev->next = current->next;
                free(current);
                current = prev->next;
            } else {
                head = current->next;
                free(current);
                current = head;
            }
        } else {
            prev = current;
            current = current->next;
        }
    }

    // Se houve blocos livres, cria um novo bloco grande
    if (total_free_size > 0) {
        Block *new_block = (Block *)malloc(sizeof(Block));
        new_block->size = total_free_size;
        new_block->is_free = 1;
        new_block->next = head;
        head = new_block;
    }

    printf("Memória realocada.\n");
}

//Função que limpa a memória 
void limpar() {
    Block *current = head;
    while (current != NULL) {
        Block *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    initialize_memory();//Reinicializa a memória após limpar
    printf("Memoria limpa.\n");
}

//Função que visualiza os blocos de memória
void visualizar() {
    Block *current = head;
    int index = 0;
    printf("Estado atual da memória:\n");
    while (current != NULL) {
        printf("Bloco %d: Tamanho = %d, %s\n", index, current->size, current->is_free ? "Livre" : "Ocupado");
        current = current->next;
        index++;
    }
}

// Função que processa os comandos do usuário
void process_command(char *command) {
    char *token = strtok(command, " ");
    if (strcmp(token, "help") == 0) {
        help();
    } else if (strcmp(token, "adicionar") == 0) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            int size = atoi(token);
            adicionar(size);
        } else {
            printf("Uso: adicionar <tamanho>\n");
        }
    } else if (strcmp(token, "remover") == 0) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            int size = atoi(token);
            remover(size);
        } else {
            printf("Uso: remover <tamanho>\n");
        }
    } else if (strcmp(token, "realocar") == 0) {
        realocar();
    } else if (strcmp(token, "limpar") == 0) {
        limpar();
    } else if (strcmp(token, "visualizar") == 0) {
        visualizar();
    } else if (strcmp(token, "sair") == 0) {
        printf("Saindo do programa.\n");
        exit(0);
    } else {
        printf("Comando desconhecido: %s\n", token);
        help();
    }
}

//Função que roda ao iniciar o programa
int main() {
    initialize_memory(); // Inicializa a memória ao iniciar o programa

    char command[COMMAND_SIZE];
    while (1) {
        printf("Digite um comando: ");
        if (fgets(command, COMMAND_SIZE, stdin) != NULL) {
            command[strcspn(command, "\n")] = '\0';  // Limpa a entrada de usuário para não existir caracteres de nova linha como \n
            process_command(command);
        }
    }

    return 0;
}