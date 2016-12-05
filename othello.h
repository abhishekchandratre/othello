#ifndef OTHELLO_H_
#define OTHELLO_H_

#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "validation.h"
#define SCALE 3

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

void print_matrix_with_colors(int **matrix,int row,int col,int curr_row,int curr_col,int n)
{
	int i,j;
	printf("\n");
	for(i=-1;i<n;i++){
		if(-1==i)
			printf("-");
		else
			printf(" %c ",i+97);
	}
	printf(BG_GRN);
	printf(BLK);
	for(i=0;i<n;i++){
		printf(RESET);
		printf("\n%d",i+1);
		printf(BG_GRN);
		printf(BLK);
		for(j=0;j<n;j++){
			if(matrix[i][j]==GREY){
				printf(" _ ");
			}
			else if(matrix[i][j]==POSSIBLE_MOVE){
				if((i==curr_row)&&(j==curr_col)){
					printf(BG_RED);
					printf(CYN " + " RESET);
				}else{
					printf(BG_MAG);
					printf(YEL " + " RESET);
				}
			}
			else if(matrix[i][j]==LIGHT){
				if((i==row)&&(j==col)){
					printf(BG_BLK);
					printf(GRN " %d " RESET,matrix[i][j]);
				}else{
					printf(BG_BLK);
					printf(RED " %d " RESET,matrix[i][j]);
				}
			}
			else if(matrix[i][j]==DARK){
				printf(BG_WHT);
				printf(BLK " %d " RESET,matrix[i][j]);
			}
			printf(BG_GRN);
			printf(BLK);
		}
	}
	printf(RESET);
	printf("\n");
}
#endif
