#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct heap_t{
	int** array;
	unsigned int length;
} heap_t;

int heapleft(int index)
{
	return 2*(index + 1) - 1;
}

int heapright(int index)
{
	return 2*(index + 1);
}

//assumes left and right are already min heaps
void fix_min_heap_at(heap_t* heap, int index)
{
	int value = *heap->array[index];
	int left = heapleft(index);
	int right = heapright(index);

	bool needs_exchange = false;
	int least = index;
	if ((left < heap->length) && (*heap->array[left] < value)){
		least = left;
		needs_exchange = true;
	}
	if ((right < heap->length) && (*heap->array[right] < *heap->array[least])){
		least = right;
		needs_exchange = true;
	}
	if (needs_exchange){
		//child less than parent
		//exchange then make child a max heap
		swap(heap->array + index, heap->array + least);
		fix_min_heap_at(heap, least);
	}
}

heap_t* make_min_heap(int** array, int length)
{
	heap_t* heap = malloc(sizeof(heap_t));
	heap->array = array;
	heap->length = length;
	int i = (heap->length)/ 2;
	while (i --> 0){
		fix_min_heap_at(heap, i);
	}
	return heap;
	/*
	int left = heapleft(index);
	int right = heapright(index);

	if (left < heap->length)  make_max_heap(heap, left);
	if (right < heap->length) make_max_heap(heap, right);
	fix_max_heap_at(heap, index);
	*/
}

typedef struct node_t {
	int freq;
	int value;
} node_t;

void swap(void** a, void** b)
{
	void* temp = *a;
	*a = *b;
	*b = temp;
}

unsigned int round_up_to_power_of_2(unsigned int x)
{
	int i = 8*sizeof(int);
	while (i --> 0){
		if (x & (1 << i)){
			int j = i;
			while (j --> 0){
				if (x & (1 << j)){
					return 1 << (i + 1);
				}
			}
			return 1 << i;
		}
	}
	return 0;
}

void pha(heap_t* heap, int index, int length, int accum)
{
	for (int i = 0; i < accum; i++) printf("|");
	if (index >= heap->length){
		printf("UNDEF\n");
	} else {
		printf("\\%d\n", *heap->array[index]);
	}
	if (length <= 2) return;
	pha(heap, heapleft(index), length/2, accum + 1);
	pha(heap, heapright(index), length/2, accum + 1);
}

void print_heap(heap_t* heap)
{
	unsigned int len = round_up_to_power_of_2(heap->length + 1) - 1;
	pha(heap, 0, len, 0);
}

void check_heap(int index, heap_t* heap)
{
	index
}

int main()
{
	srand(time(NULL));
	int N = 30;
	node_t** array = malloc(sizeof(node_t*)*N);
	for (int i = 0; i < N; i++){
		array[i] = malloc(sizeof(node_t));
		array[i]->freq = rand() % 1000;
		array[i]->value = array[i]->freq / 2;
	}
	for (int i = 0; i < N; i++){
		printf("%d : %d - %d\n", i, array[i]->freq, array[i]->value);
	}
	heap_t* heap = make_min_heap(array, N);
	for (int i = 0; i < N; i++){
		printf("%d : %d - %d\n", i, array[i]->freq, array[i]->value);
	}
	print_heap(heap);
	return 0;
}

