/* Compile using command: 
		gcc makeinput.c -o makeinput
   Run using:
   		./makeinput
   Will create a file named INPUT.TXT made using specs in group project for testing code.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define STDOUT 1

int main() {
  int i, hexagons = 233;
  int output_fd;
  char output[] = "INPUT.TXT";

  //Open and create the file "INPUT.TXT"
  if ((output_fd=open(output, O_WRONLY|O_CREAT|O_EXCL,0666)) == -1) {
    perror("open failed");
    exit(1);
  }

  //Print each line of hexagon and redirect to file
  //Randomly select cost from 1-999;
  dup2(output_fd,STDOUT);
  for (i=1; i<=hexagons;i++) {
    int d = rand() % 1000;
    printf("%d %d",i,d);
    if(i!=hexagons)printf("\n");
  }
}
