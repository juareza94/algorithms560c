/*********************************************\
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
 * [upleft,up,upright,downleft,d,dr]
\*********************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#define STDIN 0
#define MAXV 233
#define TRUE 1
#define FALSE 0
#define MAXINT 9999

struct hexdef{
  int y;                          /* hexagon number */
  int weight;                     /* hexagon weight */
  struct hexdef* next[6];         /* array of neighbors */
};
struct hexgraph{
  struct hexdef* edges[MAXV];         /* adj list for each vertex */
  int nvertices;                        /* number of vertices in graph */
  int nedges;                           /* number of edges in graph*/
};

void dijkstra(struct hexgraph* g, int start) {
  int i;
  int v;
  int w;
  struct hexdef* p;
  int weight;
  int dist;
  int intree[MAXV+1];
  int distance[MAXV+1];
  int parent[MAXV+1];

  for (i=1; i<=g->nvertices; i++) {
    intree[i] = FALSE;
    distance[i] = MAXINT;
    parent[i] = -1;
  }
  distance[start] = 0;
  v = start;

  while(!intree[v]) {
    intree[v] = TRUE;
    p = g->edges[v];
    while (p != NULL) {
      w = p -> y;
      weight = p -> weight;
      if (distance[w] > (distance[v]+weight)) {
        distance[w] = distance[v] + weight;
        parent[w] = v;
      }
      v++;
      p = g->edges[v]; //originally p = p->next;
    }
    v = 1;
    dist = MAXINT;  //As of now, assumed that MAXINT arbitrarily large for compares
    for (i=1; i <= g->nvertices; i++)
      if (!intree[i] && (dist > distance[i])) {
        dist = distance[i];
        v = i;
      }
  }
}

int main() {
  //Open and direct the file to STDIN
  char* input = "INPUT.TXT";
  char* output = "OUTPUT.TXT";
  int input_fd = open(input, O_RDONLY);
  dup2(input_fd,STDIN);

  //Read the file and create nodes
  int i,x;
  char c;
  char number[3];

  //Define the graph
  struct hexgraph graph;
  graph.nvertices = 233;
  graph.nedges = 233;

  //Create Each Node
  for (i=1;i<=233;i++) {

    //Grab and set node number
    x = 0;
    struct hexdef hexagon;// = malloc(sizeof(hexagon));
    while(isdigit(c = getchar())) {
      x = (x*10) + (c - '0');
    } printf("Node is: %d     ", x);
    hexagon.y = x;

    //Grab and set weight number
    x = 0;
    while(isdigit(c = getchar())) {
      x = (x*10) + (c - '0');
    } printf("Cost is: %d \n", x);
    hexagon.weight = x;

    //graph.vertices[i] = &hexagon;    //IN PROGRESS
    //printf("Address set is %p\n", &hexagon);
  }
  close(input_fd);

  //Assign Each Node Its Children
  //IN PROGRESS

  //Find the Shortest Path
  //dijkstra()
}
