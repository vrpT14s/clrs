#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define LEFT 0
#define RIGHT 1

#define RED 0
#define BLACK 1

typedef struct node_t
{
	struct node_t* p;
	struct node_t* c[2];

	bool color;

	int key;
	char* name; //just an example of satellite data, some use in debugging
} node_t;

typedef struct rbt_t
{
	node_t* root;
	node_t* nil;

	int bheight;
	int height;
	int number_of_nodes;

	char* name; //also example/debugging
} rbt_t;

node_t* make_node(rbt_t* T, bool color, int key, char* name)
{
	node_t* n = malloc(sizeof(node_t));
	n->color = color;
	n->key = key;
	n->name = name;

	n->p = T->nil;
	n->c[0] = T->nil;
	n->c[1] = T->nil;
	return n;
}

rbt_t* make_tree(node_t* nil, char* name)
{
	rbt_t* rbt = malloc(sizeof(rbt_t));
	rbt->nil = nil;
	rbt->name = name;

	rbt->root = nil;
	rbt->bheight = 0;
	rbt->height = 0;
	rbt->number_of_nodes = 0;

	return rbt;
}

void insert_into_subtree(rbt_t* T, node_t* subtree_root, node_t* node_to_insert)
{
	node_t* z = subtree_root;
	while(true){
		bool dir = (node_to_insert->key > z->key);
		if (z->c[dir] == T->nil){
			z->c[dir] = node_to_insert;
			node_to_insert->p = z;

			node_to_insert->color = RED;
			node_to_insert->c[0] = T->nil;
			node_to_insert->c[1] = T->nil;
			break;
		} 
		//else
		z = z->c[dir];
	}
	//TODO: fix_color()
}

void insert_into_tree(rbt_t* T, node_t* node_to_insert)
{
	if (T->root == T->nil){
		T->root = node_to_insert;
		node_to_insert->p = T->nil;

		node_to_insert->color = BLACK;
		node_to_insert->c[0] = T->nil;
		node_to_insert->c[1] = T->nil;
		node_to_insert->name = "ROOT";
	} else {
		insert_into_subtree(T, T->root, node_to_insert);
	}
}

void print_node(rbt_t* T, node_t* n)
{
	char color_char = n->color ? 'B' : 'R';
	printf(" \n\
addr	0x%x	\n\
name	%s	\n\
key	%d	\n\
color	%c	\n\
nil?	%d	\n\
	\n\
parent  0x%x	\n\
name	%s	\n\
nil?	%d	\n\
	\n\
cleft	0x%x	\n\
key	%d	\n\
nil?	%d	\n\
cright	0x%x	\n\
key	%d	\n\
nil?	%d	\n\
", n, n->name, n->key, color_char, (n == T->nil), n->p, n->p->name, (n->p == T->nil), n->c[LEFT], n->c[LEFT]->key, (n->c[LEFT] == T->nil), n->c[RIGHT], n->c[RIGHT]->key, (n->c[RIGHT] == T->nil));
	return;
}

void rotate(rbt_t* T, node_t** p, bool dir)
{
	node_t** ch = &((*p)->c[!dir]);
	if ((*ch) == T->nil) {
		printf("Rotate error.\n");
		exit(1);
	}
	//1 - set c's p to grandfather
	(*ch)->p = (*p)->p;
	if (*ch == T->nil) {
		ch = 1/0;
		printf("FUCK ALL OF YOU\n");
	}
	//2 - set grandfather's son (replace (*p)) to ch(ild)
	(*p)->p->c[(*p)->p->c[1] == (*p)] = (*ch);
	//3 - set (*p)'s p to ch
	(*p)->p = (*ch);
	//4 - replace ch in (*p)'s c list with ch's swing c
	(*p)->c[!dir] = (*ch)->c[dir];
	//5 - set swing c's p to (*p), but only if it exists
	if ((*ch)->c[dir] != T->nil){
		(*ch)->c[dir]->p = (*p);
	}
	//6 - replace swing c in ch's c list with (*p)
	(*ch)->c[dir] = (*p);
	return;
}

void navigate_tree_prompt(rbt_t* T, node_t* starting_node)
{
	node_t* z = starting_node;
	while(z != T->nil){
		print_node(T, z);
		printf("\n> ");
		//int character = getc(stdout);
		char character;
		scanf(" %c", &character);
		switch(character){
		case 'p':
			z = z->p;
			break;
		case 'l':
			if (z->c[LEFT] == T->nil){
				printf("NIL, not possible\n");
				break;
			}
			z = z->c[LEFT];
			break;
		case 'r':
			if (z->c[RIGHT] == T->nil){
				printf("NIL, not possible\n");
				break;
			}
			z = z->c[RIGHT];
			break;
		case 'L':
			rotate(T, &z, LEFT);
			break;
		case 'R':
			rotate(T, &z, RIGHT);
			break;
		case 't':
			z = starting_node;
			break;
		case 'e':
			return;
		default:
			printf("INVALID OPTION, use only p, l, r\n");
			break;
		}
	}
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	
	node_t* nil = malloc(sizeof(node_t));
	nil->color = BLACK;
	nil->name = "NIL";
	nil->key = 86064;
	rbt_t* tree = make_tree(nil, "My Red Black Tree");
	if (argc > 1){
	for (int i = 1; i < argc; i++){
		int key = atoi(argv[i]);
		char* name = "ok";
		insert_into_tree(tree, make_node(tree, RED, key, name));
	}
	} else {
	for (int i = 0; i < 4; i++){
		int key = rand() % 2000 - 1000;
		char* name = " ith node";
		//name[0] = (i % 10) + '0';
		//name[1] = (i) + '0';
		insert_into_tree(tree, make_node(tree, RED, key, name));
	}
	}
	navigate_tree_prompt(tree, tree->root);
}
