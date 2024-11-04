#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>




/*1. 
Todas as funções que serão necessárias para o trabalho:

Funções de Exibição e Interface
mostrarCabecalho(): Exibe o cabeçalho do programa.
mostrarMenuPrincipal(): Exibe o menu principal após o login com opções para o restaurante, como "Mostrar pratos" e "Logout".
mostrarMenuLogin(): Exibe a tela de login, onde o usuário insere o código do restaurante.
mostrarPratosMenu(Restaurante restaurante): Mostra o menu de pratos do restaurante selecionado, listando os pratos e permitindo que o usuário selecione um deles.
mostrarDetalhesPrato(Prato prato): Exibe os detalhes de um prato específico selecionado pelo usuário.


2. Funções de Cadastro
cadastrarRestaurantes(Restaurante restaurantes[]): Solicita ao usuário os dados dos restaurantes e armazena-os no array de Restaurante.
cadastrarPratos(Prato pratos[], Restaurante restaurantes[]): Solicita os dados dos pratos e os armazena no array de Prato, associando cada prato a um restaurante.


3. Funções de Login e Navegação
loginRestaurante(Restaurante restaurantes[], int numRestaurantes): Lê o código do restaurante inserido pelo usuário e verifica se ele existe. Retorna o índice do restaurante logado ou -1 caso o código seja inválido.
menuRestaurante(Restaurante restaurante, Prato pratos[], int numPratos): Gerencia o menu do restaurante após o login, com opções como "Mostrar pratos" e "Logout".


4. Funções de Exibição de Dados
mostrarRestaurantes(Restaurante restaurantes[], int numRestaurantes): Lista todos os restaurantes cadastrados, útil para visualização geral.
mostrarPratos(Restaurante restaurante, Prato pratos[], int numPratos): Lista todos os pratos de um restaurante específico.
mostrarDetalhesPratoPorCodigo(int codigoPrato, int codigoRestaurante, Prato pratos[], int numPratos): Busca um prato pelo código e exibe seus detalhes.


5. Funções de Utilidade e Organização
ordenarRestaurantesPorCodigo(Restaurante restaurantes[], int numRestaurantes): Ordena os restaurantes pelo código para facilitar a busca e visualização.
ordenarPratosPorCodigo(Prato pratos[], int numPratos): Ordena os pratos por código para exibir a lista em uma ordem organizada.
buscarRestaurantePorCodigo(int codigo, Restaurante restaurantes[], int numRestaurantes): Faz uma busca pelo restaurante com o código fornecido e retorna o índice ou -1 caso não seja encontrado.
buscarPratoPorCodigo(int codigoPrato, int codigoRestaurante, Prato pratos[], int numPratos): Busca um prato pelo código e retorna seu índice para facilitar a exibição de detalhes.


6. Função Principal do Programa
main(): Chama as funções na ordem correta para o fluxo do programa (cadastro, login, exibição do menu, e exibição dos pratos e detalhes).
*/

//criar struct para cada restaurante, com código e nome
#define NUM_RESTAURANTES 3
#define NUM_PRATOS_POR_RESTAURANTE 4


typedef struct{
    int codigo;
    char nome[50];
}Restaurante;

typedef struct{
    int codigoPrato;
    int codigoRest;
    char descricao[50];
    float preco;
}Comidas;


//função para cabeçalho
void mostrarCabecalho() {
    printf("**************************************************\n");
    printf("uaiFood - Restaurante\n");
    printf("**************************************************\n");
}

//função adicionar restaurante: vou usar ponteiro pois se eu fizer por valo vai significar que qualquer alteração no restaurante dentro da função não será refletida fora dela.
void add_restaurante_e_pratos(Restaurante restaurantes[], Comidas pratos[], int numeroRestaurantes, int numeroPratosPorRestaurante) {
    for(int i = 0; i < numeroRestaurantes; i++){
    printf("Cadastre o codigo e nome do restaurante %d:\n", i + 1);

    scanf("%d", &restaurantes[i].codigo);
    getchar(); // Consumir a quebra de linha após scanf para evitar problemas com fgets

    fgets(restaurantes[i].nome, sizeof(restaurantes[i].nome), stdin);
    restaurantes[i].nome[strcspn(restaurantes[i].nome, "\n")] = '\0';

     printf("Cadastre codigo do prato, codigo do restaurante, descricao e preco dos pratos do restaurante %s:\n", restaurantes[i].nome);

    for(int j = 0; j < numeroPratosPorRestaurante; j++){
        scanf("%d %d", &pratos[j].codigoPrato, &pratos[j].codigoRest);
        getchar(); // Consumir a quebra de linha após scanf para evitar problemas com fgets

        fgets(pratos[j].descricao, sizeof(pratos[j].descricao), stdin);
        
        //fazer um ponteiro que vai armazenar o endereço no ultimo espaço encontrado
            char* spaceIndex = strrchr(pratos[j].descricao, ' ');

            *spaceIndex = '\0';
            
            char precoStr[16];
            strcpy(precoStr, spaceIndex+1);

            sscanf(precoStr, "%f", &pratos[j].preco);
    }
    }
}
int findIndexOfRestaurante(Restaurante restaurante) {
    for(int i = 0; i < NUM_RESTAURANTES; i++) {
        if(restaurante.codigo == restaurantes[i].codigo)
            return i;
    }
    return -1;
}

//função de mostrar pratos do restaurante
void mostrarPratos(Restaurante restaurante) {
    printf("\nPratos disponiveis no restaurante %s:\n", restaurante.nome);
    for (int j = 0; j < NUM_PRATOS_POR_RESTAURANTE; j++) {
        int restauranteIndex = findIndexOfRestaurante(restaurante);

        if(restauranteIndex != -1) {
            Comidas prato = pratos[restauranteIndex * NUM_PRATOS_POR_RESTAURANTE + j];
            printf("%d - %s - R$%.2f\n", prato.codigoPrato, prato.descricao, prato.preco);
        }
    }
}

//função da tela de login e menu de opcoes
int main() {
    setlocale(LC_ALL, "Portuguese_Brazil");
    int codigoRestaurante, opcao, pratoEscolhido;

    mostrarCabecalho();
    add_restaurantes_e_pratos(restaurantes, pratos, NUM_RESTAURANTES, NUM_PRATOS_POR_RESTAURANTE);

    //loop de login 
    while (1) {
        printf("\nLOGIN UAIFOOD RESTAURANTE\n");
        printf("Digite o codigo do restaurante (ou um numero negativo para sair):\n");
        scanf("%d", &codigoRestaurante);

        if (codigoRestaurante < 0) {
            break; //Sair 
        }
        //Ver se o restaurante existe
        int restauranteEncontrado = 0, i;
        for (i = 0; i < NUM_RESTAURANTES; i++) {
            if (restaurantes[i].codigo == codigoRestaurante) {
                restauranteEncontrado = 1;
                break;
            }
        }

        if (!restauranteEncontrado) {
            printf("Restaurante nao encontrado. Tente novamente.\n");
            continue; //volta para o inicio do loop
        }

        //menu de opcoes
        while (1) {
            printf("%s - Menu de Opcoes\n", restaurantes[i].nome);
            printf("1 - Mostrar pratos\n");
            printf("0 - Logout\n");
            scanf("%d", &opcao);

            if (opcao == 0) {
                break; // Logout
            } else if (opcao == 1) {
                mostrarPratos(restaurantes[i]);

                //escolher prato
                printf("0 - Voltar ao menu anterior\n");
                scanf("%d", &pratoEscolhido);
                if (pratoEscolhido == 0) {
                    continue; //voltar para o outro menu
                }

                //mostrar detalhes do prato escolhido
                int j;
                for (j = 0; j < NUM_RESTAURANTES * NUM_PRATOS_POR_RESTAURANTE; j++) {
                    if (pratos[j].codigoPrato == pratoEscolhido && pratos[j].codigoRest == restaurantes[i].codigo) {
                        printf("Codigo do prato: %d\n", pratos[j].codigoPrato);
                        printf("Codigo do restaurante: %d\n", restaurantes[i].codigo);
                        printf("Nome do Prato: %s\n", pratos[j].descricao);
                        printf("Preco: %.2f\n", pratos[j].preco);
                        break;
                    }
                }
                if (j == NUM_RESTAURANTES * NUM_PRATOS_POR_RESTAURANTE) {
                    printf("Prato nao encontrado.\n");
                }
            }
        }
    }

    return 0;
}
