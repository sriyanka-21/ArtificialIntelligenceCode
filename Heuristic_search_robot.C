#include <stdio.h>
#include <graphics.h>
#include <limits.h>
#include <dos.h>
#include <math.h>

FILE *fptr;

int maxx, maxy, midx, midy;

int node_cnt = 0;
typedef struct Node{
     int idx;
     int x, y;
}Node;

Node* newNode(int, int);
int h(Node*);
int he(int, char);
void keep_searching(Node*, Node*, Node*, int uniqueNode[50][50], int*, char);
int areEqual(Node*, Node*);
void pointout(int, int, int);
void setaxes();
void makepath(Node*, Node*, int);

void main(){
	int gd = DETECT, gm;
	char path[] = "C:\\TURBOC3\\BGI";
	Node* start = newNode(0, 0);
	Node* goal = newNode(15, 20);
	Node* prev = newNode(-1, -1);
	int gen_cost = 0;
	char dir = 'u';
	int uniqueNode[50][50];
	uniqueNode[25][25] = 0;
	fptr = fopen("heuristic_path.txt", "w");
	initgraph(&gd, &gm, path);
	setaxes();
	pointout(0, 0, 9);
	pointout(15, 20, 3);
	memset(uniqueNode, 1, 2500*sizeof(int));
	keep_searching(goal, prev, start, uniqueNode, &gen_cost, dir);
	getch();
	fclose(fptr);
	closegraph();
}

Node* newNode(int x, int y){
	Node* p = (Node*)malloc(sizeof(Node));
	p -> idx = ++node_cnt;
	p -> x = x;
	p -> y = y;
	return p;
}

int h(Node* node) {
    // Manhattan distance
    return abs(node->x - 15) + abs(node->y - 20);
}

int he(int c, char dir){
	int d;
	int turn[4][4] = {{0, 1, 2, 1}, {1, 0, 1, 2}, {2, 1, 0, 1}, {1, 2, 1, 0}};
	if(dir == 'u') d = 0;
	else if(dir == 'r') d = 1;
	else if(dir == 'd') d = 2;
	else if(dir == 'l') d = 3;
	else exit(1);
	return turn[d][c];
}

void keep_searching(Node* goal, Node* prev, Node* curr, int uniqueNode[50][50], int* gen_cost, char dir){
	int x = curr -> x, y = curr -> y;
	int minCost = INT_MAX, cost;
	char ndir;
	Node* next = newNode(-1, -1), *temp;
	if(areEqual(curr, goal))
		return;
	if(curr -> x == curr -> y && curr -> y == -1)
		return;
	if(uniqueNode[x/3 + 25][(y+4)/4 + 25]){
		uniqueNode[x/3 + 25][(y+4)/4 + 25] = 0;
		cost = (*gen_cost) + 1;
		temp -> idx = ++node_cnt;
		temp -> x = x;
		temp -> y = y + 4;
		cost += h(temp);
		cost += he(0, dir);
		if(cost < minCost){
			minCost = cost;
			next -> x = temp -> x;
			next -> y = temp -> y;
			ndir = 'u';
		}
	}
	if(uniqueNode[(x+3)/3 + 25][y/4 + 25]){
		uniqueNode[(x+3)/3 + 25][y/4 + 25] = 0;
		cost = (*gen_cost) + 1;
		temp -> idx = ++node_cnt;
		temp -> x = x + 3;
		temp -> y = y;
		cost += h(temp);
		cost += he(1, dir);
		if(cost < minCost){
			minCost = cost;
			next -> x = temp -> x;
			next -> y = temp -> y;
			ndir = 'r';
		}
	}
	if(uniqueNode[x/3 + 25][(y-4)/4 + 25]){
		uniqueNode[x/3 + 25][(y-4)/4 + 25] = 0;
		cost = (*gen_cost) + 1;
		temp -> idx = ++node_cnt;
		temp -> x = x;
		temp -> y = y - 4;
		cost += h(temp);
		cost += he(2, dir);
		if(cost < minCost){
			minCost = cost;
			next -> x = temp -> x;
			next -> y = temp -> y;
			ndir = 'd';
		}
	}
	if(uniqueNode[(x-3)/3 + 25][y/4 + 25]){
		uniqueNode[(x-3)/3 + 25][y/4 + 25] = 0;
		cost = (*gen_cost) + 1;
		temp -> idx = ++node_cnt;
		temp -> x = x - 3;
		temp -> y = y;
		cost += h(temp);
		cost += he(0, dir);
		if(cost < minCost){
			minCost = cost;
			next -> x = temp -> x;
			next -> y = temp -> y;
			ndir = 'l';
		}
	}
	fprintf(fptr, "{%d, %d} --%d--> {%d, %d} ....along direction %c\n", curr -> x, curr -> y, minCost, next -> x, next -> y, ndir);
	dir = getchar();
	*gen_cost += 1;
	makepath(curr, next, 13);
	pointout(next -> x, next -> y, 10);
	keep_searching(goal, curr, next, uniqueNode, gen_cost, ndir);
}

int areEqual(Node* n1, Node* n2){
	return (n1 -> x == n2 -> x && n1 -> y == n2 -> y);
}

void pointout(int x, int y, int col){
	setcolor(col);
	x = midx + x * 3;
	y = midy - (y * 4);
	putpixel(x, y, col);
	circle(x, y, 2);
}

void setaxes(){
	cleardevice();
	maxx = getmaxx();
	maxy = getmaxy();
	midx = maxx/2;
	midy = maxy/2;
	line(midx, 0, midx, maxy);
	line(0, midy, maxx, midy);
}

void makepath(Node* curr, Node* next, int col){
	setcolor(col);
	line(midx + (curr -> x) * 3, midy - ((curr -> y) * 4), midx + (next -> x) * 3, midy - ((next -> y) * 4));
}