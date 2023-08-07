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

//X source, Y destination
int rED(char* source, int n, char* destination, int m, int* costof)
{
	if (n <= 0) return strlen(destination) * costof[INSERT];
	if (m <= 0) return strlen(source) * costof[DELETE];

	//replace
	int replace_cost = costof[REPLACE] + rED(source, n-1, destination, m-1, costof);
	int lowest_cost = replace_cost;
	enum Action best_action = REPLACE;

	//copy
	if (source[n-1] == destination[m-1]){
		int copy_cost = costof[COPY] + rED(source, n-1, destination, m-1, costof);
		argcompare(&lowest_cost, &best_action, copy_cost, COPY);
	}
	//delete
	int delete_cost = costof[DELETE] + rED(source, n-1, destination, m, costof);
	argcompare(&lowest_cost, &best_action, delete_cost, DELETE);

	//twiddle
	if ((source[n-1] == destination[m-2]) && (source[n-2] == destination[m-1])){
		int twiddle_cost = costof[TWIDDLE] + rED(source, n-2, destination, m-2, costof);
		argcompare(&lowest_cost, &best_action, twiddle_cost, TWIDDLE);
	}
	//insert
	int insert_cost = costof[INSERT] + rED(source, n, destination, m-1, costof);
	argcompare(&lowest_cost, &best_action, insert_cost, INSERT);

	printf("ba: %d\n", best_action);
	return lowest_cost;
}

int main()
{
	char* X = "xyz";
	char* Y = "abcqdef";
	int costof[6];
	costof[COPY]       = 1;
	costof[REPLACE]    = 70;
	costof[DELETE]     = 100;
	costof[TWIDDLE]    = 10;
	costof[INSERT]     = 0;
	costof[KILL]       = 0;
	int edit_distance = rED(X, strlen(X), Y, strlen(Y), costof);
	printf("%d\n", edit_distance);
}

