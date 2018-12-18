//Trabalho EDD 2017//
//Rato dentro de um labirinto//
//Autores: Haruan Justino, Luanna Gobbato//
//05-2017//

//funcao para sa?a do labirinto//
// livre: " ", parede: "█", visitado: ".", beco: ░, rato: "♥"//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h> // biblioteca para acentuação e caracteres especiais

#define FREE 32         // " "
#define WALL 219        // █
#define VISITED 46      // .
#define ALLEY 176           // ░
#define CURRENT 3           // ♥

#define numLine 30
#define numColunm 30

#define xFirstSum 0 // (0,-1) baixo, direita, cima, esquerda 
#define yFirstSum -1 // sempre em sentido anti-horário

typedef struct node{
	int position;
	struct node *next;
	int xSum;
	int ySum;
} *Node;

Node createStack();
void push(Node *stack, int position);
void pop(Node *stack);
void clearStack(Node *stack);
void createMaze(int maze[numLine][numColunm]);
void showMaze(int maze[numLine][numColunm]);
int  searchRote(Node *stack, int maze[numLine][numColunm]);
void clearScrean();
void delay();

Node createStack(){
	Node p;
	p = (Node)malloc(sizeof(struct node));
	if (!p){
		printf("!!!Problemas na Alocação!!!");
		exit(0);
	}
	return p;
}

void push(Node *stack, int position){
	Node new = createStack();

	new->position = position;
	new->next = *stack;
	new->xSum = xFirstSum; // xSum = 0 
	new->ySum = yFirstSum; // ySum = -1
	*stack = new;
}

void pop(Node *stack){
	Node aux;

	aux = *stack;
	*stack = (*stack)->next;
	free(aux);
}

void clearStack(Node *stack){
	while (*stack != NULL){
		pop(stack);
	}
}

void createMaze(int maze[numLine][numColunm]){
	int count1, count2;

	for (count1 = 0; count1 < numLine; count1++)
		for (count2 = 0; count2 < numColunm; count2++)
			maze[count1][count2] = WALL; 						// preenche toda a matriz com paredes

	srand(time(NULL));

	for (count1 = 1; count1 < numLine - 1; count1++)
		for (count2 = 1; count2 < numColunm - 1; count2++)
			if (rand() % 100 < 70)
				maze[count1][count2] = FREE;

	maze[1][1] = FREE;													//libera a posição inicial do rato
	maze[numLine - 2][numColunm - 1] = FREE;		//libera a saída do labirinto
}

void showMaze(int maze[numLine][numColunm]){
	int count1, count2, count3 = 0;
	char lineMaze[numColunm];										//vetor para imprimir linhas de caracteres

	clearScrean();

	for (count1 = 0; count1 < numLine; count1++){
		for (count2 = 0; count2 < numColunm; count2++){
			lineMaze[count3] = maze[count1][count2];
			count3++;
		}
		printf("%s \n", lineMaze);
		count3 = 0;
	}

	printf("\n");
}

void delay(){
		usleep(100000);
}

int searchRote(Node *stack, int maze[numLine][numColunm]){
	int xNext, yNext, zAux;

	do{																					//posição atual
		maze[(*stack)->position / 100][(*stack)->position % 100] = CURRENT; 

		if ((*stack)->position % 100 == numColunm - 1)	
			return 1; 															//verifica se a posição é a saída

		zAux = (*stack)->xSum;
		(*stack)->xSum = -(*stack)->ySum;					// 1st xSum = 1
		(*stack)->ySum = zAux;                   	// 1st ySum = 0
		
		xNext = (*stack)->position / 100 + (*stack)->xSum;  // 1st: xNext=2 (1+1)
		yNext = (*stack)->position % 100 + (*stack)->ySum;  // 1st: yNext=1 (1+0)

		if (maze[xNext][yNext] == FREE){
			showMaze(maze);
			maze[(*stack)->position / 100][(*stack)->position % 100] = VISITED;

			push(stack, xNext * 100 + yNext);
			delay();
		}
		else if ((*stack)->xSum == xFirstSum && 
				(*stack)->ySum == yFirstSum){   // inexistem lados livres
			showMaze(maze);
			maze[(*stack)->position / 100][(*stack)->position % 100] = ALLEY;
			pop(stack);
			delay();
		}

		delay();

	} while (*stack != NULL); // enquanto existirem elementos na pilha

	return 0;
}

void clearScrean(){
		system("cls");
}

int main(){
	setlocale(LC_ALL, "pt_BR.utf8");			//habilita a acentuação para o português
	int maze[numLine][numColunm], stay = 0;
	Node stack = NULL;

	do{
		clearScrean();
		do{
			createMaze(maze);
			showMaze(maze);

			printf("\n1. Continuar com esse labirinto.");
			printf("\n0. Criar outro labirinto.\n");
			scanf(" %i", &stay);
		} while (!stay);

		push(&stack, 101);    // empilha posição inicial

		if (searchRote(&stack, maze)){
			showMaze(maze);
			printf("Saída encontrada!\n");
		}
		else{
			showMaze(maze);
			printf("Labirinto sem saída\n");
		}

		clearStack(&stack);

		printf("\n\n1. Executar novamente?");
		printf("\n0. Sair\n");
		scanf("%i", &stay);
	} while (stay);
}

