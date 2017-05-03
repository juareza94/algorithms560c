/******************************************************************\
 * Graph Maker
 * Create and make a graph based on the
 * INPUT.TXT file in the same directory.
 *
 * Running
 * Ensure an INPUT.TXT file with proper specs
 * exists in the same folder as this source.
 * Compile using: gcc hexalgorithm.c
 *
 * Array of neighbors is defined as such
 * [0-upleft, 1-up, 2-upright, 3-downleft, 5-down, 5-downright]
 *
\******************************************************************/

/******************************************************************\
 * Current Tasks:
 * Dijkstra's Algorithm currently passes through the graph but is
 * Not taking the mose efficient path. See note on line 173 under
 * dijkstra function > Algorithm
\*******************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#define START 226
#define END 8
#define STDIN 0
#define MAXV 233
#define TRUE 1
#define FALSE 0
#define MAXINT 99999999

//Define Hexagon Cell
struct hexdef{
  int y;                          /* hexagon number */
  int weight;                     /* hexagon weight */
  struct hexdef* next[6];         /* array of neighbors */
};
//Define Graph of Hexagon Cells
struct hexgraph{
  struct hexdef* edges[MAXV+1];         /* adj list for each vertex */
  int nvertices;                        /* number of vertices in graph */
  int nedges;                           /* number of edges in graph*/
};

void dijkstra(struct hexgraph* g, int start);
void testPrinter(int i);

struct hexdef hexagon[MAXV];
struct hexgraph graph;

int main(int argc, char* argv[]) {
  //Open and direct the file to STDIN
  char* input = "INPUT.TXT";
  int input_fd = open(input, O_RDONLY);
  dup2(input_fd,STDIN);

  //Read the file and create nodes
  int i,x;
  char c;
  char number[3];

  //Set properties of the graph
  graph.nvertices = MAXV;
  graph.nedges = 0;

  //Make all hexagons in the graph
  for (i=1;i<=MAXV;i++) {
    //Create the new hexagon and store to graph list of vertices
    graph.edges[i] = &hexagon[i];

    //Grab and set node number
    x = 0;
    while(isdigit(c = getchar())) {
      x = (x * 10) + (c - '0');
    } hexagon[i].y = x;
    //printf("Node is: %d     ", hexagon[i].y);

    //Grab and set weight number
    x = 0;
    while(isdigit(c = getchar())) {
      x = (x*10) + (c - '0');
    } hexagon[i].weight = x;
    graph.nedges += 1;
    //printf("Cost is: %d \n", hexagon[i].weight);
  } close(input_fd);

  //Set children for Each Hexagon
  for (i=1; i<=MAXV; i++){
    //top left
    if (i < 9 || (i-1)%15==0) hexagon[i].next[0] = NULL;
    else {
      hexagon[i].next[0] = &hexagon[i-8];
      graph.nedges += 1;
    }
    //top
    if (i<16) hexagon[i].next[1] = NULL;
    else {
      hexagon[i].next[1] = &hexagon[i-15];
      graph.nedges += 1;
    }
    //top right
    if (i < 9 || (i-8)%15==0) hexagon[i].next[2] = NULL;
    else {
      hexagon[i].next[2] = &hexagon[i-7];
      graph.nedges += 1;
    }
    //bottom left
    if (i > 226 || (i-1)%15==0) hexagon[i].next[3] = NULL;
    else {
      hexagon[i].next[3] = &hexagon[i+7];
      graph.nedges += 1;
    }
    //bottom
    if (i>218) hexagon[i].next[4] = NULL;
    else {
      hexagon[i].next[4] = &hexagon[i+15];
      graph.nedges += 1;
    }
    //bottom right
    if (i>225 || (i-8)%15==0) hexagon[i].next[5] = NULL;
    else {
      hexagon[i].next[5] = &hexagon[i+8];
      graph.nedges += 1;
    }
  }

  //Check for test request
  if (argv[1]!=NULL) {
    int test = 0,i;
    for (i = 0;argv[1][i]!=NULL;i++){
      char c = argv[1][i];
      test = test*10 + c-'0';
    }
    testPrinter(test);
  }

  //Find the Shortest Path
  dijkstra(&graph,START);
}

void dijkstra(struct hexgraph* g, int start) {
  int i;      //the current hexagon
  int j;      //children of hexagon[i]
	int next;
	int newcost;
	int parent[MAXV+1];
	int cost[MAXV+1];
	int visited[MAXV+1];

  int queue[MAXV+1];
  int wasqueued[MAXV+1];
  int front = 0, rear = 0;

	//Prepare for Dijkstra's Algorithm
	for (i=1;i<=MAXV;i++){
		parent[i] = 0;
		cost[i] = MAXINT;
		visited[i] = FALSE;
    wasqueued[i] = 0;
	} cost[start] = g->edges[start]->weight;  //add in cost of first node

  //Add Start to the Queue
  queue[front] = start;
  rear += 1;
  i = queue[front];

  //Algorithm
  /* See PDF Posted by Julia in Messenger to find where code does not traverse
   * the correct path.
   */
  while(front != rear) {  //Loop till queue is empty
    //printf("At hexagon %d\n", i);
    front+=1;
    for (j = 0 ; j < 6 ; j++)               //Check all children
      if (g->edges[i]->next[j]!=NULL) {  //Check existance
        next = g->edges[i]->next[j]->y;  //Pull node number
        //printf("At child %d\n", next);
        if (visited[next]==FALSE) {                 //Check if visited - TODO
          if (!wasqueued[next]) {
            queue[rear] = next;
            rear += 1;
            wasqueued[next] = 1;
          }
          //printf("Added %d to the queue\n", next);
          newcost = g->edges[next]->weight + cost[i];
          if (cost[next] > newcost) {
            cost[next] = newcost;
            parent[next] = i;
          }
        }
      }
    visited[i] = TRUE;
    i = queue[front];
  }
  //for (i=1;i<=MAXV;i++) printf("Vertex '%d' has cost '%d' to reach.\n", i, cost[i]);
  //for (i=1;i<=MAXV;i++) printf("%d, ", queue[i]);
  //for (i=1;i<=MAXV;i++) if (visited[i] == 1) printf("%d has been visited.\n", i);
  printf("Cost of end is %d\n", cost[8]);
  for(i=END;i!=start;) {
    printf("Parent of %d is %d\n", i, parent[i]);
    i = parent[i];
  }
}


void testPrinter(int i){
  printf("Hexagon %d in grid looks like:\n\n", i);
  //TOP
  if (hexagon[i].next[1]==NULL)  printf("     NULL\n");
  else printf("     %d\n", graph.edges[i]->next[1]->y);
  if (hexagon[i].next[1]==NULL) printf("      __     \n");
  else printf("      __     \n");
  //TOP LEFT
  if(hexagon[i].next[0] == NULL) printf("NULL/    ");
  else printf(" %d/    ", graph.edges[i]->next[0]->y);
  //TOP RIGHT
  if (hexagon[i].next[2] == NULL) printf("\\NULL\n");
  else printf("\\%d \n", graph.edges[i]->next[2]->y);
  //BOTTOM LEFT
  if (hexagon[i].next[3] == NULL) printf("NULL\\ __ ");
  else printf(" %d\\ __ ",graph.edges[i]->next[3]->y);
  //BOTTOM RIGHT
  if (hexagon[i].next[5] == NULL) printf("/NULL \n");
  else printf("/%d \n", graph.edges[i]->next[5]->y);
  //BOTTOM
  if(hexagon[i].next[4] == NULL) printf("     NULL \n");
  else printf("     %d \n", graph.edges[i]->next[4]->y);
}
