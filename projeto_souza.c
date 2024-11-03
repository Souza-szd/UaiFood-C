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



int main(){

    Restaurante restaurantes[NUM_RESTAURANTES];
    Comidas pratos[NUM_PRATOS_POR_RESTAURANTE];


    mostrarCabecalho();
    add_restaurante_e_pratos(restaurantes, pratos, NUM_RESTAURANTES, NUM_PRATOS_POR_RESTAURANTE);
     
}
   