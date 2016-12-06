#include "othello.h"
#include "astar.h"

void process_game(int **matrix,int n);
int main(int argc,char *argv[])
{
	int n;
	int **matrix;
	if(2!=argc){
		printf("Usage:./a.out <number_of_boxes >= 4>\n");
		exit(1);
	}
	initscr();
	start_color();
	n = atoi(argv[1]);
	initialize_colors();
	initialize_windows(n);
	matrix = initialize_matrix(n);
	initializ_root_possible_matrix_for_best_move(n);
	process_game(matrix,n);
	free_matrix(matrix,n);
	getch();
	endwin();
	return 0;
}

void process_game(int **matrix,int n)
{
	int move,row,col,reset,possible_values;
	int **possible_matrix;
	int no_move[2],finish,best_move[2];
	char ch=0;

	move=DARK;
	row=col=reset=finish=0;
	possible_matrix = initialize_matrix(n);
	while(1){
		while(1){
			if(reset){
				/*If move was invalid*/
				matrix[row][col]=GREY;
			}
			copy_matrix(matrix,possible_matrix,n);
			possible_values = fill_possible_matrix(possible_matrix,n,move);
			if(no_possible_move(possible_matrix,n)){
				no_move[move]=1;
				if(no_move[0]==1){
					if(no_move[0]==no_move[1]){
						finish = 1;
						break;
					}
				}
				mvprintw(LINES-1,0,"Pass to other player no possible move");
				break;
			}else{
				no_move[0] = no_move[1] = -1;
				if(LIGHT==move){
					get_best_move(matrix,move,n,best_move);
					mvprintw(LINES-3,0,"Printing best move [%d] [%d].",best_move[0],best_move[1]);
					row = best_move[0];
					col = best_move[1];
				}
			}
			/*1 player mode*/
			if(move==DARK){
				int curr_row,curr_col;
				int **possible_array;
				int counter=0;
				possible_array = fill_possible_array(possible_matrix,possible_values,n);
				//set here row and col
				while(1){
					if(counter==possible_values){
						counter = 0;
					}
					curr_row = possible_array[0][counter];
					curr_col = possible_array[1][counter];
					mvprintw(LINES-2,0,"Player1 Play ");
					print_matrix_with_windows(possible_matrix,row,col,curr_row,curr_col,n);
					ch = getch();
					if(ch=='y')
						break;
					counter++;
				}
				row = curr_row;
				col = curr_col;
				free_2d_array(possible_array,2);
			}
			if(GREY!=matrix[row][col]){
				mvprintw(LINES-4,0,"\nInvalid Move");
				continue;
			}
			make_move(matrix,row,col,move);
			if(validate_state(matrix,row,col,n)){
				reset = 0;
				confirm_move(matrix,row,col,n);
				break;
			}else{
				/*printf("\n Invalid move please try again");*/
				reset = 1;
			}
		}
		if(finish){
			int winner;
			winner = get_winner(matrix,n);
			mvprintw(LINES-5,0,"\nWinner is %d",winner);
			break;
		}
		flip_move(&move);
	}
	free_matrix(possible_matrix,n);
}
