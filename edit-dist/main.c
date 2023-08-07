#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum Action {REPLACE, COPY, DELETE, TWIDDLE, INSERT, KILL};

void argcompare(int* minimum, int* best_argument, int candidate, int argument)
{
	if (candidate >= *minimum) return;
	*minimum = candidate;
	*best_argument = argument;
}

struct Memo2D {
	int* array;
	int bounds;
} memo;

int mED(char* source, int n, char* destination, int m, int* costof)
{
	int *val = &memo.array[n*memo.bounds + m];
	if (*val >= 0) return *val;
	if ((n <= 0) && (m <= 0)) {
		*val = 0;
		return 0;
	}
	if (n <= 0){
		*val = strlen(destination) * costof[INSERT];
		return *val;
	}
	if (m <= 0){
		*val = strlen(source) * costof[DELETE];
		return *val;
	}

	//replace
	int replace_cost = costof[REPLACE] + mED(source, n-1, destination, m-1, costof);
	int lowest_cost = replace_cost;
	enum Action best_action = REPLACE;

	//copy
	if (source[n-1] == destination[m-1]){
		int copy_cost = costof[COPY] + mED(source, n-1, destination, m-1, costof);
		argcompare(&lowest_cost, &best_action, copy_cost, COPY);
	}
	//delete
	int delete_cost = costof[DELETE] + mED(source, n-1, destination, m, costof);
	argcompare(&lowest_cost, &best_action, delete_cost, DELETE);

	//twiddle
	if ((source[n-1] == destination[m-2]) && (source[n-2] == destination[m-1])){
		int twiddle_cost = costof[TWIDDLE] + mED(source, n-2, destination, m-2, costof);
		argcompare(&lowest_cost, &best_action, twiddle_cost, TWIDDLE);
	}
	//insert
	int insert_cost = costof[INSERT] + mED(source, n, destination, m-1, costof);
	argcompare(&lowest_cost, &best_action, insert_cost, INSERT);

	//printf("ba: %d\n", best_action);
	*val = lowest_cost;
	return lowest_cost;
}

//lol
	//for (int i = 0; i < 10000; i++) memo = malloc(10000000000);

int mmED(const char* source, const char* destination, const int* costof)
{
	const int m = strlen(destination);
	const int n = strlen(source);

	int (*memo)[m+1] = malloc(sizeof(int[n+1][m+1]));
	char (*action)[m+1] = malloc(sizeof(char[n+1][m+1]));

	memo[0][0] = 0;
	action[0][0] = 100; //supposed to be undef?
	for (int i = 1; i <= m; i++){
		memo[0][i] = i*costof[INSERT];
		action[0][i] = INSERT;
	}
	for (int i = 1; i <= n; i++){
		memo[i][0] = i*costof[DELETE];
		action[i][0] = KILL;
	}

	for (int i = 1; i <= n; i++){
	for (int j = 1; j <= m; j++){
		//replace
		int replace_cost = costof[REPLACE] + memo[i-1][j-1];
		int lowest_cost = replace_cost;
		enum Action best_action = REPLACE;

		//copy
		if (source[i-1] == destination[j-1]){
			int copy_cost = costof[COPY] + memo[i-1][j-1];
			argcompare(&lowest_cost, &best_action, copy_cost, COPY);
		}
		//delete
		int delete_cost = costof[DELETE] + memo[i-1][j];
		argcompare(&lowest_cost, &best_action, delete_cost, DELETE);

		//twiddle
		if ((i >= 2) && (j >= 2) &&
				(source[i-1] == destination[j-2]) && (source[i-2] == destination[j-1])){
			int twiddle_cost = costof[TWIDDLE] + memo[i-2][j-2];
			argcompare(&lowest_cost, &best_action, twiddle_cost, TWIDDLE);
		}
		//insert
		int insert_cost = costof[INSERT] + memo[i][j-1];
		argcompare(&lowest_cost, &best_action, insert_cost, INSERT);

		memo[i][j] = lowest_cost;
		action[i][j] = best_action;
	}}

	//kill
	int lowest_cost = memo[n][m];
	//int best_action = 100;
	for (int i = 0; i < n; i++){
		int calculated_cost = costof[KILL] + memo[i][m];
		if (calculated_cost < lowest_cost){
			lowest_cost = calculated_cost;
			//best_action = KILL;
		}
	}

	return lowest_cost;
}

int main()
{
	char* X = "Dinosaur";
	char* Y = "Parrot";
	int costof[6];
	costof[COPY]       = 0;
	costof[REPLACE]    = 100;
	costof[DELETE]     = 100;
	costof[TWIDDLE]    = 10;
	costof[INSERT]     = 1000;
	costof[KILL]       = 10;
	//recursion setup
	{
		int size = (strlen(X) + 1)*(strlen(Y) + 1);
		size_t* bytesize = sizeof(int)*size;
		printf("memo size - %d\n", size);
		memo.array = malloc(bytesize);
		for (int i = 0; i < size; i++) memo.array[i] = -1;
		memo.bounds = strlen(Y) + 1;
	}
	int ed  = mED(X, strlen(X), Y, strlen(Y), costof);
	int ed2 =mmED(X, Y, costof);
	//printf("There once was a man\n");
	printf("%d vs %d\n", ed, ed2);
}

