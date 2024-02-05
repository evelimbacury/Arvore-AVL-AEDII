#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó da árvore AVL
typedef struct No {
    int chave;
    struct No *esquerda;
    struct No *direita;
    int altura;
} No;

// Função para criar um novo nó
No *novoNo(int chave) {
    No *no = (No *)malloc(sizeof(No));
    no->chave = chave;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1; // Novo nó tem altura 1
    return no;
}

// Função para calcular a altura de um nó
int altura(No *no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

// Função para calcular o máximo de dois números
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para realizar uma rotação simples à direita
No *rotacaoDireita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    // Realiza a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualiza as alturas
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

// Função para realizar uma rotação simples à esquerda
No *rotacaoEsquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    // Realiza a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualiza as alturas
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

// Função para calcular o fator de balanceamento de um nó
int fatorBalanceamento(No *no) {
    if (no == NULL)
        return 0;
    return altura(no->esquerda) - altura(no->direita);
}

// Função para inserir um nó na árvore AVL
No *inserir(No *no, int chave) {
    if (no == NULL)
        return novoNo(chave);

    // Realiza a inserção
    if (chave < no->chave)
        no->esquerda = inserir(no->esquerda, chave);
    else if (chave > no->chave)
        no->direita = inserir(no->direita, chave);
    else
        return no; // Chaves iguais não são permitidas em uma árvore AVL

    // Atualiza a altura do nó atual
    no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;

    // Calcula o fator de balanceamento
    int balanciamento = fatorBalanceamento(no);

    // Casos de desbalanceamento

    // Rotação à direita (Caso Esquerda-Esquerda)
    if (balanciamento > 1 && chave < no->esquerda->chave)
        return rotacaoDireita(no);

    // Rotação à esquerda (Caso Direita-Direita)
    if (balanciamento < -1 && chave > no->direita->chave)
        return rotacaoEsquerda(no);

    // Rotação dupla à direita (Caso Esquerda-Direita)
    if (balanciamento > 1 && chave > no->esquerda->chave) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }

    // Rotação dupla à esquerda (Caso Direita-Esquerda)
    if (balanciamento < -1 && chave < no->direita->chave) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }

    return no;
}

// Função para encontrar o mínimo
No *acharMin(No *no) {
    while (no->esquerda != NULL)
        no = no->esquerda;
    return no;
}

// Função para encontrar o máximo
No *acharMax(No *no) {
    while (no->direita != NULL)
        no = no->direita;
    return no;
}

// Função para remover um nó da árvore AVL
No *remover(No *raiz, int chave) {
    if (raiz == NULL)
        return raiz;

    // Realiza a remoção
    if (chave < raiz->chave)
        raiz->esquerda = remover(raiz->esquerda, chave);
    else if (chave > raiz->chave)
        raiz->direita = remover(raiz->direita, chave);
    else {
        // Nó a ser removido encontrado

        // Caso com apenas um filho ou nenhum filho
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            // Caso sem filho
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else // Caso com um filho
                *raiz = *temp; // Copia o conteúdo do nó não nulo
            free(temp);
        } else {
            // Caso com dois filhos, obter o sucessor in-order (menor da subárvore direita)
            No *temp = acharMin(raiz->direita);

            // Copiar o sucessor in-order para este nó
            raiz->chave = temp->chave;

            // Remover o sucessor in-order
            raiz->direita = remover(raiz->direita, temp->chave);
        }
    }

    // Se a árvore tinha apenas um nó, então retorna
    if (raiz == NULL)
        return raiz;

    // Atualiza a altura do nó atual
    raiz->altura = max(altura(raiz->esquerda), altura(raiz->direita)) + 1;

    // Calcula o fator de balanceamento
    int balanciamento = fatorBalanceamento(raiz);

    // Casos de desbalanceamento

    // Rotação à direita (Left-Left Case)
    if (balanciamento > 1 && fatorBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);

    // Rotação à esquerda (Right-Right Case)
    if (balanciamento < -1 && fatorBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);

    // Rotação dupla à direita (Left-Right Case)
    if (balanciamento > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    // Rotação dupla à esquerda (Right-Left Case)
    if (balanciamento < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Função para imprimir a árvore em ordem (in-order traversal)
void imprimirEmOrdem(No *raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esquerda);
        printf("%d ", raiz->chave);
        imprimirEmOrdem(raiz->direita);
    }
}

// Função para buscar um nó na árvore AVL
No *buscar(No *raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave)
        return raiz;

    if (chave < raiz->chave)
        return buscar(raiz->esquerda, chave);
    return buscar(raiz->direita, chave);
}

// Função para encontrar o sucessor de um nó
No *sucessor(No *raiz, int chave) {
    No *atual = buscar(raiz, chave);

    if (atual == NULL)
        return NULL;

    // Caso 1: Se o nó tem uma subárvore direita
    if (atual->direita != NULL)
        return acharMin(atual->direita);

    // Caso 2: Se o nó não tem uma subárvore direita
    No *sucessor = NULL;
    No *antecessor = raiz;

    while (antecessor != atual) {
        if (atual->chave < antecessor->chave) {
            sucessor = antecessor;
            antecessor = antecessor->esquerda;
        } else
            antecessor = antecessor->direita;
    }

    return sucessor;
}

// Função para encontrar o predecessor de um nó
No *predecessor(No *raiz, int chave) {
    No *atual = buscar(raiz, chave);

    if (atual == NULL)
        return NULL;

    // Caso 1: Se o nó tem uma subárvore esquerda
    if (atual->esquerda != NULL)
        return acharMax(atual->esquerda);

    // Caso 2: Se o nó não tem uma subárvore esquerda
    No *predecessor = NULL;
    No *antecessor = raiz;

    while (antecessor != atual) {
        if (atual->chave > antecessor->chave) {
            predecessor = antecessor;
            antecessor = antecessor->direita;
        } else
            antecessor = antecessor->esquerda;
    }

    return predecessor;
}



// Função principal
int main() {
    No *raiz = NULL;

    // Inserir elementos na árvore
    raiz = inserir(raiz, 50);
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 70);
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 40);
    raiz = inserir(raiz, 60);
    raiz = inserir(raiz, 80);

    // Imprimir a árvore em ordem
    printf("Árvore em ordem: ");
    imprimirEmOrdem(raiz);
    printf("\n");

    // Buscar um elemento na árvore
    int chaveBusca = 40;
    No *resultadoPesquisa = buscar(raiz, chaveBusca);
    if (resultadoPesquisa != NULL)
        printf("Elemento %d encontrado na árvore.\n", chaveBusca);
    else
        printf("Elemento %d não encontrado na árvore.\n", chaveBusca);

    // Encontrar o sucessor e predecessor
    int noChave = 30;
    No *noSucessor = sucessor(raiz, noChave);
    No *noPredecessor = predecessor(raiz, noChave);

    printf("Sucessor de %d: %d\n", noChave, noSucessor ? noSucessor->chave : -1);
    printf("Predecessor de %d: %d\n", noChave, noPredecessor ? noPredecessor->chave : -1);

    // Encontrar o mínimo e máximo
    No *noMin = acharMin(raiz);
    No *noMax = acharMax(raiz);

    printf("Mínimo: %d\n", noMin ? noMin->chave : -1);
    printf("Máximo: %d\n", noMax ? noMax->chave : -1);

    // Remover um elemento da árvore
    int chaveDeletar = 30;
    raiz = remover(raiz, chaveDeletar);

    // Imprimir a árvore em ordem após a remoção
    printf("Árvore em ordem após a remoção: ");
    imprimirEmOrdem(raiz);
    printf("\n");

    return 0;
}
