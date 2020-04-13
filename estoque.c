#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#define QUANTIDADE_INVALIDA -1
#define TAMANHO_MAX 100
#define PRIMEIRO_NOH 0
#define NUMERO_DE_MATCHES 2
#define LISTA_VAZIA 0
#define NOMES_IGUAIS 0
#define TAMANHO_NULO 0
#define ARQUIVO_INVALIDO -1
enum Operacao {INSERIR=1, DELETAR, ADICIONAR, REMOVER, EXIBIR, LISTAR, SALVAR, CARREGAR, ENCERRAR};
typedef enum Operacao Operacao;
char * descricaoDaOperacao[]=
{
    [INSERIR]="Inserir um novo item (eletrodom�stico) no estoque",
    [DELETAR]="Deletar um item do estoque.",
    [ADICIONAR]="Adicionar no estoque unidades de um item.",
    [REMOVER]="Remover do estoque unidades de um item",
    [EXIBIR]="Exibir o n�mero de unidades de um item em estoque.",
    [LISTAR]="Listar todos os itens do estoque ao lado de sua quantidade estocada.",
    [SALVAR]="Salvar todos itens do estoque no arquivo �estoque.txt�.",
    [CARREGAR]="Carregar os itens do estoque do arquivo �estoque.txt�.",
    [ENCERRAR]="Encerrar as opera��es no estoque."
};
typedef struct Estoque Estoque;
struct Estoque
{
    char nome[TAMANHO_MAX];
    int unidadesEmEstoque;
    Estoque *prox;
};
typedef struct Estoque * Lista;
int TamanhoDaLista(Lista lista)
{
    int cont = 0;
    for(; lista->prox!=NULL; lista=lista->prox)
    {
        cont++;
    }
    return cont;
}
void ordenar(Lista *lista)
{
    bool trocou=true;
    for (int ateh=0; ateh<2*(TamanhoDaLista(*lista)) && trocou; ateh++)
    {
        trocou = false;
        for (Lista auxiliar=*lista; auxiliar->prox->prox!=NULL; auxiliar=auxiliar->prox)
        {

            if (strcmpi(auxiliar->prox->nome, auxiliar->prox->prox->nome)>0)
            {
                Lista posicao= auxiliar->prox;
                Lista aux = auxiliar->prox->prox->prox;
                auxiliar->prox=auxiliar->prox->prox;
                auxiliar->prox->prox=posicao;
                posicao->prox= aux;
                trocou=true;
            }
        }
    }
}
Operacao lerOperacao()
{
    int candidatoAOperacao;
    do
    {
        candidatoAOperacao=ENCERRAR;
        printf("\nDigite a Opera��o: ");
        scanf("%d", &candidatoAOperacao);
        if(candidatoAOperacao<INSERIR||candidatoAOperacao>ENCERRAR)
        {
            printf("\nOpera��o invalida!");
        }
    }
    while(candidatoAOperacao<INSERIR||candidatoAOperacao>ENCERRAR);
    return (Operacao)candidatoAOperacao;
}
bool Carregar(Lista *lista)
{
    FILE* arquivoEntrada= NULL;
    bool retorno= true;
    arquivoEntrada=fopen("estoque.txt", "r");
    int tamahoDaLista;
    if(arquivoEntrada==NULL)
    {
        printf("O arquivo inexiste!");
        retorno=false;
    }
    else
    {
        int matches=fscanf(arquivoEntrada, "%d", &tamahoDaLista);
        if (matches!=1)
        {
            printf("Arquivo mal formado ");
            retorno=false;
        }
        else if(tamahoDaLista<TAMANHO_NULO)
        {
            printf("Arquivo mal formado ");
            retorno=false;
        }
        else if(tamahoDaLista>=TAMANHO_NULO)
        {
            Lista auxiliar=NULL;
            int contador;
            *lista = (Lista)malloc(sizeof(Estoque));
            for (auxiliar=(*lista), contador=0; contador<=tamahoDaLista&&retorno; contador++)
            {

                if(contador==tamahoDaLista)
                {
                    matches=fscanf(arquivoEntrada, "%s %d");
                    if(matches!=EOF)
                    {
                        printf("Arquivo mal formado");
                        retorno=false;
                    }
                }
                else
                {

                    auxiliar->prox = (Lista)malloc(sizeof(Estoque));
                    auxiliar=auxiliar->prox;
                    matches=fscanf(arquivoEntrada, "%s %d",  auxiliar->nome, &auxiliar->unidadesEmEstoque);
                    if (matches!=NUMERO_DE_MATCHES)
                    {
                        printf("Arquivo mal formado ");
                        retorno=false;
                    }
                }
            }
            auxiliar->prox=NULL;
            for(Lista buscador=(*lista); buscador->prox!=NULL&&retorno; buscador=buscador->prox)
            {
                for(Lista auxiliar=(buscador); auxiliar->prox->prox!=NULL&&retorno; auxiliar=auxiliar->prox)
                {
                    if(strcmpi(buscador->prox->nome, auxiliar->prox->prox->nome)==0)
                    {
                        printf("Arquivo mal formado, h� dois �tens iguais no estoque");
                        retorno=false;
                    }
                }

            }
        }
    }
    fclose(arquivoEntrada);
    return retorno;
}
void Inserir(char nome[], Lista *lista, int  numeroDeUnidadesInseridas)
{
    Lista auxiliar, posicao=NULL;
    for(auxiliar=(*lista); auxiliar->prox!=NULL&&strcmpi(auxiliar->prox->nome,nome)<0; auxiliar=auxiliar->prox);

    if(strcmpi(auxiliar->prox->nome,nome)==NOMES_IGUAIS)
    {
        printf("Erro. O aparelho j� existe no estoque\n");
    }
    else
    {
        posicao=auxiliar->prox;
        auxiliar->prox=(Lista)malloc(sizeof(Estoque));
        strcpy(auxiliar->prox->nome, nome);
        auxiliar->prox->unidadesEmEstoque= numeroDeUnidadesInseridas;
        auxiliar->prox->prox=posicao;
    }
}

void Deletar(char nome[], Lista *lista)
{
    Lista auxiliar, posicao = NULL;
    bool achou = false;
    for(auxiliar=(*lista); auxiliar->prox!=NULL && !achou; auxiliar=auxiliar->prox)
    {
        if(strcmpi(auxiliar->prox->nome, nome)==NOMES_IGUAIS)
        {
            posicao = auxiliar;
            achou = true;
        }
    }
    if(!achou)
    {
        printf("O item que voc� deseja deletar n�o existe no estoque\n");
    }
    else
    {
        Lista ajudante;
        ajudante = posicao->prox;
        posicao->prox = posicao->prox->prox;
        free(ajudante);
    }
}

void Adicionar(Lista *lista, int numeroDeUnidadesAdicionadas, char nomeAparelho[])
{
    Lista auxiliar, posicao = NULL;
    bool achou = false;
    for(auxiliar=(*lista); auxiliar->prox!=NULL && !achou; auxiliar=auxiliar->prox)
    {
        if(strcmpi(auxiliar->prox->nome, nomeAparelho)==NOMES_IGUAIS)
        {
            posicao = auxiliar->prox;
            achou = true;
        }
    }
    if(!achou)
    {
        printf("O aparelho n�o existe no estoque\n");
    }
    else
    {
        posicao->unidadesEmEstoque+=numeroDeUnidadesAdicionadas;
    }
}
void Remover(Lista *lista, int numeroDeUnidadesRemovidas, char nomeAparelho[])
{
    Lista auxiliar, posicao = NULL;
    bool achou = false;
    for(auxiliar=(*lista); auxiliar->prox!=NULL && !achou; auxiliar=auxiliar->prox)
    {
        if(strcmpi(auxiliar->prox->nome, nomeAparelho)==NOMES_IGUAIS)
        {
            posicao = auxiliar->prox;
            achou = true;
        }
    }
    if(!achou)
    {
        printf("O aparelho n�o existe no estoque\n");
    }
    else if (posicao->unidadesEmEstoque<numeroDeUnidadesRemovidas)
    {
        printf("O n�mero de unidades a serem removidas � maior que o n�mero de unidades em estoque e, portanto, ser� deletada apenas a  quantidade em estoque.\n\n");
        posicao->unidadesEmEstoque=0;
    }
    else
    {
        posicao->unidadesEmEstoque-=numeroDeUnidadesRemovidas;
    }
}
void Exibir(char nomeAparelho[], Lista *lista)
{
    Lista auxiliar, posicao = NULL;
    bool achou = false;
    for(auxiliar=(*lista); auxiliar->prox!=NULL && !achou; auxiliar=auxiliar->prox)
    {
        if(strcmpi(auxiliar->prox->nome, nomeAparelho)==NOMES_IGUAIS)
        {
            posicao = auxiliar->prox;
            achou = true;
        }
    }
    if(!achou)
    {
        printf("O aparelho n�o existe no estoque\n");
    }
    else
    {
        printf("Quantidade em estoque: %d\n", posicao->unidadesEmEstoque);
    }
}
void Listar(Lista lista)
{
    Lista posicao;
    if (lista->prox==NULL)
    {
        printf("O estoque est� vazio\n");
    }
    else
    {
        for (posicao=lista->prox; posicao!=NULL; posicao=posicao->prox)
        {
            printf("%s %d unidades\n", posicao->nome, posicao->unidadesEmEstoque);
        }
    }
}
void Salvar(Lista lista)
{
    FILE* arquivoSaida= fopen("estoque.txt", "w");
    fprintf(arquivoSaida, "%d\n",TamanhoDaLista(lista));
    for(Lista auxiliar=lista; auxiliar->prox!=NULL; auxiliar=auxiliar->prox)
    {
        fprintf(arquivoSaida, "%s %d\n", auxiliar->prox->nome, auxiliar->prox->unidadesEmEstoque);
    }
    fclose(arquivoSaida);
    printf("Estoque salvo com sucesso\n");
}
void cabecalho(Lista estoque)
{

    Operacao operacao = ENCERRAR;
    do
    {
        for(int operation=INSERIR; operation<=ENCERRAR; operation++)
        {
            printf("%d. %s\n", operation, descricaoDaOperacao[operation]);
        }
        switch(operacao = lerOperacao())
        {
        case INSERIR:
        {
            int numeroDeUnidadesInseridas=QUANTIDADE_INVALIDA;
            char nome[TAMANHO_MAX];
            printf("Informe o nome do aparelho que deseja inserir e a quantidade que ser� estocada, respectivamente: ");
            fflush(stdin);
            scanf("%s %d", nome, & numeroDeUnidadesInseridas);
            while( numeroDeUnidadesInseridas<0)
            {
                printf("A quantidade de �tens deve ser positiva! Digite novamente: ");
                fflush(stdin);
                scanf("%d", & numeroDeUnidadesInseridas);
            }
            Inserir(nome, &estoque,  numeroDeUnidadesInseridas);
            break;
        }
        case ADICIONAR:
        {
            if(TamanhoDaLista(estoque)==LISTA_VAZIA)
            {
                printf("O estoque est� vazio, n�o � poss�vel adicionar �tens a um estoque vazio\n");
            }
            else
            {
                printf("Informe o aparelho do qual desejas adicionar �tens e a quantidade de �tens, respectivamente: ");
                fflush(stdin);
                int numeroDeUnidadesAdicionadas=QUANTIDADE_INVALIDA;
                char nome[TAMANHO_MAX];
                scanf("%s %d", nome, &numeroDeUnidadesAdicionadas);
                fflush(stdin);
                while(numeroDeUnidadesAdicionadas<0)
                {
                    printf("A quantidade de �tens deve ser positiva!, digite novamente: ");
                    fflush(stdin);
                    scanf("%d", &numeroDeUnidadesAdicionadas);
                }
                Adicionar(&estoque,numeroDeUnidadesAdicionadas, nome);
            }
            break;
        }
        case DELETAR:
        {
            char nome[TAMANHO_MAX];
            if(TamanhoDaLista(estoque)==LISTA_VAZIA)
            {
                printf("O estoque est� vazio, n�o � poss�vel deletar aparelhos de um estoque vazio\n");
            }
            else
            {
                printf("Digite o nome do aparelho que voc� deseja deletar: ");
                fflush(stdin);
                scanf("%s", nome);
                Deletar(nome, &estoque);
            }

            break;
        }
        case REMOVER:
        {
            if(TamanhoDaLista(estoque)==LISTA_VAZIA)
            {
                printf("O estoque est� vazia, n�o � poss�vel remover �tens de um estoque vazio\n");
            }
            else
            {
                printf("Informe o aparelho do qual desejas remover �tens e a quantidade de �tens, respectivamente: ");
                fflush(stdin);
                int numeroDeUnidadesRemovidas=QUANTIDADE_INVALIDA;
                char nome[TAMANHO_MAX];
                scanf("%s %d", nome, &numeroDeUnidadesRemovidas);
                fflush(stdin);
                while(numeroDeUnidadesRemovidas<0)
                {
                    printf("A quantidade de �tens deve ser positiva!, digite novamente: ");
                    fflush(stdin);
                    scanf("%d", &numeroDeUnidadesRemovidas);
                }
                Remover(&estoque,numeroDeUnidadesRemovidas, nome);
            }
            break;


        }
        case LISTAR:
        {
            Listar(estoque);
            break;
        }
        case SALVAR:
        {
            Salvar(estoque);
            break;
        }
        case EXIBIR:
        {
            if(TamanhoDaLista(estoque)==LISTA_VAZIA)
            {
                printf("O estoque est� vazio, n�o � poss�vel exibir �tens de um estoque vazio\n");
            }
            else
            {
                char nome[TAMANHO_MAX];
                printf("Digite o nome do aparelho do qual voc� deseja saber a quantidade em estoque: ");
                fflush(stdin);
                scanf("%s", nome);
                Exibir(nome, &estoque);
            }
            break;
        }
        case ENCERRAR:
        {
            printf("Fim da execu��o. bye bye");
            break;
        }
        }
    }
    while (operacao != ENCERRAR);
}
int main()
{
    setlocale(LC_ALL, "Portuguese");
    Lista estoque=NULL;
    bool retorno=Carregar(&estoque);
    if(retorno)
    {
        ordenar(&estoque);
        cabecalho(estoque);
    }
    return(!retorno);
}
