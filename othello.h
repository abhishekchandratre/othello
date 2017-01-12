#ifndef OTHELLO_H_
#define OTHELLO_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "helper.h"
#include "validation.h"
#define SCALE 3
#define SCORE_BOARD_WIDTH 40
#define COLOR_POSSIBLE_SELECTED 2
#define COLOR_POSSIBLE 3
#define COLOR_LAST_LIGHT 4
#define COLOR_LIGHT 5
#define COLOR_DARK 6
#define COLOR_GREY 1

#define COLOR_SELECTED 7
#define COLOR_SCOREBOARD 8

#define COLOR_WHITE_WHITE 9
#define COLOR_WHITE_MAGENTA 10

/*Function would initialize a game state with given size n*/
/*Where WHITE is 0*/
/*BLACK is 1*/
/*GREY is -1*/

static WINDOW ***windows;
static WINDOW *score_board;
static WINDOW *player1, *player2;

WINDOW *create_window(int height,int width, int y,int x)
{
	WINDOW *local_win;
	local_win = newwin(height,width,y,x);
	box(local_win,0,0);
	wbkgd(local_win,COLOR_PAIR(COLOR_GREY));
	wrefresh(local_win);
	return local_win;
}

void initialize_windows(int n)
{
	int i,j;
	int height,width;
	int startx,starty;
	int x,y;
	height = SCALE*n;
	width = (SCALE + 4)*n;
	starty = (LINES - height) / 2;
	startx = (COLS - width) / 2;
	x = startx;
	y = starty;
	windows = (WINDOW ***)malloc(sizeof(WINDOW **)*n);
	for(i=0;i<n;i++){
		windows[i] = (WINDOW **)malloc(sizeof(WINDOW *)*n);
	}
	for(i=0;i<n;i++){
		x = startx;
		for(j=0;j<n;j++){
			windows[i][j] = create_window(height/n,width/n,y,x);
			x = x + width/n;
		}
		y = y + height/n;
	}
	refresh();
}

void initialize_colors()
{
	init_pair(COLOR_GREY,COLOR_BLUE, COLOR_GREEN);
	init_pair(COLOR_POSSIBLE_SELECTED,COLOR_BLACK, COLOR_CYAN);
	init_pair(COLOR_POSSIBLE,COLOR_BLACK, COLOR_YELLOW);
	init_pair(COLOR_LAST_LIGHT,COLOR_RED, COLOR_WHITE);
	init_pair(COLOR_LIGHT,COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOR_DARK,COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_SELECTED,COLOR_WHITE, COLOR_CYAN);
	init_pair(COLOR_SCOREBOARD,COLOR_BLACK, COLOR_BLUE);
	init_pair(COLOR_WHITE_WHITE,COLOR_WHITE, COLOR_WHITE);
	init_pair(COLOR_WHITE_MAGENTA,COLOR_BLACK, COLOR_MAGENTA);
}

void initialize_scoreboard(int n)
{
	int height,width;
	char score_board_str[] = "SCOREBOARD";
	char player1_str[] = "PLAYER 1";
	char player2_str[] = "OTHELLO";
	height = SCALE*n;
	width = SCORE_BOARD_WIDTH;
	score_board = newwin(3,width,2,(COLS - width)/2);
	box(score_board,0,0);
	mvwprintw(score_board,1,(width - strlen(score_board_str))/2,score_board_str);
	wbkgd(score_board,COLOR_PAIR(COLOR_SCOREBOARD));
	//Initialize player 1 board
	player1 = newwin(4,width/2,5,(COLS - width)/2);
	box(player1,0,0);
	mvwprintw(player1,1,(width/2 - strlen(player1_str))/2,player1_str);
	wbkgd(player1,COLOR_PAIR(COLOR_SCOREBOARD));
	//Initialize othello board
	player2 = newwin(4,width/2,5,COLS/2);
	box(player2,0,0);
	mvwprintw(player2,1,(width/2 - strlen(player2_str))/2,player2_str);
	wbkgd(player2,COLOR_PAIR(COLOR_SCOREBOARD));
	//Refesh window
	wrefresh(score_board);
	wrefresh(player1);
	wrefresh(player2);
}

void update_scoreboard(int move,int dark_count,int light_count)
{
	if(DARK==move){
		wbkgd(player1,COLOR_PAIR(COLOR_SELECTED));
		wbkgd(player2,COLOR_PAIR(COLOR_SCOREBOARD));
	}else{
		wbkgd(player2,COLOR_PAIR(COLOR_SELECTED));
		wbkgd(player1,COLOR_PAIR(COLOR_SCOREBOARD));
	}
	mvwprintw(player1,2,(SCORE_BOARD_WIDTH/2 - 3)/2,"%d",dark_count);
	mvwprintw(player2,2,(SCORE_BOARD_WIDTH/2 - 3)/2,"%d",light_count);
	wnoutrefresh(player1);
	wnoutrefresh(player2);
	refresh();
}

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


void print_in_middle(WINDOW *window,int print_char)
{
	wclear(window);
	box(window,0,0);
	wmove(window,SCALE/2,(SCALE + 4)/2);
	if(print_char)
		waddch(window,ACS_CKBOARD);
}

void move_possible_selected(int row,int col)
{
	wbkgd(windows[row][col],COLOR_PAIR(COLOR_POSSIBLE_SELECTED));
	print_in_middle(windows[row][col],1);
}

void move_possible(int row,int col)
{
	wbkgd(windows[row][col],COLOR_PAIR(COLOR_POSSIBLE));
	print_in_middle(windows[row][col],0);
}

void move_last_light(int row,int col)
{
	wbkgd(windows[row][col],COLOR_PAIR(COLOR_LAST_LIGHT));
	print_in_middle(windows[row][col],1);
}

void move_light(int row,int col)
{
	wbkgd(windows[row][col],COLOR_PAIR(COLOR_LIGHT));
	print_in_middle(windows[row][col],1);
}

void move_dark(int row,int col)
{
	wbkgd(windows[row][col],COLOR_PAIR(COLOR_DARK));
	print_in_middle(windows[row][col],1);
}

void move_grey(int row,int col)
{
	wbkgd(windows[row][col],COLOR_PAIR(COLOR_GREY));
	print_in_middle(windows[row][col],0);
}

void print_matrix_with_windows(int **matrix,int row,int col,int curr_row,int curr_col,int n)
{
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(matrix[i][j]==GREY){
				//non explored moves
				move_grey(i,j);
			}
			else if(matrix[i][j]==POSSIBLE_MOVE){
				if((i==curr_row)&&(j==curr_col)){
					//select move
					move_possible_selected(i,j);
				}else{
					move_possible(i,j);
				}
			}
			else if(matrix[i][j]==LIGHT){
				if((i==row)&&(j==col)){
					//last AI move
					move_last_light(i,j);
				}else{
					//Other AI tiles
					move_light(i,j);
				}
			}
			else if(matrix[i][j]==DARK){
				//Human moves
				move_dark(i,j);
			}
			wnoutrefresh(windows[i][j]);
		}
	}
	refresh();
}
#endif
