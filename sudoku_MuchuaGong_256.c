/*
 ============================================================================
 Name        : HW2.c
 Author      : Muchuan Gong
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#define GRID_SIZE 9
#define NUM_THREADS 11
typedef struct{
	int row;
	int column;
	int tid;
} parameters;

int arry[NUM_THREADS]={1,1,1,1,1,1,1,1,1,1,1};
/*initialize the variables needed*/
	int grid[GRID_SIZE][GRID_SIZE]={{6,5,3,1,2,8,7,9,4},{1,7,4,3,5,9,6,8,2},{9,2,8,4,6,7,5,3,1},
			{2,8,6,5,1,4,3,7,9},{3,9,1,7,8,2,4,5,6},{5,4,7,6,9,3,2,1,8},
			{8,6,5,2,3,1,9,4,7},{4,1,2,9,7,5,8,6,3},{7,3,9,8,4,6,1,2,5}};

void *th_col(void *data1);
void *th_row(void *data1);
void *th_matrix(void *data);




int main(void) {
	pthread_t tid[NUM_THREADS];

	bool flag = true;
	/*print out the greeting and current matrix*/
	printf("CS149 Sudoku from Muchuan Gong\n");

	for(int i = 0; i < GRID_SIZE;i++){
		for(int j = 0; j < GRID_SIZE; j++){
			printf("%d,",grid[i][j]);
		}
		printf("\n");
	}

	parameters *data[NUM_THREADS] ;

	for(int i = 0; i <NUM_THREADS;i++){
		data[i]=(parameters *)malloc(sizeof(parameters));
	}
	for (int i = 0; i < NUM_THREADS; i++){
		if(i<2){
			data[i]->column = 0;
			data[i]->row = 0;
			data[i]->tid = i;
			if(i ==0){
				pthread_create(&tid[i],NULL, th_col,data[i]);

			}
			else {
				pthread_create(&tid[i],NULL, th_row,data[i]);
			}
		}
		else{
				for(int j = 0; j < 3;j++){
					data[i+j]->row =(i-2);
					data[i+j]->column =j*3;
					data[i+j]->tid=i+j;

					pthread_create(&tid[i+j],NULL, th_matrix,data[i+j]);
				}
				i+=2;
			}

	}

	for(int i  = 0; i<NUM_THREADS;i++){
		pthread_join(tid[i],NULL);

	}


	for(int i = 0; i<NUM_THREADS;i++){
		//printf("%d",arry[i]);
			free(data[i]);

		}

	for(int i = 0; i < NUM_THREADS;i++){

		if(arry[i]==0){
			flag = false;
		}
	}

	if(flag==false) printf("Not Valid\n");
	else printf("valid\n");

}


void *th_col(void *data1){

	parameters *data = (parameters *)data1;
	while(data->column < GRID_SIZE){
		int temp[GRID_SIZE] = {0};
		while(data->row<GRID_SIZE){
			int te = grid[data->row][data->column];
				te--;
				temp[te]++;
			data->row++;
		}
		while(data->row!=0){
			data->row--;
			if(temp[data->row]>=2){
				arry[data->tid]=0;

				pthread_exit(0);
			}

		}
		data->column++;
	}

	pthread_exit(0);
}
void *th_row(void *data1){

	parameters *data = (parameters *)data1;
	while(data->row<GRID_SIZE){
			int temp[GRID_SIZE] = {0};
			while(data->column<GRID_SIZE){
				int te = grid[data->row][data->column];
				te--;
				temp[te]++;
				data->column++;
			}
			while(data->column!=0){
				data->column--;
				if(temp[data->row]>=2){
					printf("??????\n");
					arry[data->tid]=0;
					pthread_exit(0);
				}
			}
			data->row++;
		}

	pthread_exit(0);
}

void *th_matrix(void *data1){

	parameters *data = (parameters *)data1;

	int temp[GRID_SIZE] = {0};


	for(int i = 0; i < 3;i++){
		for(int j = 0; j < 3;j++){
			temp[grid[data->row+i][data->column+j]-1]++;
		}
	}

	for(int i = 0; i < GRID_SIZE;i++){
		if(temp[i]>=2){
			arry[data->tid]=0;
			pthread_exit(0);
		}
	}
	pthread_exit(0);
}
