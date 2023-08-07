#include <stdio.h>
#include <stdlib.h>
#define N 4

int dimensions[N+1] = {
	5,
	4,
	3,
	5,
	6,
};
//cost of array a = i; a < j; a++
int cost_memo[N][N] = { 0 };
//int best_cuts[N] = { 0 };
int optimal_cost(int* dimensions, int i, int j)
{
	if (cost_memo[i][j] > 0) return cost_memo[i][j];
	int min_cost = -1;
	//int min_cut = -1;
	if (j - i == 2) {
		min_cost = dimensions[i]*dimensions[i]*dimensions[j];
	}
	for (int cut = 2; cut < (j - i) - 2; cut++){
		int cost = dimensions[i]*dimensions[cut]*dimensions[j];
		cost += optimal_cost(dimensions, i, i+cut);
		cost += optimal_cost(dimensions, i+cut, j);
		if (cost < min_cost) {
			min_cost = cost;
			//min_cut = i + cut;
		}
	}
	cost_memo[i][j] = min_cost;
	return min_cost;
}

int main()
{
	printf("%d\n", optimal_cost(dimensions, 0, N));
	return 0;
}

