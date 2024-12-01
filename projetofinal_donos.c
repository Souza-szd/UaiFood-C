#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

int numRests = 0,
numPratos = 0;

//Tamanho máximo para qualquer string do programa
#define MAX_STRING_LENGTH 50

#define restsTxt "restaurantes.txt"
#define pratosTxt "pratos.txt"

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

Restaurante* restaurantes = NULL;
Prato* pratos = NULL;

void allocerror();

void removeNewLineFromStrEnd(char* str);

int findIndexOfRestByCode(int codigoRestaurante);
int findIndexOfPratoByItsCodeAndRestCode(int pratoCode, int restCode);

int deleteRest(int restIndex);
int createNewRest();
int saveRests();
int readRests();

int deletePrato(int pratoIndex);
int createNewPrato();
int savePratos();
int readPratos();

//Função para cabeçalho
void mostrarCabecalho() {
    printf("\n**************************************************\n");
    printf("uaiFood - Restaurante\n");
    printf("**************************************************\n\n");
}

//função de mostrar pratos do restaurante
void mostrarPratos(int codigoRest) {
    int restIndex = findIndexOfRestByCode(codigoRest);

    if(restIndex == -1)
        return;

    printf("\nPratos disponiveis no restaurante %s:\n", restaurantes[restIndex].nome);
    for (int j = 0; j < numPratos; j++) {
        if (pratos[j].codigoRest == codigoRest) {
            printf(" %i - %s - R$%.2f\n", pratos[j].codigoPrato, pratos[j].descricao, pratos[j].preco);
        }
    }
}

// Função para renomear restaurante
void renomearRestaurante(Restaurante *restaurante) {
    getchar();
    printf("\nDigite o novo nome: \n");
    fgets(restaurante->nome, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(restaurante->nome);  // Remove o '\n' após final de fgets

    printf("\nO novo nome eh: %s\n", restaurante->nome);

    saveRests();
}


void renomearPrato (Prato *prato){
    getchar();
    printf ("%s - Digite o novo nome:\n", prato->descricao);
    fgets(prato->descricao, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(prato->descricao); //remove o \n após o final do fgets

    printf("O novo nome eh: %s\n", prato->descricao);

    savePratos();
}

//função da tela de login e menu de opcoes
void login() {
    int codigoRestaurante, opcao, pratoEscolhido, restauranteEscolhido, index_restaurante;

    mostrarCabecalho();

    int readStatusCode = readRests();

    if(readStatusCode == -1) printf("Nenhum Restaurante Registrado!\n");
    if(readStatusCode <= 0) return;

    //Loop de login 
    while (1) {
        voltaraomenu:

        if(numRests == 0)
            return;

        printf("\nLOGIN UAIFOOD RESTAURANTE\n");
        printf("\nDigite o codigo do restaurante (ou um numero negativo para sair):\n");
        scanf("%d", &codigoRestaurante);
        
        if (codigoRestaurante < 0) {
            break; //Sair 
        }

        //Ver se o restaurante existe
        int restIndex = findIndexOfRestByCode(codigoRestaurante);

        if (restIndex == -1) {
            printf("\nRestaurante nao encontrado. Tente novamente.\n");
            continue; //volta para o inicio do loop
        }

        //menu de opcoes
        while (1) {
            
            printf("\n%s - Menu de Opcoes\n", restaurantes[restIndex].nome);
            printf(" 1 - Renomear\n");
            printf(" 2 - Deletar\n");
            printf(" 3 - Mostrar Pratos\n");
            printf("-1 - Logout\n\n");

            scanf("%d", &opcao);

            if (opcao == -1) {
                break; // Logout 
            } else if (opcao == 1) { //renomear

                renomearRestaurante(&restaurantes[restIndex]);

            }else if(opcao==2){ // deletar restaurante
                
                deleteRest(restIndex);

                goto voltaraomenu;

            }else if (opcao==3){

                int readPratosStatusCode = readPratos();

                if(readPratosStatusCode == -1) printf("\nAinda Nao Existem Pratos!\n");
                if(readPratosStatusCode <= 0) continue;

                mostrarPratos(restaurantes[restIndex].codigo);
                printf(" 0 - Cadastrar Novo Prato\n");
                printf("-1 - Voltar ao menu anterior\n\n");
                scanf("%d", &pratoEscolhido);

                //Ver detalhes de cada prato ou ir para o menu anterior
                if (pratoEscolhido == -1){
                    continue; //voltar ao menu anterior
                }else if (pratoEscolhido == 0) {

                    createNewPrato(restIndex);

                } else {
                    readPratos();
                    int index_prato = findIndexOfPratoByItsCodeAndRestCode(pratoEscolhido, restaurantes[restIndex].codigo);

                    if(index_prato == -1) {
                        printf("\nPrato nao encontrado!\n");
                        continue;
                    }


                    printf("\n%s - Menu de Opcoes\n", pratos[index_prato].descricao);
                    printf("1 - Renomear\n");
                    printf("2 - Alterar preco\n");
                    printf("3 - Deletar\n");
                    printf("0 - Voltar ao menu anterior\n\n");
                    int aux; //essa variavel vai ajudar a ler com função o usuárioi vai escolher entre renomear e etc.
                    scanf("%d", &aux);
                    if (aux == 0) {
                        continue; //voltar para o outro menu
                    } else if(aux == 1){
                        renomearPrato(&pratos[index_prato]);
                    } else if (aux == 2){ //Alterar preço do prato
                        float novo_preco;
                        printf("\n%s - Digite o novo preco: \n", pratos[index_prato].descricao);
                        scanf("%f", &novo_preco);
                        index_restaurante = findIndexOfRestByCode(codigoRestaurante);
                        pratos[index_prato].preco = novo_preco;
                        
                        savePratos();
                    } else if(aux==3){ //deletar

                        deletePrato(index_prato);

                    }    

                }
            }

        }
    }
}

int main() {

    int hasExited = 0;

    readRests();

    while(!hasExited) {

        mostrarCabecalho();

        printf("Selecione a acao desejada:\n");
        printf(" 1 - Criar Restaurante\n");
        printf(" 2 - Fazer Login\n");
        printf("-1 - Sair\n\n");

        int option;
        scanf("%i", &option);

        switch(option) {
        case 2:
            login();
            break;
        case 1:
            hasExited = !createNewRest();
            break;
        default:
            hasExited = 1;
        }

    }

    exit(1);

    free(restaurantes);
    free(pratos);

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

void swapRests(Restaurante* r1, Restaurante* r2) {
    Restaurante tempRest = *r1;
    *r1 = *r2;
    *r2 = tempRest;
}

int deleteRest(int restIndex) {

    int lastIndex = numRests-1;

    for(int i = restIndex+1; i <= lastIndex; i++) {
        swapRests(&restaurantes[i], &restaurantes[i-1]);
    }

    numRests--;
    restaurantes = (Restaurante*)realloc(restaurantes,numRests*sizeof(Restaurante));

    saveRests();

    printf("\nRestaurante deletado com sucesso.\n");
}

int createNewRest() {

    int newRestCode = 1;

    if(numRests > 0) {
        int biggestRestCode_i = 0;

        for(int i = 1; i < numRests; i++) {
            if(restaurantes[i].codigo > restaurantes[biggestRestCode_i].codigo)
                biggestRestCode_i = i;
        }

        newRestCode = restaurantes[biggestRestCode_i].codigo + 1;
    }

    numRests++;

    if(numRests > 1) 
        restaurantes = (Restaurante*)realloc(restaurantes, numRests*sizeof(Restaurante));
    else
        restaurantes = (Restaurante*)malloc(sizeof(Restaurante));

    if(restaurantes == NULL) {
        allocerror();
        return 0;
    }

    restaurantes[numRests-1].codigo = newRestCode;

    printf("\nCriar novo restaurante - Digite o nome do novo restaurante:\n");

    getchar();
    fgets(restaurantes[numRests-1].nome, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(restaurantes[numRests-1].nome);

    return saveRests();

}

int saveRests() {
    FILE* fRests = fopen(restsTxt, "w");

    fprintf(fRests, "%i", numRests);

    for(int i = 0; i < numRests; i++) {
        fprintf(fRests, "\n%i %s", restaurantes[i].codigo, restaurantes[i].nome);
    }

    fclose(fRests);

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
            printf("Erro na leitura dos dados do arquivo \"%s\"!\n\n", restsTxt);
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

void swapPratos(Prato* p1, Prato* p2) {
    Prato tempPrato = *p1;
    *p1 = *p2;
    *p2 = tempPrato;
}

int deletePrato(int pratoIndex) {
    int lastIndex = numPratos-1;

    for(int i = pratoIndex+1; i <= lastIndex; i++) {
        swapPratos(&pratos[i], &pratos[i-1]);
    }

    numPratos--;
    pratos = (Prato*)realloc(pratos,numPratos*sizeof(Prato));

    savePratos();

    printf("\nPrato deletado com sucesso.\n");
}

int createNewPrato(int restIndex) {

    int newPratoCode = 1;

    if(numPratos > 0) {
        int biggestPratoCode_i = 0;

        for(int i = 1; i < numPratos; i++) {
            if(!pratos[i].codigoRest == restaurantes[restIndex].codigo)
                continue;
            if(pratos[i].codigoPrato > pratos[biggestPratoCode_i].codigoPrato)
                biggestPratoCode_i = i;
        }

        newPratoCode = pratos[biggestPratoCode_i].codigoPrato + 1;
    }

    numPratos++;

    if(numPratos > 1) 
        pratos = (Prato*)realloc(pratos, numPratos*sizeof(Prato));
    else
        pratos = (Prato*)malloc(sizeof(Prato));

    if(pratos == NULL) {
        allocerror();
        return 0;
    }

    pratos[numPratos-1].codigoPrato = newPratoCode;
    pratos[numPratos-1].codigoRest = restaurantes[restIndex].codigo;

    printf("\n%s - Pratos - Novo prato:\n\nDigite o nome do novo prato:\n", restaurantes[restIndex].nome);

    getchar();
    fgets(pratos[numPratos-1].descricao, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(pratos[numPratos-1].descricao);

    printf("\nDigite o preco do novo prato:\n");
    scanf("%f", &pratos[numPratos-1].preco);

    return savePratos();

}

int savePratos() {
    FILE* fPratos = fopen(pratosTxt, "w");

    fprintf(fPratos, "%i", numPratos);

    for(int i = 0; i < numPratos; i++) {
        fprintf(fPratos, "\n%i %i %s %g", pratos[i].codigoPrato, pratos[i].codigoRest, pratos[i].descricao, pratos[i].preco);
    }

    fclose(fPratos);

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
            printf("Erro na leitura dos dados do arquivo \"%s\"!\n\n", pratosTxt);
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
