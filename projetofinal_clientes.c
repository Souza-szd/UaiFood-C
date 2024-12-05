#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

//Tamanho máximo para qualquer string do programa
#define MAX_STRING_LENGTH 50
#define MAX_LINE_LENGTH 2 * MAX_STRING_LENGTH

#define PEDIDOS_END_SUFFIX "[END]"

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
    int quantidade;
} Pedido;

Restaurante* restaurantes = NULL;
Prato* pratos = NULL;
Cliente* clientes = NULL;
Pedido* pedidos = NULL;

int numRests = 0,
numPratos = 0,
numClientes = 0,
numPedidos = 0;

#define restsTxt "restaurantes.txt"
#define pratosTxt "pratos.txt"
#define pedidosTxt "pedidos.txt"
#define clientesTxt "clientes.txt"

void allocerror();

void removeNewLineFromStrEnd(char* str);

int findIndexOfRestByCode(int codigoRestaurante);
int findIndexOfPratoByItsCodeAndRestCode(int pratoCode, int restCode);
int findIndexOfClientByCode(int codigoCliente);

int deleteCliente(int clienteIndex);
int createNewCliente();
int saveClientes();
int readClientes();
int readRests();
int readPratos();

int arrayContainsValue(int* array, int arraySize, int value);

//Função para cabeçalho
void mostrarCabecalho() {
    printf("\n**************************************************\n");
    printf("uaiFood - Pedidos\n");
    printf("**************************************************\n\n");
}

void mostrarRestaurantes() {
    readRests();
    for(int i = 0; i < numRests; i++) {
        printf(" %i - %s\n", i+1, restaurantes[i].nome);
    }
}

void renomearCliente(Cliente *cliente) {
    getchar();
    printf("\nDigite o novo nome:\n");
    fgets(cliente->nome, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(cliente->nome);

    printf("\nO novo nome eh: %s\n", cliente->nome);

    saveClientes();
}

void getClienteCodeStr(int clienteCode, char* codeStr) {
    strcpy(codeStr, "[");
    char tempStr[12];
    sprintf(tempStr, "%i", clienteCode);
    strcat(codeStr, tempStr);
    strcat(codeStr, "]");
}

int getBiggestPedidoCode() {
    FILE* fPedidos = fopen(pedidosTxt, "r");

    if(fPedidos == NULL) {
        printf("\nErro na Abertura do Arquivo \"%s\"!\n", pedidosTxt);
        return -1;
    }

    const char PEDIDO_CODE_PREFIX[] = "Codigo do Pedido: ";

    char line[MAX_LINE_LENGTH];

    int biggestPedidoCode = 0;

    while(!feof(fPedidos)) {
        fgets(line, MAX_LINE_LENGTH, fPedidos);

        //Checando se linha começa com "Codigo do Pedido: "
        if(!strncmp(line, PEDIDO_CODE_PREFIX, strlen(PEDIDO_CODE_PREFIX))) {
            int pedidoCode;
            sscanf(line, "Codigo do Pedido: %i", &pedidoCode);

            if(pedidoCode > biggestPedidoCode)
                biggestPedidoCode = pedidoCode;
        }
    }

    fclose(fPedidos);

    return biggestPedidoCode;
}

int mostrarHistorico(int clienteCode) {
    FILE* fPedidos = fopen(pedidosTxt, "r");

    if(fPedidos == NULL) {
        printf("\nErro na Abertura do Arquivo \"%s\"!\n", pedidosTxt);
        return -1;
    }

    printf("\n---------------------------------------------------------------------\n\n");

    char codeStr[16];
    //Definindo codeStr como "[CODIGO_DO_CLIENTE]"
    getClienteCodeStr(clienteCode, codeStr);

    char line[MAX_LINE_LENGTH];

    int allowedToPrint = 0;

    int jaFezPeloMenosUmPedido = 0;

    while(!feof(fPedidos)) {
        fgets(line, MAX_LINE_LENGTH, fPedidos);

        //Checando se linha é "[END]"
        if(!strncmp(line, PEDIDOS_END_SUFFIX, strlen(PEDIDOS_END_SUFFIX))) {
            allowedToPrint = 0;
            continue;
        }
        
        if(allowedToPrint)
            printf("%s", line);

        //Checando se linha é "[CODIGO_DO_CLIENTE]"
        if(!strncmp(line, codeStr, strlen(codeStr))) {
            allowedToPrint = 1;
            jaFezPeloMenosUmPedido = 1;
        }
    }

    if(!jaFezPeloMenosUmPedido) {
        printf("Nenhum Pedido Foi Feito Ainda!\n");
    }

    fclose(fPedidos);

    return 1;
}

void print(FILE* file, char* str) {
    printf("%s", str);
    fprintf(file, "%s", str);
}

float resumir_retornarPreco(int clienteIndex) {

    char* nomeCliente = clientes[clienteIndex].nome;

    //Definindo numPedidos como o número de pedidos feitos até agora
    //getNumPedidos();

    int biggestPedidoCode = getBiggestPedidoCode();

    printf("\n**************************************************\n");
    printf("Resumo do Pedido - %s\n", nomeCliente);
    printf("**************************************************\n");

    float precoTotal = 0;
    int numEntregas = 0, restsIncluded[numRests];

    for(int i = 0; i < numRests; i++) {
        restsIncluded[i] = -2;
    }



    FILE* fPedidos = fopen(pedidosTxt, "a");

    if(fPedidos == NULL) {
        printf("\nErro na Abertura do Arquivo \"%s\"!\n", pedidosTxt);
        return -1;
    }

    fprintf(fPedidos, "\n[%i]\nCodigo do Pedido: %i\n",
            clientes[clienteIndex].codigoCliente,
            biggestPedidoCode+1);


    char line[MAX_LINE_LENGTH];

    for(int j = 0; j < numRests; j++) {
        for(int i = 0; i < numPedidos; i++) {
            if(pedidos[i].codigoRest == restaurantes[j].codigo) {
                int restIndex = findIndexOfRestByCode(pedidos[i].codigoRest);
                int pratoIndex = findIndexOfPratoByItsCodeAndRestCode(pedidos[i].codigoPrato, pedidos[i].codigoRest);

                if(!arrayContainsValue(restsIncluded, numRests, pedidos[i].codigoRest)) {
                    restsIncluded[numEntregas] = pedidos[i].codigoRest;
                    numEntregas++;

                    sprintf(line, "\nRestaurante %s\n", restaurantes[restIndex].nome);
                    print(fPedidos, line);
                }

                float precoPedido = (float)pedidos[i].quantidade * pratos[pratoIndex].preco;
                precoTotal += precoPedido;
                sprintf(line, "%ix %s = R$%.2f\n", pedidos[i].quantidade, pratos[pratoIndex].descricao, precoPedido);
                print(fPedidos, line);
            }
        }
    }

    precoTotal += numEntregas*4.99f;

    sprintf(line, "\nTaxa de Entrega x %i = R$%.2f\n\n", numEntregas, numEntregas*4.99f);
    print(fPedidos, line);
    sprintf(line, "TOTAL DO PEDIDO = R$%.2f\n", precoTotal);
    print(fPedidos, line);
    sprintf(line, "---------------------------------------------------------------------\n\n");
    print(fPedidos, line);

    fprintf(fPedidos,"%s", PEDIDOS_END_SUFFIX);

    fclose(fPedidos);

    return precoTotal;
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

void fecharPedido(int clienteIndex) {
    float preco = resumir_retornarPreco(clienteIndex);
    int formaDePagamento = pagamento();
}

//função de mostrar pratos do restaurante
int mostrarPratos(int codigoRest) {
    int restIndex = findIndexOfRestByCode(codigoRest);

    if(restIndex == -1)
        return 0;

    readPratos();

    int numPratosInThisRest = 0;
    for(int i = 0; i < numPratos; i++){
        if(pratos[i].codigoRest == codigoRest)
            numPratosInThisRest++;
    }

    if(numPratosInThisRest == 0) {
        printf("\nAinda Nao Existem Pratos Disponiveis!\n");
        return 0;
    }

    printf("\nPratos disponiveis no restaurante %s:\n", restaurantes[restIndex].nome);
    for (int j = 0; j < numPratos; j++) {
        if (pratos[j].codigoRest == codigoRest) {
            printf(" %i - %s - R$%.2f\n", pratos[j].codigoPrato, pratos[j].descricao, pratos[j].preco);
        }
    }

    return 1;
}

//função da tela de login e menu de opcoes
int login() {
    int codigoCliente, opcao, pratoEscolhido, restIndex;

    int readStatusCode = readClientes();

    if(readStatusCode == 2) printf("Nenhum Cliente Registrado!\n");
    if(readStatusCode != 1) return readStatusCode;

    //Loop de login 
    while (1) {
        voltaraomenu:
        printf("\nLOGIN UAIFOOD PEDIDOS\n");
        printf("\nDigite o codigo do cliente (ou um numero negativo para sair):\n");
        for(int i = 0; i < numClientes; i++) {
            printf("%i - %s\n", clientes[i].codigoCliente, clientes[i].nome);
        }
        printf("\n");
        scanf("%d", &codigoCliente);
        
        if (codigoCliente < 0) {
            break; //Sair 
        }
        
        int clienteIndex = findIndexOfClientByCode(codigoCliente);

        if (clienteIndex == -1) {
            printf("\nCliente nao encontrado. Tente novamente.\n");
            continue; //volta para o inicio do loop
        }

        //menu de opcoes
        while (1) {
            
            printf("\nBem-vindo %s - Selecione a opcao desejada:\n", clientes[clienteIndex].nome);
            printf(" 1 - Renomear Cliente\n");
            printf(" 2 - Deletar Cliente\n");
            printf(" 3 - Fazer Pedido\n");
            printf(" 4 - Mostrar Historico de Pedidos\n");
            printf("-1 - Fazer Logout\n\n");

            scanf("%i", &opcao);

            if(opcao == -1) {
                break; //Logout
            }
            else if(opcao == 1) { //Renomear
                renomearCliente(&clientes[clienteIndex]);
            } else if (opcao == 2) {
                deleteCliente(clienteIndex);
                if(numClientes == 0)
                    return 1;
                break;
            } else if (opcao == 3) {

                voltaraosrestaurantes:

                printf("\n%s - Novo pedido - Selecione o restaurante:\n", clientes[clienteIndex].nome);
                mostrarRestaurantes();
                printf(" 0 - Fechar Pedido\n");
                printf("-1 - Logout\n\n");
                scanf("%d", &opcao);

                restIndex = opcao-1;

                if (opcao == -1) {
                    free(pedidos);
                    numPedidos = 0;
                    pedidos = NULL;
                    continue; // Logout 
                } else if(opcao == 0) {
                    fecharPedido(clienteIndex);
                    continue;
                } else if (restIndex >= 0 && restIndex < numRests) {
                    
                    voltaraospratos:

                    printf("\n%s - Pratos - Menu de Opcoes\n", restaurantes[restIndex].nome);
                    if(!mostrarPratos(restaurantes[restIndex].codigo))
                        continue;
                    printf(" 0 - Fechar Pedido\n");
                    printf("-1 - Voltar ao menu anterior\n\n");
                    scanf("%d", &pratoEscolhido);

                    int index_prato = findIndexOfPratoByItsCodeAndRestCode(pratoEscolhido, restaurantes[restIndex].codigo);

                    //Ver detalhes de cada prato ou ir para o menu anterior
                    if (pratoEscolhido == -1){
                        goto voltaraosrestaurantes; 
                    } else if (pratoEscolhido == 0) {
                        fecharPedido(clienteIndex);
                        continue;
                    }else if (index_prato >= 0 && index_prato < numPratos) {
                        
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
                        pedidos[lastPedidoIndex].codigoRest = restaurantes[restIndex].codigo;
                        pedidos[lastPedidoIndex].codigoPrato = pratos[index_prato].codigoPrato;
                        pedidos[lastPedidoIndex].quantidade = quantidade;
                       
                        goto voltaraospratos;

                    } else {
                        printf("\nPrato Nao Encontrado!\n");
                        goto voltaraospratos;
                    }
                } else {
                    printf("\nRestaurante Nao Encontrado!\n");
                }
            } else if(opcao == 4) {
                mostrarHistorico(clientes[clienteIndex].codigoCliente);
            }
        }
    }
    
    return 1;
}

int main() {

    int hasExited = 0;

    readClientes();

    while(!hasExited) {

        mostrarCabecalho();

        printf("Selecione a acao desejada:\n");
        printf(" 1 - Criar Cliente\n");
        printf(" 2 - Fazer Login\n");
        printf("-1 - Sair\n\n");

        int option;
        scanf("%i", &option);

        switch(option) {
        case 2:
            hasExited = !login();
            break;
        case 1:
            hasExited = !createNewCliente();
            break;
        default:
            hasExited = 1;
        }

    }

    exit(1);

    free(restaurantes);
    free(pratos);
    free(pedidos);
    free(clientes);

    return 0;
}

void removeNewLineFromStrEnd(char* str) {
    char* newLinePtr = strrchr(str, '\n');
    if(newLinePtr != NULL)
        *newLinePtr = '\0';
}

int findIndexOfRestByCode(int codigoRestaurante) {
    for(int i = 0; i < numRests; i++) {
        if(codigoRestaurante == restaurantes[i].codigo)
            return i;
    }
    return -1;
}

int findIndexOfPratoByItsCodeAndRestCode(int pratoCode, int restCode) {
    for(int i = 0; i < numPratos; i++) {
        if(pratos[i].codigoPrato == pratoCode && pratos[i].codigoRest == restCode)
            return i;
    }
    return -1;
}

int findIndexOfClientByCode(int codigoCliente) {
    for(int i = 0; i < numClientes; i++) {
        if(codigoCliente == clientes[i].codigoCliente)
            return i;
    }
    return -1;
}

int deletePedidosOfCliente(int clienteIndex) {

    if(getBiggestPedidoCode() <= 0)
        return -1;

    FILE* fPedidos = fopen(pedidosTxt, "r");

    char* fileData = NULL;
    int fileDataSize = 0;
    char line[MAX_LINE_LENGTH];

    int allowedToRead = 1;

    char clienteCodeStr[16];
    //Definindo codeStr como "[CODIGO_DO_CLIENTE]"
    getClienteCodeStr(clientes[clienteIndex].codigoCliente, clienteCodeStr);

    while(!feof(fPedidos)) {
        fgets(line, MAX_LINE_LENGTH, fPedidos);

        //Checando se line é igual "[CODIGO_DO_CLIENTE]"
        if(!strncmp(line, clienteCodeStr, strlen(clienteCodeStr))){ 
            allowedToRead = 0;
            continue;
        } 
        //Checando se line é igual a "[END]" e se allowedToRead é 0
        else if(!strncmp(line, PEDIDOS_END_SUFFIX, strlen(PEDIDOS_END_SUFFIX)) && !allowedToRead) {
            allowedToRead = 1;
            continue;
        }

        if(allowedToRead) {
            fileData = (char*)realloc(fileData, fileDataSize+MAX_LINE_LENGTH);
            
            if(fileData == NULL) {
                allocerror();
                return -1;
            }

            if(fileDataSize == 0)
                strcpy(fileData, "");
            strcat(fileData, line);

            fileDataSize = strlen(fileData)+1;
            fileData = (char*)realloc(fileData, fileDataSize);
        }
    }

    fclose(fPedidos);

    fPedidos = fopen(pedidosTxt, "w");

    //Checando se o ultimo caractere é '\n' e deletando-o
    if(fileData[strlen(fileData)-1] == '\n')
        removeNewLineFromStrEnd(fileData);

    fprintf(fPedidos, "%s", fileData);

    fclose(fPedidos);
    free(fileData);
}

void swapClientes(Cliente* c1, Cliente* c2) {
    Cliente tempCliente = *c1;
    *c1 = *c2;
    *c2 = tempCliente;
}

int deleteCliente(int clienteIndex) {

    deletePedidosOfCliente(clienteIndex);

    int lastIndex = numClientes-1;

    for(int i = clienteIndex+1; i <= lastIndex; i++) {
        swapClientes(&clientes[i], &clientes[i-1]);
    }

    numClientes--;
    clientes = (Cliente*)realloc(clientes,numClientes*sizeof(Cliente));

    saveClientes();

    printf("\nCliente deletado com sucesso.\n");
}

int createNewCliente() {

    int newClienteCode = 1;

    if(numClientes > 0) {
        int biggestClienteCode_i = 0;

        for(int i = 1; i < numClientes; i++) {
            if(clientes[i].codigoCliente > clientes[biggestClienteCode_i].codigoCliente)
                biggestClienteCode_i = i;
        }

        newClienteCode = clientes[biggestClienteCode_i].codigoCliente + 1;
    }

    numClientes++;

    if(numClientes > 1) 
        clientes = (Cliente*)realloc(clientes, numClientes*sizeof(Cliente));
    else
        clientes = (Cliente*)malloc(sizeof(Cliente));

    if(clientes == NULL) {
        allocerror();
        return 0;
    }

    clientes[numClientes-1].codigoCliente = newClienteCode;

    printf("\nCriar novo cliente - Digite o nome do novo cliente:\n");

    getchar();
    fgets(clientes[numClientes-1].nome, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(clientes[numClientes-1].nome);

    return saveClientes();
}

int saveClientes() {
    FILE* fClientes = fopen(clientesTxt, "w");

    fprintf(fClientes, "%i", numClientes);

    for(int i = 0; i < numClientes; i++) {
        fprintf(fClientes, "\n%i %s", clientes[i].codigoCliente, clientes[i].nome);
    }

    fclose(fClientes);

    return 1;
}

int readClientes() {

    if(clientes != NULL) {
        free(clientes);
    }

    FILE* fClientes = fopen(clientesTxt, "r");

    fscanf(fClientes, "%i", &numClientes);

    if(numClientes == 0) {
        return 2;
    }
    
    clientes = (Cliente*)malloc(numClientes*sizeof(Cliente));

    if(clientes == NULL) {
        allocerror();
        return 0;
    }

    int i = 0;

    while(!feof(fClientes)) {

        if(i >= numClientes) {
            fclose(fClientes);
            printf("\nErro na leitura dos dados do arquivo \"%s\"!\n\n", clientesTxt);
            return 0;
        }

        fscanf(fClientes, "%i", &clientes[i].codigoCliente);
        fgetc(fClientes);

        fgets(clientes[i].nome, MAX_STRING_LENGTH, fClientes);
        removeNewLineFromStrEnd(clientes[i].nome);

        i++;
    }

    fclose(fClientes);

    return 1;
}

int readRests() {

    if(restaurantes != NULL) {
        free(restaurantes);
    }

    FILE* fRests = fopen(restsTxt, "r");

    fscanf(fRests, "%i", &numRests);

    if(numRests == 0) {
        return -1;
    }
    
    restaurantes = (Restaurante*)malloc(numRests*sizeof(Restaurante));

    if(restaurantes == NULL) {
        allocerror();
        return 0;
    }

    int i = 0;

    while(!feof(fRests)) {

        if(i >= numRests) {
            fclose(fRests);
            printf("\nErro na leitura dos dados do arquivo \"%s\"!\n\n", restsTxt);
            return 0;
        }

        fscanf(fRests, "%i", &restaurantes[i].codigo);
        fgetc(fRests);

        fgets(restaurantes[i].nome, MAX_STRING_LENGTH, fRests);
        removeNewLineFromStrEnd(restaurantes[i].nome);

        i++;
    }

    fclose(fRests);

    return 1;
}

int readPratos() {

    if(pratos != NULL) {
        free(pratos);
    }

    FILE* fPratos = fopen(pratosTxt, "r");

    fscanf(fPratos, "%i", &numPratos);

    if(numPratos == 0) {
        return -1;
    }

    pratos = (Prato*)malloc(numPratos*sizeof(Prato));

    if(pratos == NULL) {
        allocerror();
        return 0;
    }

    int i = 0;

    while(!feof(fPratos)) {

        if(i >= numPratos) {
            fclose(fPratos);
            printf("\nErro na leitura dos dados do arquivo \"%s\"!\n\n", pratosTxt);
            return 0;
        }

        fscanf(fPratos, "%i", &pratos[i].codigoPrato);
        fscanf(fPratos, "%i", &pratos[i].codigoRest);

        fgetc(fPratos);
        fgets(pratos[i].descricao, MAX_STRING_LENGTH, fPratos);
        
        /*Pegando a última ocorrência do
          caractere de espaço*/
        char* lastSpaceIndex = strrchr(pratos[i].descricao, ' ');
            
        /*Limitando a descrição até o ultimo caractere de espaço.
          Depois dele, é o preço do prato*/
        *lastSpaceIndex = '\0';

        char precoStr[16];
        strcpy(precoStr, lastSpaceIndex+1);

        sscanf(precoStr, "%f", &pratos[i].preco);

        i++;
    }

    fclose(fPratos);

    return 1;
}

void allocerror() {
    printf("\nErro de Alocacao!\n\n");
}

int arrayContainsValue(int* array, int arraySize, int value) {
    for(int i = 0; i < arraySize; i++) {
        if(array[i] == value)
            return 1;
    }
    return 0;
}
