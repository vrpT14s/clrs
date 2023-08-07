#include <stdio.h>
#include "bootleg_vector.h"

int r = 0;
int calculate_best_price(int n, int* prices)
{
	r++;
	int max = prices[n - 1];
	for (int cut = 1; cut < n / 2 + 1; cut++){
		int calculated_price = calculate_best_price(n - cut, prices) + calculate_best_price(cut, prices);
		if (calculated_price > max) max = calculated_price;
	}
	return max;
}

int prices[10] = {
	1,
	5,
	8,
	9,
	10,
	17,
	17,
	20,
	24,
	30,
};


int m = 0;
int best_prices[10] = { 0 };
int fake(int n, int* prices)
{
	m++;
	if (best_prices[n - 1] > 0) return best_prices[n-1];
	int max = prices[n - 1];
	for (int cut = 1; cut < n/2 + 1; cut++){
		int calculated_price = cbr_memo(n - cut, prices) + cbr_memo(cut, prices);
		if (calculated_price > max) max = calculated_price;
	}
	best_prices[n-1] = max;
	return max;
}

stack_t* stack = NULL;
int cutlist[10] = { 0 };
int cbr_memo(int n, int* prices)
{
	if (best_prices[n - 1] > 0) {
		push(stack, cutlist[n - 1]);
		return best_prices[n-1];

	}
	int maxcut = 0;
	int max = prices[n - 1];
	for (int cut = 1; cut < n/2 + 1; cut++){
		int calculated_price = cbr_memo(n - cut, prices) + cbr_memo(cut, prices);
		if (calculated_price > max){
			max = calculated_price;
			maxcut = cut;
		}
	}
	best_prices[n-1] = max;
	cutlist[n - 1] = maxcut;
	push(stack, maxcut);
	return max;
}



int main()
{
	stack = make_stack();
	cbr_memo(10, prices);
	print_stack(stack);
	return 0;
}

