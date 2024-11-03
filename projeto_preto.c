#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>




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



typedef struct {
    int codigo;
    char nome[50];
}Restaurante;

typedef struct {
    int codigoPrato;
    int codigoRest;
    char descricao[50];
    float preco;
}Comidas;

Restaurante restaurantes[NUM_RESTAURANTES];
Comidas pratos[NUM_RESTAURANTES * NUM_PRATOS_POR_RESTAURANTE];

#define MAX_STRING_LENGTH 50

//função para cabeçalho
void mostrarCabecalho() {
    printf("**************************************************\n");
    printf("uaiFood - Restaurante\n");
    printf("**************************************************\n");
}

void tirarNovaLinhaLastIndex(char* str) {
    str[strlen(str) - 1] = '\0';
}

//função adicionar restaurante
void add_restaurantes_e_pratos(Restaurante restaurantes[], Comidas pratos[], int numeroRestaurantes, int numeroPratosPorRestaurante) {
    for (int i = 0; i < numeroRestaurantes; i++) {
        printf("\nCadastre o codigo e nome do restaurante %d:\n", i + 1);

        
          /*Tirando o scanf do nome do restaurante pois
          ele pode conter espaços*/
        //scanf("%d %49s", &restaurantes[i].codigo, restaurantes[i].nome);
        
        scanf("%i", &restaurantes[i].codigo);
        getchar();

        fgets(restaurantes[i].nome, MAX_STRING_LENGTH, stdin);
        
         //Tirando o "\n" do local do ultimo caractere
        tirarNovaLinhaLastIndex(restaurantes[i].nome);

        printf("\nCadastre codigo do prato, codigo do restaurante, descricao e preco dos pratos do restaurante %s:\n", restaurantes[i].nome);

        for (int j = 0; j < numeroPratosPorRestaurante; j++) {

            /*Tirando o scanf da descrição pois 
              ela pode conter espaços*/
            //scanf("%d %d %49s %f", &pratos[j].codigoPrato, &pratos[j].codigoRest, pratos[j].descricao, &pratos[j].preco);

            scanf("%d %d", &pratos[i * numeroPratosPorRestaurante + j].codigoPrato, &pratos[i * numeroPratosPorRestaurante + j].codigoRest);
            getchar();

             fgets(pratos[i * numeroPratosPorRestaurante + j].descricao, MAX_STRING_LENGTH, stdin);
            
            /*Pegando a última ocorrência do
              caractere de espaço*/

            char* spaceIndex = strrchr(pratos[i * numeroPratosPorRestaurante + j].descricao, ' ');
            
            /*Limitando a descrição até o ultimo caractere de espaço.
              Depois dele, é o preço do prato*/

            *spaceIndex = '\0';

            char precoStr[16];
            strcpy(precoStr, spaceIndex+1);

            sscanf(precoStr, "%f", &pratos[i * numeroPratosPorRestaurante + j].preco);
        }
    }
}

//função de mostrar pratos do restaurante
void mostrarPratos(Restaurante restaurante) {
    printf("\nPratos disponiveis no restaurante %s:\n", restaurante.nome);
    for (int j = 0; j < NUM_PRATOS_POR_RESTAURANTE; j++) {
        Comidas prato = pratos[restaurante.codigo * NUM_PRATOS_POR_RESTAURANTE + j];
        printf("%d - %s - R$%.2f\n", prato.codigoPrato, prato.descricao, prato.preco);
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
