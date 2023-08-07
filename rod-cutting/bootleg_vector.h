#pragma once
#include <stdlib.h>


typedef struct link_t {
	struct link_t* next;
	int key;
} link_t;

typedef link_t* stack_t;

stack_t* make_stack()
{
	stack_t* s = malloc(sizeof(stack_t));
	*s = NULL;
	return s;
}
void push(stack_t* stack, int key)
{
	link_t* l = malloc(sizeof(link_t));
	l->key = key;
	l->next = *stack;
	*stack = l;
}

void print_stack(stack_t* stack)
{
	printf("{%d", (*stack)->key);
	for (link_t* l = (*stack)->next; l != NULL; l = l->next){
		printf(", %d", l->key);
	}
	printf("}\n");
}

void delete_stack(stack_t* stack){
	if (stack == NULL) return;
	link_t* l = *stack;
	link_t* m = l->next;
	while(1){
		free(l);
		l = m;
		m = l->next;
		if (m == NULL){
			free(l);
			stack = NULL;
			return;
		}
	}
}

