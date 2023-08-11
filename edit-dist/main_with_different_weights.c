#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum Action {REPLACE, COPY, DELETE, TWIDDLE, INSERT, KILL};

int mmED(const char *source, const char *destination, const int *costof)
{
	const int m = strlen(destination);
	const int n = strlen(source);

	int (*memo)[m+1] = malloc(sizeof(int[n+1][m+1]));

	memo[0][0] = 0;
	for (int i = 1; i <= m; i++){
		memo[0][i] = i*costof[INSERT];
	}
	for (int i = 1; i <= n; i++){
		memo[i][0] = i*costof[DELETE];
	}

	for (int i = 1; i <= n; i++){
	for (int j = 1; j <= m; j++){
		//replace
		int replace_cost = costof[REPLACE] + memo[i-1][j-1];
		int lowest_cost = replace_cost;

		//copy
		if (source[i-1] == destination[j-1]){
			int copy_cost = costof[COPY] + memo[i-1][j-1];
			if (copy_cost < lowest_cost) lowest_cost = copy_cost;
		}
		//delete
		int delete_cost = costof[DELETE] + memo[i-1][j];
		if (delete_cost < lowest_cost) lowest_cost = delete_cost;

		//twiddle
		if ((i >= 2) && (j >= 2) && (source[i-1] == destination[j-2]) && (source[i-2] == destination[j-1])){
			int twiddle_cost = costof[TWIDDLE] + memo[i-2][j-2];
			if (twiddle_cost < lowest_cost) lowest_cost = twiddle_cost;
		}
		//insert
		int insert_cost = costof[INSERT] + memo[i][j-1];
		if (insert_cost < lowest_cost) lowest_cost = insert_cost;

		memo[i][j] = lowest_cost;
	}}

	//kill
	int lowest_cost = memo[n][m];
	//int best_action = 100;
	for (int i = 0; i < n; i++){
		int kill_cost = costof[KILL] + memo[i][m];
		if (kill_cost < lowest_cost) lowest_cost = kill_cost;
	}

	return lowest_cost;
}

int main(int argc, char **argv)
{
	int costof[6];
	costof[COPY]       = 1;
	costof[REPLACE]    = 100;
	costof[DELETE]     = 10;
	costof[TWIDDLE]    = 10;
	costof[INSERT]     = 500;
	costof[KILL]       = 200;
	if (argc <= 1) {
		fprintf(stderr, "mmED: No arguments.\n");
		return 1;
	}
	//printf("> ");
	//char input[50];
	//scanf("%s", input);
	for (int i = 2; i < argc; i++){
		printf("%s - %d\n", argv[i], mmED(argv[i], argv[1], costof));
	}
	printf("Done.\n");
	return 0;
}

