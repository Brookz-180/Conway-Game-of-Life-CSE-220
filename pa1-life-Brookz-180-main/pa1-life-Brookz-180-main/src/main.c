#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> //allows use of atoi()
#include <unistd.h> //this allows use of usleep()

#include "life.h"

/* Be sure to read life.h and the other given source files to understand
 * what they provide and how they fit together.  You don't have to
 * understand all of the code, but you should understand how to call
 * parse_life() and clearterm().
 */

/* This is where your program will start.  You should make sure that it
 * is capable of accepting either one or two arguments; the first
 * argument (which is required) is a starting state in one of the Life
 * formats supported by parse_life(), and the second (which is optional)
 * is a number of generations to run before printing output and stopping.
 *
 * The autograder will always call your program with two arguments.  The
 * one-argument format (as described in the handout) is for your own
 * benefit!
 */
int alivenbrs(char topleft,char topmid, char topright,char midleft, char midright, char bottomleft, char bottommid, char bottomright) {
    int nbrs = 0; //counter for number of alive neighbors
    if (topleft==LIVE) {   //check the 8 neighbors and see if they are alive
        nbrs++;
    }
    if (topmid==LIVE) {
        nbrs++;
    }
    if (topright==LIVE) {
        nbrs++;
    }
    if (midleft==LIVE) {
        nbrs++;
    }
    if (midright==LIVE) {
        nbrs++;
    }
    if (bottomleft==LIVE) {
        nbrs++;
    }
    if (bottommid==LIVE) {
        nbrs++;
    }
    if (bottomright==LIVE) {
        nbrs++;
    }
    return nbrs;
}

int swaparray(int current) { //swaps between 0 and 1 to keep track of current gen
    if (current==0) { 
        current = 1;
    } else {
        current = 0;
    }
    return current;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3) { //error handling for command line arguments
        printf("Error: Incorrect number of arguments.");
        return -1;
    }
    if (argc == 3 && atoi(argv[2])<0) {
        printf("Error: Invalid generation number.");
        return -1;
    }

    char **parsed = parse_life(argv[1]);

    
    char grid[2][26][82]; //remake grid with outer edge of dead cells
    for (int i=1; i<25; i++) {
        for (int j=1; j<81; j++) {
            grid[0][i][j] = parsed[i-1][j-1]; //copy inner grid from parsed
        }
    }
    for (int i=0;i<26;i++) {
        for (int j=0; j<82; j++) {
            if (i==0 || i==25 || j==0 || j==81) {
                grid[0][i][j] = DEAD; //outer ring of cells is dead
                grid[1][i][j] = DEAD;
            }
        }
    }

    int current = 0; //initialize starting grid is matrix[0][26][82]
    int next = 1; //next generation starts with matrix[1]
    
    for (int gen=0; ; gen++) {
        for (int i=1;i<25;i++) {
            for (int j=1;j<81;j++) { //loop through all indices of matrix
                int nbrs = alivenbrs(grid[current][i-1][j-1],grid[current][i-1][j],grid[current][i-1][j+1],grid[current][i][j-1],grid[current][i][j+1],grid[current][i+1][j-1],grid[current][i+1][j],grid[current][i+1][j+1]);
                if (grid[current][i][j]==LIVE) { //if the cell is alive
                    if (nbrs == 2 || nbrs == 3) { //and has 2 or 3 neighbors
                        grid[next][i][j] = LIVE;      //stays alive in next gen
                    } else if (nbrs<2 || nbrs>3) { //else has <2 or >3 neighbors
                        grid[next][i][j] = DEAD;      //dies for next gen
                    }
                } else if (grid[current][i][j] == DEAD) { //if cell is dead
                    if (nbrs == 3) {          //if it has 3 neighbors
                        grid[next][i][j] = LIVE; //it becomes alive
                    } else {
                        grid[next][i][j] = DEAD; //else, it stays dead
                    }
                }
            }
        }

        //********************************
        //COMMENT THIS PRINT LOOP OUT FOR UPLOADING TO AUTOLAB, ONLY NEED FINAL GEN
        /*
        for (int i=1;i<25;i++) {
            for (int j=1;j<81;j++) {
                putchar(grid[current][i][j]); //print grid to terminal
            }
            putchar('\n');
        }
        */
        
        
        //********************************
        //PRINT FINAL GEN, ONLY USE THIS FOR SUBMITTING TO AUTOLAB
        //FOR LOCAL ANIMATION, COMMENT THIS OUT AND UNCOMMENT ABOVE LOOP
        
        if (argc==3) {
            if (gen==atoi(argv[2])) {
                for (int i=1;i<25;i++) {
                    for (int j=1;j<81;j++) {
                        putchar(grid[current][i][j]);
                    }
                    putchar('\n');
                }
            }
        }
        
        
        current = swaparray(current);
        next = (current+1)%2; //change the roles of current and next between the arrays 0 and 1 of the 3d grid. This idea was adapted from the PA1 handout

        if (argc==3) {
            if (gen == atoi(argv[2])) { //atoi is taken from K&R textbook
                break;  //end the loop when desired generation is reached
            }
        }

        //************
        //ONLY UNCOMMENT USLEEP AND CLEARTERM WHEN LOCALLY RUNNING ANIMATIONS
        //KEEP THEM TURNED OFF TO SUBMIT TO AUTOLAB
        //usleep and clear before next generation
        /*
        usleep(300000);
        clearterm();
        */
    }
    
    return 0;
}
