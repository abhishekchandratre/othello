#ifndef VALIDATION_H_
#define VALIDATION_H_
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

//The function would perform validation to check whether current config
//is valid or not and return an array of possible directions
int *validate_matrix(int **matrix,int row,int col,int n)
{
	int i;
	int *valid_directions,*possible_area;
	int my_value;
	valid_directions = (int *)calloc(SQUARE,sizeof(int ));
	my_value = get_self(matrix,row,col,n);
	if(my_value==-1){
		printf("\nPlease set the matrix value before calling validatea matrix");
		exit(1);
	}
	possible_area = get_neighbors(matrix,row,col,n);
	for(i=0;i<SQUARE;i++){
		if(DARK == my_value){
			if(LIGHT == possible_area[i]){
				if(my_value == check_direction(matrix,row,col,n,i,0)){
					valid_directions[i] = 1;
				}
			}
		}
		else{
			if(DARK == possible_area[i]){
				if(my_value == check_direction(matrix,row,col,n,i,0)){
					valid_directions[i] = 1;
				}
			}
		}
	}
	free_neighbors(possible_area);
	return valid_directions;
}

void free_validate_matrix(int *result)
{
	free(result);
}

int validate_state(int **matrix,int row,int col,int n)
{
	int i,*temp;
	if((row<0)||(row>=n))
		return 0;
	if((col<0)||(col>=n))
		return 0;
	temp = validate_matrix(matrix,row,col,n);
	for(i=0;i<SQUARE;i++){
		if(temp[i]){
			free_validate_matrix(temp);
			return 1;
		}
	}
	free_validate_matrix(temp);
	return 0;
}

void confirm_move(int **matrix,int row,int col,int n)
{
	int i;
	int *neighbors;
	neighbors = validate_matrix(matrix,row,col,n);
	for(i=0;i<SQUARE;i++){
		if(neighbors[i])
			check_direction(matrix,row,col,n,i,1);
	}
}

int fill_possible_matrix(int **matrix,int n,int move)
{
	int i,j,count;
	count = 0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(matrix[i][j]==GREY){
				make_move(matrix,i,j,move);
				if(validate_state(matrix,i,j,n)){
					matrix[i][j]=POSSIBLE_MOVE;
					count++;
				}else{
					matrix[i][j]=GREY;
				}
			}
		}
	}
	return count;
}

int **fill_possible_array(int **possible_matrix,int num_child,int n)
{
	int i,j,k=0;
	int **possible_array;
	possible_array = (int **)malloc(sizeof(int *)*2);
	for(i=0;i<2;i++){
		possible_array[i] = (int *)malloc(sizeof(int )*num_child);
	}
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(POSSIBLE_MOVE==possible_matrix[i][j]){
				possible_array[0][k] = i;
				possible_array[1][k] = j;
				k++;
			}
		}
	}
	return possible_array;
}

int no_possible_move(int **matrix,int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(POSSIBLE_MOVE==matrix[i][j])
				return 0;
		}
	}
	return 1;
}

#endif
