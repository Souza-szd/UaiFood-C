#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

int numRests = 0;
int numPratos = 0;

//Tamanho máximo para qualquer string do programa
#define MAX_STRING_LENGTH 50
#define MAX_LINE_LENGTH MAX_STRING_LENGTH*3

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

void removeNewLineFromStrEnd(char* str);

int findIndexOfRestByCode(int codigoRestaurante);

//Função para cabeçalho
void mostrarCabecalho() {
    printf("\n**************************************************\n");
    printf("uaiFood - Restaurante\n");
    printf("**************************************************\n");
}

//Função adicionar restaurante
void add_restaurantes_e_pratos(FILE* fRests) {
    for (int i = 0; i < numRests; i++) {
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

        //Definindo NUM_PRATOS_POR_RESTAURANTE temporariamente
        //para evitar erros de compilação
        int NUM_PRATOS_POR_RESTAURANTE = 0;

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

//função de mostrar pratos do restaurante
void mostrarPratos(int codigoRest) {
    int restIndex = findIndexOfRestByCode(codigoRest);

    if(restIndex == -1)
        return;

    //Definindo NUM_PRATOS_POR_RESTAURANTE temporariamente
    //para evitar erros de compilação
    int NUM_PRATOS_POR_RESTAURANTE = 0;

    printf("\nPratos disponiveis no restaurante %s:\n", restaurantes[restIndex].nome);
    for (int j = 0; j < NUM_PRATOS_POR_RESTAURANTE; j++) {
        
        int pratoIndex = restIndex * NUM_PRATOS_POR_RESTAURANTE + j;
        Prato prato = pratos[pratoIndex];
        if (prato.codigoPrato != -1){
            printf("%d - %s - R$%.2f\n", prato.codigoPrato, prato.descricao, prato.preco);
        }
    }
}

// Função para renomear restaurante
void renomearRestaurante(Restaurante *restaurante) {
    char novo_nome[MAX_STRING_LENGTH];
    getchar();
    printf("Digite o novo nome: \n");
    fgets(novo_nome, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(novo_nome);  // Remove o '\n' após final de fgets

    strcpy(restaurante->nome, novo_nome);

    printf("O novo nome eh: %s\n", restaurante->nome);
}


void renomearPrato (Prato *prato){
    char novo_nome[MAX_STRING_LENGTH];
    getchar();
    printf ("%s - Digite o novo nome: ", prato->descricao);
    fgets(novo_nome, MAX_STRING_LENGTH, stdin);
    removeNewLineFromStrEnd(novo_nome); //remove o \n após o final do fgets

    strcpy(prato->descricao, novo_nome);

    printf("O novo nome eh: %s\n", prato->descricao);
}

void criarRest() {

}

//função da tela de login e menu de opcoes
void login() {
    int codigoRestaurante, opcao, pratoEscolhido, restauranteEscolhido, index_restaurante;

    mostrarCabecalho();
    //add_restaurantes_e_pratos();

    FILE* fRests;

    fRests = fopen("restaurantes.txt", "a+");

    if(feof(fRests)) {
        printf("\nNenhum Restaurante Registrado!\n\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, fRests);
    sscanf(line, "%i", &numRests);

    restaurantes = (Restaurante*)malloc(numRests*sizeof(Restaurante));

    int i = 0;

    while(!feof(fRests)) {

        if(i >= numRests) {
            printf("Erro na leitura dos dados do arquivo \"restaurante.txt\"!\n\n");
            return;
        }

        fgets(line, MAX_LINE_LENGTH, fRests);

        printf("\nline: %s\n", line);
        sscanf(line, "%i", &restaurantes[i].codigo);

        char* firstSpacePtr = strchr(line, ' ');

        char restName[MAX_STRING_LENGTH] = "";
        strcat(restName, firstSpacePtr+1);
        
        strcpy(restaurantes[i].nome, restName);

        //printf("\nAdded rest with name: %s\nCode: %i\n\n", restName, restaurantes[i].codigo);

        i++;
    }

    //Loop de login 
    while (1) {
        voltaraomenu:
        printf("\nLOGIN UAIFOOD RESTAURANTE\n");
        printf("\nDigite o codigo do restaurante (ou um numero negativo para sair):\n");
        scanf("%d", &codigoRestaurante);
        index_restaurante = findIndexOfRestByCode(codigoRestaurante);

        if (codigoRestaurante < 0) {
            break; //Sair 
        }
        //Ver se o restaurante existe
        int i = findIndexOfRestByCode(codigoRestaurante);

        if (i == -1) {
            printf("\nRestaurante nao encontrado. Tente novamente.\n");
            continue; //volta para o inicio do loop
        }

        //menu de opcoes
        while (1) {
            
            printf("\n%s - Menu de Opcoes\n", restaurantes[i].nome);
            printf("1 - Renomear\n");
            printf("2 - Deletar\n");
            printf("3 - Mostrar pratos\n");
            printf("0 - Logout\n\n");
            scanf("%d", &opcao);

            if (opcao == 0) { //na etapa 2, a alteração deve ser feita aqui
                break; // Logout 
            } else if (opcao == 1) { //renomear
                renomearRestaurante(&restaurantes[i]);


            }else if(opcao==2){ // deletar restaurante
                restaurantes[index_restaurante].codigo = -1;
                
                //Deletar o nome do restaurante
                restaurantes[index_restaurante].nome[0]= '\0';

                printf("Restaurante deletado com sucesso.\n");

                goto voltaraomenu;

            }else if (opcao==3){
                mostrarPratos(restaurantes[i].codigo);
                printf("0 - Voltar ao menu anterior\n\n");
                scanf("%d", &pratoEscolhido);

                //Ver detalhes de cada prato ou ir para o menu anterior
                if (pratoEscolhido==0){
                    continue; //voltar ao menu anterior
                }else if (pratoEscolhido != 0){
                    int index_prato = (index_restaurante*4) + pratoEscolhido -1;
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
                        //dentro do colchetes tem que somar o numero do restaurante por conta da posição do prato no vetor
                    } else if(aux==3){ //deletar
                        pratos[index_prato].codigoPrato = -1;
                        pratos[index_prato].codigoRest = -1;
                        pratos[index_prato].preco = -1;
                        pratos[index_prato].descricao[0]= '\0';
                    }    

                }
            }

        }
    }
}

int main() {

    int hasExited = 0;

    while(!hasExited) {

        printf("**************************************************\n");
        printf("uaiFood - Restaurante\n");
        printf("**************************************************\n\n");

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
            criarRest();
            break;
        default:
            hasExited = 1;
        }

    }

    exit(1);

    return 0;
}

void removeNewLineFromStrEnd(char* str) {
    str[strlen(str) - 1] = '\0';
}

int findIndexOfRestByCode(int codigoRestaurante) {
    for(int i = 0; i < numRests; i++) {
        if(codigoRestaurante == restaurantes[i].codigo)
            return i;
    }
    return -1;
}
