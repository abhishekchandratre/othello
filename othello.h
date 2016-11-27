#ifndef OTHELLO_H_
#define OTHELLO_H_

#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

/*Function would initialize a game state with given size n*/
/*Where WHITE is 0*/
/*BLACK is 1*/
/*GREY is -1*/
int **initialize_matrix(int n)
{
	int i,j;
	int **temp;
	temp = (int **)malloc(sizeof(int *)*n);
	for(i=0;i<n;i++){
		temp[i] = (int *)malloc(sizeof(int )*n);
	}
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			temp[i][j]=GREY;

	temp[n/2][n/2] = LIGHT;
	temp[n/2][n/2-1] = DARK;
	temp[n/2-1][n/2] = DARK;
	temp[n/2-1][n/2-1] = LIGHT;
	return temp;
}

void free_matrix(int **matrix,int n)
{
	int i;
	for(i=0;i<n;i++)
		free(matrix[i]);
	free(matrix);
}

/*This function takes in two parameter to display the game state*/
void print_matrix(int **matrix, int n)
{
	int i,j;
	printf("\n");
	for(i=-1;i<n;i++){
		if(-1==i)
			printf("-\t");
		else
			printf("|%d|\t",i);
	}
	for(i=0;i<n;i++){
		printf("\n%d|\t",i);
		for(j=0;j<n;j++){
			if(matrix[i][j]==GREY)
				printf(" _\t");
			else if(matrix[i][j]==POSSIBLE_MOVE)
				printf(YEL " +\t" RESET);
			else if(matrix[i][j]==LIGHT)
				printf(BLU " %d\t" RESET,matrix[i][j]);
			else if(matrix[i][j]==DARK)
				printf(RED " %d\t" RESET,matrix[i][j]);
		}
	}
}
#endif
