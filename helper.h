#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include <stdlib.h>

//Possible state of chip
#define DARK 1
#define LIGHT 0
#define GREY -1
#define OUT_BOUND -2
#define POSSIBLE_MOVE -3

#define TOP_LEFT 0
#define TOP 1
#define TOP_RIGHT 2
#define LEFT 3
#define SELF 4
#define RIGHT 5
#define BOTTOM_LEFT 6
#define BOTTOM 7
#define BOTTOM_RIGHT 8

#define SQUARE 9

//colors
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int get_self(int **matrix,int row,int col,int n)
{
	return matrix[row][col];
}

int get_left(int **matrix,int row,int col,int n)
{
	if((col-1)<0)
		return OUT_BOUND;
	return matrix[row][col-1];
}

int get_right(int **matrix,int row,int col,int n)
{
	if((col+1)>=n)
		return OUT_BOUND;
	return matrix[row][col+1];
}

int get_top(int **matrix,int row,int col,int n)
{
	if((row-1)<0)
		return OUT_BOUND;
	return matrix[row-1][col];
}

int get_bottom(int **matrix,int row,int col,int n)
{
	if((row+1)>=n)
		return OUT_BOUND;
	return matrix[row+1][col];
}

int get_top_left(int **matrix,int row,int col,int n)
{
	if((row-1)<0)
		return OUT_BOUND;
	if((col-1)<0)
		return OUT_BOUND;
	return matrix[row-1][col-1];
}

int get_top_right(int **matrix,int row,int col,int n)
{
	if((row-1)<0)
		return OUT_BOUND;
	if((col+1)>=n)
		return OUT_BOUND;
	return matrix[row-1][col+1];
}

int get_bottom_left(int **matrix,int row,int col,int n)
{
	if((row+1)>=n)
		return OUT_BOUND;
	if((col-1)<0)
		return OUT_BOUND;
	return matrix[row+1][col-1];
}

int get_bottom_right(int **matrix,int row,int col,int n)
{
	if((row+1)>=n)
		return OUT_BOUND;
	if((col+1)>=n)
		return OUT_BOUND;
	return matrix[row+1][col+1];
}

int get_from_direction(int **matrix,int row,int col,int n,int direction)
{
	int val;
	switch(direction){
		case TOP_LEFT:
			val = get_top_left(matrix,row,col,n);
			break;
		case TOP:
			val = get_top(matrix,row,col,n);
			break;
		case TOP_RIGHT:
			val = get_top_right(matrix,row,col,n);
			break;
		case LEFT:
			val = get_left(matrix,row,col,n);
			break;
		case SELF:
			val = get_self(matrix,row,col,n);
			break;
		case RIGHT:
			val = get_right(matrix,row,col,n);
			break;
		case BOTTOM_LEFT:
			val = get_bottom_left(matrix,row,col,n);
			break;
		case BOTTOM:
			val = get_bottom(matrix,row,col,n);
			break;
		case BOTTOM_RIGHT:
			val = get_bottom_right(matrix,row,col,n);
			break;
	}
	return val;
}

int get_from_movement(int **matrix,int *row,int *col,int n,int direction)
{
	int val;
	if((*row<0)||(*row>=n))
		return OUT_BOUND;
	if((*col<0)||(*col>=n))
		return OUT_BOUND;
	switch(direction){
		case TOP_LEFT:
			val = get_top_left(matrix,*row,*col,n);
			*row = *row - 1;
			*col = *col - 1;
			break;
		case TOP:
			val = get_top(matrix,*row,*col,n);
			*row = *row - 1;
			break;
		case TOP_RIGHT:
			val = get_top_right(matrix,*row,*col,n);
			*row = *row - 1;
			*col = *col + 1;
			break;
		case LEFT:
			val = get_left(matrix,*row,*col,n);
			*col = *col - 1;
			break;
		case SELF:
			val = get_self(matrix,*row,*col,n);
			break;
		case RIGHT:
			val = get_right(matrix,*row,*col,n);
			*col = *col + 1;
			break;
		case BOTTOM_LEFT:
			val = get_bottom_left(matrix,*row,*col,n);
			*row = *row + 1;
			*col = *col - 1;
			break;
		case BOTTOM:
			val = get_bottom(matrix,*row,*col,n);
			*row = *row + 1;
			break;
		case BOTTOM_RIGHT:
			val = get_bottom_right(matrix,*row,*col,n);
			*row = *row + 1;
			*col = *col + 1;
			break;
	}
	return val;
}

int *get_neighbors(int **matrix,int row,int col,int n)
{
	int i;
	int *temp;
	temp = (int *)malloc(sizeof(int )*9);
	for(i=0;i<SQUARE;i++){
		temp[i] = get_from_direction(matrix,row,col,n,i);
	}
	return temp;
}

void free_neighbors(int *result)
{
	free(result);
}

int check_direction(int **matrix,int row,int col,int n,int direction,int flip)
{
	int my_val,val;
	int move;
	move = matrix[row][col];
	my_val = matrix[row][col];
	do{
		val = get_from_movement(matrix,&row,&col,n,direction);
		if(flip){
			matrix[row][col]=move;
		}
	}while((val!=my_val)&&(val != OUT_BOUND));
	return val;
}

void make_move(int **matrix,int row,int col,int move)
{
	matrix[row][col] = move;
}

void flip_move(int *move)
{
	if(DARK==*move)
		*move = LIGHT;
	else if(LIGHT==*move)
		*move = DARK;
	else
		*move = GREY;
}

void copy_matrix(int **source,int **destination,int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			destination[i][j] = source[i][j];
}

int **allocate_copy_matrix(int **source,int n)
{
	int i;
	int **temp;
	temp = (int **)malloc(sizeof(int *)*n);
	for(i=0;i<n;i++){
		temp[i] = (int *)malloc(sizeof(int )*n);
	}
	copy_matrix(source,temp,n);
	return temp;
}

int get_matrix_count(int **matrix,int n,int move)
{
	int count,i,j;
	count = 0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(move==matrix[i][j])
				count++;
		}
	}
	return count;
}

int get_winner(int **matrix,int n)
{
	int dark_count,light_count;
	dark_count=get_matrix_count(matrix,n,DARK);
	light_count=get_matrix_count(matrix,n,LIGHT);
	if(dark_count>light_count)
		return DARK;
	else if(dark_count<light_count)
		return LIGHT;
	else 
		return GREY;
}

void free_2d_array(int **array,int n)
{
	int i;
	for(i=0;i<n;i++){
		free(array[i]);
	}
	free(array);
}

#endif
