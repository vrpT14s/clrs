//copy some code for priority queue if needed
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#define NUMS 15

typedef struct heap_t{
	int* array;
	unsigned int length;
} heap_t;

//assumes left and right are already max heaps
void fix_max_heap_at(heap_t* heap, int index){
	int value = heap->array[index];	
	int left = heapleft(index);
	int right = heapright(index);

	bool needs_exchange = false;
	int largest = index;
	if ((left < heap->length) && (value < heap->array[left])){
		largest = left;
		needs_exchange = true;
	}
	if ((right < heap->length) && (heap->array[largest] < heap->array[right])){
		largest = right;	
		needs_exchange = true;
	}
	if (needs_exchange){
		//child larger than parent
		//exchange then make child a max heap
		swap(heap->array + index, heap->array + largest);
		fix_max_heap_at(heap, largest);
	}
}

void make_max_heap(heap_t* heap){
	int i = (heap->length)/ 2;
	while (i --> 0){
		fix_max_heap_at(heap, i);
	}
	/*
	int left = heapleft(index);
	int right = heapright(index);

	if (left < heap->length)  make_max_heap(heap, left);
	if (right < heap->length) make_max_heap(heap, right);
	fix_max_heap_at(heap, index);
	*/
}

void sort_array(int* array, int length){
	heap_t heapr = {array, length};
	heap_t* heap = &heapr;
	make_max_heap(heap);

	for (int i = heap->length - 1; i >= 1; i--){
		swap(heap->array, heap->array + i);
		heap->length -= 1;
		fix_max_heap_at(heap, 0);
	}
}

int main(){
	int myarray[NUMS] = {
	-99999,3,4,-23,6,144,8,234,0,0,2,3,5,2,5,7
	//1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
	//1,2,3,4
	//4,3,2,1
	};
	//swap(myarray + 4, myarray + 5);
	printf("Original array:\n");
	print_array(myarray, NUMS);

	heap_t myheap = {myarray, NUMS};
	print_heap(&myheap);
	make_max_heap(&myheap);
	printf("\nnice heap:\n");
	print_heap(&myheap);
	sort_array(myarray, NUMS);
	printf("Sorted array:\n");
	print_array(myarray, NUMS);
	if (!(check_increasing(myarray, NUMS))){
		exit(1);
	}
	return 0;
}

unsigned int round_up_to_power_of_2(unsigned int x){
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

void pha(heap_t* heap, int index, int length, int accum){
	for (int i = 0; i < accum; i++) printf("|");
	if (index >= heap->length){
		printf("UNDEF\n");
	} else {
		printf("\\%d\n", heap->array[index]);
	}
	if (length <= 2) return;
	pha(heap, heapleft(index), length/2, accum + 1);
	pha(heap, heapright(index), length/2, accum + 1);
}


int heapleft(int index){
	return 2*(index + 1) - 1;
}
int heapright(int index){
	return 2*(index + 1);
}

void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void print_array(int* array, int length){
	printf("[%d", array[0]);
	for (int i = 1; i < length; i++){
		printf(", %d", array[i]);
	}
	printf("]\n");
}

int check_increasing(int* array, int length){
	for (int i = 0; i < length - 1; i++){
		if (array[i] > array[i+1]){
			printf("array not increasing\n");
			return false;
		}
	}
	printf("array increasing\n");
	return true;
}

void print_heap(heap_t* heap){
	unsigned int len = round_up_to_power_of_2(heap->length + 1) - 1;
	pha(heap, 0, len, 0);
}

