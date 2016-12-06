#ifndef ASTAR_H_
#define ASTAR_H_
#include <stdio.h>
#include <limits.h>
#include "validation.h"
#include "othello.h"
#include <sys/time.h>

#define WORST -INT_MAX
#define BEST INT_MAX
#define INITIAL_DEPTH 3
#define TIME_LIMIT 2


typedef struct node{
	int id;			//deb_id
	int move;		//DARK or LIGHT
	int utility;		//Utility value of a node
	int depth;		//depth at which current node is at
	struct node *parent;	//Parent node address
	int number_of_childs;	//This will hold number of child node has
	int row,col;		//Row and col of newly placed position
	int alpha;		//alpha = best already explored option for max
	int beta;		//beta = best already explored option for min
	struct node **child_nodes;//pointer to hold child nodes
	int purned;		//This field would tell us whether the node is purned
}node;

static int max_depth;		//Use this value to set initial depth
static int **possible_matrix;	//This matrix will store possible value for current matrix in consideration
static int **root_matrix;	//Parent matrix to store root matrix which will be used to calculate current state
static int **matrix;		//Use this matrix, dont change root matrix
static int n;			//the size which never changes
static int max;			//Max of this iteration

//Debug variable
int deb_count = 0;
int deb_pruned = 0;

void initializ_root_possible_matrix_for_best_move(int num)
{
	possible_matrix = initialize_matrix(num);
	root_matrix = initialize_matrix(num);
	matrix = initialize_matrix(num);
	n = num;
}

void fill_matrix_from_root(node *state)
{
	int i=0;
	node *head;
	node **list_nodes;
	list_nodes = (node **)malloc(sizeof(node *)*max_depth);
	head = state;
	copy_matrix(root_matrix,matrix,n);
	while(head->parent){
		list_nodes[i] = head;
		head = head->parent;
		i++;
	}
	i--;
	while(i>=0){
		head = list_nodes[i];
		make_move(matrix,head->row,head->col,head->parent->move);
		confirm_move(matrix,head->row,head->col,n);
		i--;
	}
	free(list_nodes);
}

void print_node_info(node *state){
	int i;
	if(state->parent)
		printf("\n*+[%d]-p[%d] m[%d] nc[%d]-->",state->id,state->parent->id,state->move,state->number_of_childs);
	else
		printf("\n[%d]-p[-] m[%d] nc[%d]",state->id,state->move,state->number_of_childs);
	for(i=0;i<state->number_of_childs;i++){
		printf(" %d ",state->child_nodes[i]->id);
	}
	printf("+*");
}

void print_all_nodes(node *state)
{
	int i;
	print_node_info(state);
	for(i=0;i<state->number_of_childs;i++){
		print_all_nodes(state->child_nodes[i]);
	}
}

node *new_state(int move,int depth, node *parent_node)
{
	node *temp;
	temp = (node *)malloc(sizeof(node));
	temp->id = deb_count;
	deb_count++;
	temp->row = temp->col = 0;
	if(max==move){
		temp->utility = WORST;
	}else{
		temp->utility = BEST;
	}
	temp->move = move;
	temp->depth = depth;
	temp->number_of_childs = 0;
	temp->alpha = WORST;
	temp->beta = BEST;
	temp->child_nodes = NULL;
	temp->purned = 0;
	temp->parent = parent_node;
	return temp;
}

void reset_utility(node *state)
{
	if(max==state->move){
		state->utility = WORST;
	}else{
		state->utility = BEST;
	}
}

void initialize_place_for_child_nodes(node *state)
{
	state->child_nodes = (node **)malloc(sizeof(node *)*state->number_of_childs);
}

int get_corner_count(node *state)
{
	int corner_count=0;
	if(OUT_BOUND==get_from_direction(matrix,state->row,state->col,n,TOP)){
		corner_count++;
	}
	if(OUT_BOUND==get_from_direction(matrix,state->row,state->col,n,LEFT)){
		corner_count++;
	}
	if(OUT_BOUND==get_from_direction(matrix,state->row,state->col,n,RIGHT)){
		corner_count++;
	}
	if(OUT_BOUND==get_from_direction(matrix,state->row,state->col,n,BOTTOM)){
		corner_count++;
	}
	return corner_count;
}

void fill_utility(node *state)
{
	int max_count,min_count;
	int max_moves,min_moves;
	int corner_count=0;
	int min;
	min = max;
	flip_move(&min);
	state->utility = 0;
	fill_matrix_from_root(state);
	//get normal win points
	max_count=get_matrix_count(matrix,n,max);
	min_count=get_matrix_count(matrix,n,min);
	state->utility += ((max_count - min_count)/(max_count + min_count)) * 100;
	//try to get to corner
	corner_count = get_corner_count(state);
	state->utility += corner_count==2?30:0;
	//check mobility of our and opponent
	max_moves = fill_possible_matrix(matrix,n,max);
	min_moves = fill_possible_matrix(matrix,n,min);
	if(max_moves+min_moves)
		state->utility += ((max_moves-min_moves)/(max_moves + min_moves)) * 100;
}

int is_terminal(node *state)
{
	if(max_depth==state->depth){
		return 1;
	}
	if(state->purned){
		return 1;
	}
	return 0;
}

int generate_successors(node *state)
{
	int i;
	int index;
	int alt_move;
	int **possible_array;
	node *temp;
	reset_utility(state);
	if(is_terminal(state)){
		//Now will have to fill utility
		fill_utility(state);
		return -1;
	}
	if(!state->child_nodes){
		fill_matrix_from_root(state);
		copy_matrix(matrix,possible_matrix,n);
		state->number_of_childs = fill_possible_matrix(possible_matrix,n,state->move);
		//printf("\nPrinting for [%d]",state->id);
		//print_matrix(possible_matrix,n);
		possible_array = fill_possible_array(possible_matrix,state->number_of_childs,n);
		initialize_place_for_child_nodes(state);
		alt_move = state->move;
		flip_move(&alt_move);
		for(i=0;i<state->number_of_childs;i++){
			if(max==alt_move){
				temp = new_state(alt_move,state->depth + 1,state);
			}
			else{
				temp = new_state(alt_move,state->depth + 1,state);
			}
			temp->row = possible_array[0][i];
			temp->col = possible_array[1][i];
			state->child_nodes[i] = temp;
			//fill alpha and beta properly
		}
		free_2d_array(possible_array,2);
	}
	//else{
		//printf("\n[%d]has child nodes",state->id);
	//}
	if(0==state->number_of_childs){
		state->purned = 1;
		fill_utility(state);
		return -1;
	}
	for(i=0;i<state->number_of_childs;i++){
		if(max==state->move){
			if(state->utility<state->beta){
				generate_successors(state->child_nodes[i]);
				if(state->utility<state->child_nodes[i]->utility){
					state->utility = state->child_nodes[i]->utility;
					state->alpha = state->utility;
					index = i;
				}
			}else{
				state->child_nodes[i]->purned = 1;
				deb_pruned++;
			}
		}else{
			if(state->utility>state->alpha){
				generate_successors(state->child_nodes[i]);
				if(state->utility>state->child_nodes[i]->utility){
					state->utility = state->child_nodes[i]->utility;
					state->beta = state->utility;
					index = i;
				}
			}else{
				//maximizer has better option available
				deb_pruned++;
				state->child_nodes[i]->purned = 1;
			}
		}
	}
	//printf("\n[%d] utility[%d]",state->id,state->utility);
	return index;
}

int search_tree(node *state)
{
	int i,index;
	int min,max;
	if(is_terminal(state)){
		fill_utility(state);
		return -1;
	}
	for(i=0;i<state->number_of_childs;i++){
		search_tree(state->child_nodes[i]);
	}
	min = BEST;
	max = WORST;
	index = -1;
	for(i=0;i<state->number_of_childs;i++){
		if(DARK==state->move){
			if(max<state->child_nodes[i]->utility){
				max = state->child_nodes[i]->utility;
				index = i;
			}
		}else{
			if(min>state->child_nodes[i]->utility){
				min = state->child_nodes[i]->utility;
				index = i;
			}
		}
	}
	if(DARK==state->move){
		state->utility = max;
	}else{
		state->utility = min;
	}
	return index;
}

void free_tree(node *state)
{
	int i;
	if(is_terminal(state)){
		free(state);
		return;
	}
	for(i=0;i<state->number_of_childs;i++){
		free_tree(state->child_nodes[i]);
	}
	free(state);
}

//Params
//matrix is possible matrix which contains possible fields
//return the result in best_move[0] best_move[1]
void get_best_move(int **src_matrix,int move,int n,int *best_move)
{
	node *state;
	int best_state;
	int last_deb_count=-1;
	struct timeval start,end;
	double elapsed=0;
	max_depth = 3;
	deb_count = 0;
	max = move;
	copy_matrix(src_matrix,root_matrix,n);
	state = new_state(move,1,NULL);
	gettimeofday(&start,NULL);
	while(elapsed < TIME_LIMIT){
		best_state = generate_successors(state);
		if(last_deb_count==deb_count){
			mvprintw(0,0,"\n[%d]-[%d]No new node max depth-%d pruned-%d",state->child_nodes[best_state]->row,state->child_nodes[best_state]->col,max_depth,deb_pruned);
			break;
		}
		last_deb_count = deb_count;
		//best_state = search_tree(state);
		max_depth = max_depth + 1;
		//printf("\n[%d]-[%d]increasing max depth to %d pruned %d",state->child_nodes[best_state]->row,state->child_nodes[best_state]->col,max_depth,deb_pruned);
		gettimeofday(&end,NULL);
		elapsed = (end.tv_sec - start.tv_sec) +
					((end.tv_usec - start.tv_usec)/1000000.0);
	}
	mvprintw(0,0,"[%d]-[%d]Time limit max depth-%d pruned-%d",state->child_nodes[best_state]->row,state->child_nodes[best_state]->col,max_depth,deb_pruned);
	free_tree(state);
	best_move[0] = state->child_nodes[best_state]->row;
	best_move[1] = state->child_nodes[best_state]->col;
}
#endif
