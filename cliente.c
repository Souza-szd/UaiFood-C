#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

#define NUM_RESTAURANTES 3
#define NUM_PRATOS_POR_RESTAURANTE 4
#define NUM_PRATOS NUM_RESTAURANTES*NUM_PRATOS_POR_RESTAURANTE
#define NUM_CLIENTES 2

//Tamanho máximo para qualquer string do programa
#define MAX_STRING_LENGTH 50

typedef struct {
    int codigo;
    char nome[MAX_STRING_LENGTH];
} Restaurante;

typedef struct {
    int codigoPrato;
    int codigoRest;
    char descricao[MAX_STRING_LENGTH];
    float preco;
} Prato;

typedef struct {
    int codigoCliente;
    char nome[MAX_STRING_LENGTH];
} Cliente;

typedef struct {
    int codigoCliente;
    int codigoRest;
    int codigoPrato;
    int indexPrato;
    int quantidade;
} Pedido;

Restaurante restaurantes[NUM_RESTAURANTES];
Prato pratos[NUM_PRATOS];
Cliente clientes[NUM_CLIENTES];
Pedido* pedidos;
int numPedidos = 0;

void removeNewLineFromStrEnd(char* str);
int arrayContainsValue(int* array, int arraySize, int value);

int findIndexOfRestByCode(int codigoRestaurante);
int findIndexOfClientByCode(int codigoCliente);

//Função para cabeçalho
void mostrarCabecalho() {
    printf("**************************************************\n");
    printf("uaiFood - Pedidos\n");
    printf("**************************************************\n");
}

void mostrarRestaurantes() {
    for(int i = 0; i < NUM_RESTAURANTES; i++) {
        if(restaurantes[i].codigo != -1) {
            printf(" %i - %s\n", i+1, restaurantes[i].nome);
        }
    }
}

float resumir_retornarPreco(char* nomeCliente) {
    printf("**************************************************\n");
    printf("Resumo do Pedido - %s\n", nomeCliente);
    printf("**************************************************");

    float precoTotal = 0;
    int numEntregas = 0, restsIncluded[NUM_RESTAURANTES];

    for(int i = 0; i < NUM_RESTAURANTES; i++) {
        restsIncluded[i] = -2;
    }

    for(int j = 0; j < NUM_RESTAURANTES; j++) {
        for(int i = 0; i < numPedidos; i++) {
            if(pedidos[i].codigoRest == restaurantes[j].codigo) {
                int restIndex = findIndexOfRestByCode(pedidos[i].codigoRest);
                int pratoIndex = pedidos[i].indexPrato;

                if(!arrayContainsValue(restsIncluded, NUM_RESTAURANTES, pedidos[i].codigoRest)) {
                    restsIncluded[numEntregas] = pedidos[i].codigoRest;
                    numEntregas++;

                    printf("\n%s\n", restaurantes[restIndex].nome);
                }

                float precoPedido = (float)pedidos[i].quantidade * pratos[pratoIndex].preco;
                precoTotal += precoPedido;
                printf("%ix %s = R$%.2f\n", pedidos[i].quantidade, pratos[pratoIndex].descricao, precoPedido);
            }
        }
    }

    precoTotal += numEntregas*4.99f;

    printf("\nTaxa de Entrega x %i = R$%.2f\n", numEntregas, numEntregas*4.99f);
    printf("---------------------------------------------------------------------\n");
    printf("TOTAL DO PEDIDO = R$%.2f\n\n", precoTotal);
}

//função adicionar restaurante
void add_restaurantes_e_pratos() {
    for (int i = 0; i < NUM_RESTAURANTES; i++) {
        printf("\nCadastre o codigo e o nome do restaurante %d:\n", i + 1);
        
          /*Tirando o scanf do nome do restaurante pois
          ele pode conter espaços*/
        //scanf("%d %49s", &restaurantes[i].codigo, restaurantes[i].nome);
        
        scanf("%i", &restaurantes[i].codigo);
        getchar();

        fgets(restaurantes[i].nome, MAX_STRING_LENGTH, stdin);
        
         //Tirando o "\n" do local do último caractere
        removeNewLineFromStrEnd(restaurantes[i].nome);

        printf("\nCadastre o codigo do prato, codigo do restaurante, descricao e preco dos pratos do restaurante %s:\n", restaurantes[i].nome);

        for (int j = 0; j < NUM_PRATOS_POR_RESTAURANTE; j++) {

            /*Tirando o scanf da descrição pois 
              ela pode conter espaços*/
            //scanf("%d %d %49s %f", &pratos[j].codigoPrato, &pratos[j].codigoRest, pratos[j].descricao, &pratos[j].preco);

            int indexPrato = i * NUM_PRATOS_POR_RESTAURANTE + j;

            scanf("%d %d", &pratos[indexPrato].codigoPrato, &pratos[indexPrato].codigoRest);
            getchar();

            fgets(pratos[indexPrato].descricao, MAX_STRING_LENGTH, stdin);
            
            /*Pegando a última ocorrência do
              caractere de espaço*/

            char* spaceIndex = strrchr(pratos[indexPrato].descricao, ' ');
            
            /*Limitando a descrição até o ultimo caractere de espaço.
              Depois dele, é o preço do prato*/

            *spaceIndex = '\0';

            char precoStr[16];
            strcpy(precoStr, spaceIndex+1);

            sscanf(precoStr, "%f", &pratos[indexPrato].preco);
        }
    }
}

int pagamento(){
    int formaDePagamento;

    printf ("Selecione a forma de pagamento:\n");
    printf ("1 - PIX\n2 - Dinheiro\n3 - Cartao de Credito\n4 - Cartao de Debito\n\n");

    scanf("%i", &formaDePagamento);

    free(pedidos);
    pedidos = NULL;
    numPedidos = 0;

    return formaDePagamento;
}

//função de mostrar pratos do restaurante
void mostrarPratos(int codigoRest) {
    int restIndex = findIndexOfRestByCode(codigoRest);

    if(restIndex == -1)
        return;

    for (int j = 0; j < NUM_PRATOS_POR_RESTAURANTE; j++) {
        int pratoIndex = restIndex * NUM_PRATOS_POR_RESTAURANTE + j;
        Prato prato = pratos[pratoIndex];
        if (prato.codigoPrato != -1){
            printf(" %d - %s - R$%.2f\n", prato.codigoPrato, prato.descricao, prato.preco);
        }
    }
}

//função da tela de login e menu de opcoes
int main() {
    int codigoCliente, opcao, pratoEscolhido, restIndex;

    mostrarCabecalho();
    add_restaurantes_e_pratos();

    for(int i = 0; i < NUM_CLIENTES; i++) {
        printf("\nCadastre o codigo e nome do cliente %i:\n", i+1);
        int codigoCliente;
        char nomeCliente[MAX_STRING_LENGTH];

        scanf("%d", &codigoCliente);
        getchar();

        fgets(nomeCliente, MAX_STRING_LENGTH, stdin);
        removeNewLineFromStrEnd(nomeCliente);

        clientes[i].codigoCliente = codigoCliente;
        strcpy(clientes[i].nome, nomeCliente);
    }

    //Loop de login 
    while (1) {
        voltaraomenu:
        printf("\nLOGIN UAIFOOD PEDIDOS\n");
        printf("\nDigite o codigo do cliente (ou um numero negativo para sair):\n");
        scanf("%d", &codigoCliente);
        
        if (codigoCliente < 0) {
            break; //Sair 
        }
        
        int i = findIndexOfClientByCode(codigoCliente);

        if (i == -1) {
            printf("\nCliente nao encontrado. Tente novamente.\n");
            continue; //volta para o inicio do loop
        }

        //menu de opcoes
        while (1) {
            
            printf("\nBem-vindo %s - Selecione o restaurante:\n", clientes[i].nome);
            mostrarRestaurantes();
            printf(" 0 - Fechar Pedido\n");
            printf("-1 - Logout\n\n");
            scanf("%d", &opcao);

            restIndex = opcao-1;

            if (opcao == -1) {
                free(pedidos);
                numPedidos = 0;
                pedidos = NULL;
                break; // Logout 
            } else if (restIndex >= 0 && restIndex < NUM_RESTAURANTES) {
                
                voltaraospratos:

                printf("\n%s - Pratos - Menu de Opcoes\n", restaurantes[restIndex].nome);
                mostrarPratos(restaurantes[restIndex].codigo);
                printf(" 0 - Fechar Pedido\n");
                printf("-1 - Voltar ao menu anterior\n\n");
                scanf("%d", &pratoEscolhido);

                //Ver detalhes de cada prato ou ir para o menu anterior
                if (pratoEscolhido == -1){
                    continue; //voltar ao menu anterior
                }else if (pratoEscolhido != 0) {
                    int index_prato = (restIndex*4) + pratoEscolhido - 1;
                    
                    printf("\n%s - R$%.2f. Qual a quantidade?\n\n", pratos[index_prato].descricao, pratos[index_prato].preco);

                    int quantidade;
                    scanf("%i", &quantidade);

                    if(pedidos == NULL) {
                        //Primeira alocação
                        pedidos = (Pedido*)malloc(sizeof(Pedido));
                    } else {
                        //Realocação
                        pedidos = (Pedido*)realloc(pedidos, (numPedidos+1)*sizeof(Pedido));
                    }
                    //Verificando se 'pedidos' ainda 
                    //é NULL ou se virou NULL
                    if(pedidos == NULL) {
                        printf("Erro de alocacao!");
                        return 0;
                    }

                    numPedidos++;

                    int lastPedidoIndex = numPedidos-1;
                    pedidos[lastPedidoIndex].codigoCliente = codigoCliente;
                    pedidos[lastPedidoIndex].indexPrato = index_prato;
                    pedidos[lastPedidoIndex].codigoRest = restaurantes[restIndex].codigo;
                    pedidos[lastPedidoIndex].codigoPrato = pratos[index_prato].codigoPrato;
                    pedidos[lastPedidoIndex].quantidade = quantidade;
                   
                    goto voltaraospratos;

                } else {
                    float preco = resumir_retornarPreco(clientes[i].nome);
                    int formaDePagamento = pagamento();
                    continue;
                }
            } else {
                float preco = resumir_retornarPreco(clientes[i].nome);
                int formaDePagamento = pagamento();
                continue;
            }

        }
    }

    free(pedidos);
    
    return 0;
}

void removeNewLineFromStrEnd(char* str) {
    str[strlen(str) - 1] = '\0';
}

int findIndexOfRestByCode(int codigoRestaurante) {
    for(int i = 0; i < NUM_RESTAURANTES; i++) {
        if(codigoRestaurante == restaurantes[i].codigo)
            return i;
    }
    return -1;
}

int findIndexOfClientByCode(int codigoCliente) {
    for(int i = 0; i < NUM_CLIENTES; i++) {
        if(codigoCliente == clientes[i].codigoCliente)
            return i;
    }
    return -1;
}

int arrayContainsValue(int* array, int arraySize, int value) {
    for(int i = 0; i < arraySize; i++) {
        if(array[i] == value)
            return 1;
    }
    return 0;
}
