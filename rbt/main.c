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
	color_fix_after_insert(T, node_to_insert);
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
	T->number_of_nodes++;
}

char color_char(bool color)
{
	return color ? 'b' : 'r';
}

void print_node(rbt_t* T, node_t* n)
{
	printf("\n");
	char* printstring = "name\t%s\nkey\t%d\ncolor\t%c\n";
	printf("\n");
	printf(printstring, n->name, n->key, color_char(n->color));

	if (n->c[LEFT] == T->nil) printf("\nLEFT NIL\n");
	else {
		printf("\nLEFT\n");
		node_t* c = n->c[LEFT];
		printf(printstring, c->name, c->key, color_char(c->color));
	}

	if (n->c[RIGHT] == T->nil) printf("\nRIGHT NIL\n");
	else {
		printf("\nRIGHT\n");
		node_t* c = n->c[RIGHT];
		printf(printstring, c->name, c->key, color_char(c->color));
	}
	return;
}

void rotate(rbt_t* T, node_t* p, bool dir)
{
	if (p == T->nil) {
		printf("Rotate error.\n");
		exit(1);
	}
	node_t* ch = p->c[!dir];
	if (ch == T->nil) {
		printf("Rotate error.\n");
		exit(1);
	}
	//1 - set c's p to grandfather
	ch->p = p->p;
	if (ch == T->nil) {
		printf("FUCK ALL OF YOU\n");
		//ch = 1/0;
		exit(1);
	}
	//2 - set grandfather's son (replace p) to ch(ild)
	if (p->p != T->nil){
		p->p->c[p->p->c[1] == p] = ch;
	}
	//3 - set p's p to ch
	p->p = ch;
	//4 - replace ch in p's c list with ch's swing c
	p->c[!dir] = ch->c[dir];
	//5 - set swing c's p to p, but only if it exists
	if (ch->c[dir] != T->nil){
		ch->c[dir]->p = p;
	}
	//6 - replace swing c in ch's c list with p
	ch->c[dir] = p;
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
			rotate(T, z, LEFT);
			break;
		case 'R':
			rotate(T, z, RIGHT);
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

bool type_of_child(node_t* child)
{
	return (child->p->c[1] == child);
}

void set_red(node_t* node)
{
	node->color = RED;
}

void set_black(node_t* node)
{
	node->color = BLACK;
}

void color_fix_after_insert(rbt_t* T, node_t* inserted_node)
{
	node_t* z = inserted_node;
	while (z->p->color == RED) {
		/* z - current node
		 * p - parent
		 * pp - grand parent
		 * u - uncle
		 */
		bool pdir = type_of_child(z->p);
		node_t* p = (z->p);
		node_t* pp = (z->p->p);
		node_t* u = (z->p->p->c[!pdir]);
		//case 1:
		//uncle is red
case_1:
		if (u->color == RED) {
			set_black(u);
			set_black(p);
			set_red(pp);
			z = pp;
			continue;
		}
case_2:
		//case 2:
		//uncle is black and z is rotation adoptee
		if (type_of_child(z) != pdir) {
			rotate(T, p, pdir);
			rotate(T, pp, !pdir);
			set_black(z);
			set_red(pp);
			break;
		}
case_3:
		//case 3:
		//uncle is black and z is not rotation adoptee
		rotate(T, pp, !pdir);
		set_black(p);
		set_red(pp);
		break;
	}
	if (T->root->p != T->nil){
		T->root = T->root->p;
	}
	set_black(T->root);
}

//returns bnode height of subtree
int validate(rbt_t* T, node_t* subtree_root, int* temp_node_number)
{
	if (subtree_root == T->nil) return 1;
	(*temp_node_number)++;
	int bnodes1 = validate(T, subtree_root->c[0], temp_node_number);
	int bnodes2 = validate(T, subtree_root->c[1], temp_node_number);
	//if (bnodes1 < 0) return;
	if (bnodes2 != bnodes1){
		//can return neg value and propogate error
		//if i dont want full exit
		printf("%d - Black property fail\n", subtree_root->key);
		//exit(1);
	}

	if (subtree_root->color == RED){
		if (subtree_root->p->color == RED){
			printf("%d - Red property fail\n", subtree_root->key);
			//exit(1);
		}
		return bnodes1;
	} else {
		return bnodes1 + 1;
	}
}

int validate_tree(rbt_t* rbt)
{
	int temp_node_number = 0;
	int bheight = validate(rbt, rbt->root, &temp_node_number);
	if (temp_node_number != rbt->number_of_nodes){
		printf("calculated node number not accurate, node lossage\n");
	}
	printf("\n\n%d - nodes\n%d - calculated node number\n", rbt->number_of_nodes, temp_node_number);
	printf("\ncalculated blackheight - %d\n", bheight);
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
		navigate_tree_prompt(tree, tree->root);
	}
	} else {
	for (int i = 0; i < 400; i++){
		int key = rand() % 2000 - 1000;
		printf("Adding %d\n", key);
		char* name = " ith node";
		//name[0] = (i % 10) + '0';
		//name[1] = (i) + '0';
		insert_into_tree(tree, make_node(tree, RED, key, name));
		//navigate_tree_prompt(tree, tree->root);

	}
	}
	validate_tree(tree);
	navigate_tree_prompt(tree, tree->root);
}
