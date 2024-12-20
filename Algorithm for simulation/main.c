#include "API.h"
#include <stdio.h>
#include <stdbool.h>

int dx[4] = {1, 0, -1, 0}; //  order matched with the directions for the maze
int dy[4] = {0, 1, 0, -1};

int current_x = 0;
int current_y = 0;
bool solved = false;
//bool return_0 = false;

#define maze_size 16

int maze[maze_size][maze_size] = {0};
bool vis[maze_size][maze_size] = {0};
int wall_exist[maze_size][maze_size] = {0};


char current_direction = 'n';


#define NORTH 1    // 0001
#define EAST  2    // 0010
#define SOUTH 4    // 0100
#define WEST  8    // 1000



// start queue
typedef struct {
    int x;
    int y;
} Pair;

typedef struct {
    Pair data[260];
    int front;
    int back;
} Queue;

void initQueue(Queue* q) {
    q->front = 0;
    q->back = 0;
}

void enqueue(Queue* q, int x, int y) {
    q->data[q->back].x = x;
    q->data[q->back].y = y;
    q->back++;
}

Pair dequeue(Queue* q) {
    return q->data[q->front++];
}

int isEmpty(Queue* q) {
    return q->front == q->back;
}

void intToString(int num, char* str) {
    sprintf(str, "%d", num);
}
// end queue


Pair path[500] ;
int start = 1;

void floodfill() {
    Queue q;
    initQueue(&q);
    enqueue(&q, maze_size/2, maze_size/2);

    maze[maze_size/2][maze_size/2] = 0;
    vis[maze_size/2][maze_size/2] = true;
    API_setText(maze_size/2, maze_size/2, "0");
    while (!isEmpty(&q)) {
        Pair p = dequeue(&q);

        for (int i = 0; i < 4; i++) {
            int xx = p.x + dx[i];
            int yy = p.y + dy[i];
            if (xx < maze_size && xx >= 0 && yy < maze_size && yy >= 0) {
                if (!vis[xx][yy] && !(wall_exist[p.x][p.y] & (1 << i)) ) { // (1 << i) matches the directions
                    enqueue(&q, xx, yy);
                    vis[xx][yy] = true;
                    maze[xx][yy] = maze[p.x][p.y] + 1;

                    int number = maze[xx][yy];
                    char str[20];
                    intToString(number, str);
                    API_setText(yy, xx, str);
                }
            }
        }
    }
}

void set_walls(int x, int y) {
    if (current_direction == 'n') {
        if (API_wallFront()) {
            wall_exist[x][y] |= NORTH;
            if(x+1 < maze_size)
                wall_exist[x + 1][y] |= SOUTH;
            API_setWall(y, x, 'n');
        }
        if (API_wallLeft() ) {
            wall_exist[x][y] |= WEST;
            if(y - 1 >= 0)
                wall_exist[x][y - 1] |= EAST;
            API_setWall(y, x, 'w');
        }
        if (API_wallRight()) {
            wall_exist[x][y] |= EAST;
            if(y + 1 < maze_size)
                wall_exist[x][y + 1] |= WEST;
            API_setWall(y, x, 'e');
        }
    } else if (current_direction == 'e') {
        if (API_wallFront()) {
            wall_exist[x][y] |= EAST;
            if(y + 1 < maze_size)
                wall_exist[x][y + 1] |= WEST;
            API_setWall(y, x, 'e');
        }
        if (API_wallLeft()) {
            wall_exist[x][y] |= NORTH;
            if(x + 1 < maze_size)
                wall_exist[x + 1][y] |= SOUTH;
            API_setWall(y, x, 'n');
        }
        if (API_wallRight()) {
            wall_exist[x][y] |= SOUTH;
            if(x - 1 >= 0)
                wall_exist[x - 1][y] |= NORTH;
            API_setWall(y, x, 's');
        }
    } else if (current_direction == 'w') {
        if (API_wallFront() ) {
            wall_exist[x][y] |= WEST;
            if(y - 1 >= 0)
                wall_exist[x][y - 1] |= EAST;
            API_setWall(y, x, 'w');
        }
        if (API_wallLeft() ) {
            wall_exist[x][y] |= SOUTH;
            if(x - 1 >= 0)
                wall_exist[x - 1][y] |= NORTH;
            API_setWall(y, x, 's');
        }
        if (API_wallRight()) {
            wall_exist[x][y] |= NORTH;
            if(x + 1 < maze_size)
                wall_exist[x + 1][y] |= SOUTH;
            API_setWall(y, x, 'n');
        }
    } else if (current_direction == 's') {
        if (API_wallFront()) {
            wall_exist[x][y] |= SOUTH;
            if(x - 1 >= 0)
                wall_exist[x - 1][y] |= NORTH;
            API_setWall(y, x, 's');
        }
        if (API_wallLeft()) {
            wall_exist[x][y] |= EAST;
            if(y + 1 < maze_size)
                wall_exist[x][y + 1] |= WEST;
            API_setWall(y, x, 'e');
        }
        if (API_wallRight() ) {
            wall_exist[x][y] |= WEST;
            if(y - 1 >= 0)
                wall_exist[x][y - 1] |= EAST;
            API_setWall(y, x, 'w');
        }
    }
}

void log(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

void move() {
    int x_go=0, y_go=0;
    bool find_a_cell = false;
    int mn = maze[current_x][current_y];
    for (int i = 0; i < 4; i++) {
        int xx = current_x + dx[i];
        int yy = current_y + dy[i];

        if (xx >= 0 && xx < maze_size && yy >= 0 && yy < maze_size  ) {
            if(!(wall_exist[current_x][current_y] & (1 << i)) && (maze[xx][yy] <= mn)  ){
                fprintf(stderr, " %d , %d  >>>>> %d , %d\n", current_x , current_y , xx , yy);
                fflush(stderr);

                fprintf(stderr, "maze[%d][%d] = %d\n",  xx , yy ,maze[xx][yy] );
                fflush(stderr);
                mn = maze[xx][yy];
                x_go = xx;
                y_go = yy;
                find_a_cell = true;
            }
        }
    }

    fprintf(stderr, "current  %d , %d  \n", current_x , current_y );
    fflush(stderr);

    if (find_a_cell) {
        char log_msg[128];
        sprintf(log_msg, "%d %d ->>> %d %d direction: %c", current_x, current_y, x_go, y_go, current_direction);
        log(log_msg);

        if (current_direction == 'n') {
            if (y_go > current_y) {
                log("Right turn");
                API_turnRight();
                API_moveForward();
                current_direction = 'e';
            } else if (y_go < current_y) {
                log("Left turn");
                API_turnLeft();
                API_moveForward();
                current_direction = 'w';
            } else if (x_go > current_x) {
                log("Front");
                API_moveForward();
            } else if (x_go < current_x) {
                log("Back");
                API_turnRight();
                API_turnRight();
                API_moveForward();
                current_direction = 's';
            }
        } else if (current_direction == 's') {
            if (y_go > current_y) {
                log("Left turn");
                API_turnLeft();
                API_moveForward();
                current_direction = 'e';
            } else if (y_go < current_y) {
                log("Right turn");
                API_turnRight();
                API_moveForward();
                current_direction = 'w';
            } else if (x_go < current_x) {
                log("Front");
                API_moveForward();
            } else if (x_go > current_x) {
                log("Back");
                API_turnRight();
                API_turnRight();
                API_moveForward();
                current_direction = 'n';
            }
        } else if (current_direction == 'w') {
            if (x_go < current_x) {
                log("Left turn");
                API_turnLeft();
                API_moveForward();
                current_direction = 's';
            } else if (x_go > current_x) {
                log("Right turn");
                API_turnRight();
                API_moveForward();
                current_direction = 'n';
            } else if (y_go < current_y) {
                log("Front");
                API_moveForward();
            } else if (y_go > current_y) {
                log("Back");
                API_turnRight();
                API_turnRight();
                API_moveForward();
                current_direction = 'e';
            }
        } else if (current_direction == 'e') {
            if (x_go > current_x) {
                log("Left turn");
                API_turnLeft();
                API_moveForward();
                current_direction = 'n';
            } else if (x_go < current_x) {
                log("Right turn");
                API_turnRight();
                API_moveForward();
                current_direction = 's';
            } else if (y_go > current_y) {
                log("Front");
                API_moveForward();
            } else if (y_go < current_y) {
                log("Back");
                API_turnRight();
                API_turnRight();
                API_moveForward();
                current_direction = 'w';
            }
        }
        current_x = x_go;
        current_y = y_go;
        path[start].x = current_x;
        path[start].y = current_y;
        start++;


    } else {
        log("there is no cell to go");
        solved = true;
        start--;

    }
}

//void return_00() {
//
//    fprintf(stderr, "current  %d , %d  \n", current_x , current_y );
//    fflush(stderr);
//
//    start--;
//    if (start >= 0) {
//    int x_go = path[start].x ;
//    int y_go = path[start].y ;
//
//        char log_msg[128];
//        sprintf(log_msg, "%d %d ->>> %d %d direction: %c", current_x, current_y, x_go, y_go, current_direction);
//        log(log_msg);
//
//        if (current_direction == 'n') {
//            if (y_go > current_y) {
//                log("Right turn");
//                API_turnRight();
//                API_moveForward();
//                current_direction = 'e';
//            } else if (y_go < current_y) {
//                log("Left turn");
//                API_turnLeft();
//                API_moveForward();
//                current_direction = 'w';
//            } else if (x_go > current_x) {
//                log("Front");
//                API_moveForward();
//            } else if (x_go < current_x) {
//                log("Back");
//                API_turnRight();
//                API_turnRight();
//                API_moveForward();
//                current_direction = 's';
//            }
//        } else if (current_direction == 's') {
//            if (y_go > current_y) {
//                log("Left turn");
//                API_turnLeft();
//                API_moveForward();
//                current_direction = 'e';
//            } else if (y_go < current_y) {
//                log("Right turn");
//                API_turnRight();
//                API_moveForward();
//                current_direction = 'w';
//            } else if (x_go < current_x) {
//                log("Front");
//                API_moveForward();
//            } else if (x_go > current_x) {
//                log("Back");
//                API_turnRight();
//                API_turnRight();
//                API_moveForward();
//                current_direction = 'n';
//            }
//        } else if (current_direction == 'w') {
//            if (x_go < current_x) {
//                log("Left turn");
//                API_turnLeft();
//                API_moveForward();
//                current_direction = 's';
//            } else if (x_go > current_x) {
//                log("Right turn");
//                API_turnRight();
//                API_moveForward();
//                current_direction = 'n';
//            } else if (y_go < current_y) {
//                log("Front");
//                API_moveForward();
//            } else if (y_go > current_y) {
//                log("Back");
//                API_turnRight();
//                API_turnRight();
//                API_moveForward();
//                current_direction = 'e';
//            }
//        } else if (current_direction == 'e') {
//            if (x_go > current_x) {
//                log("Left turn");
//                API_turnLeft();
//                API_moveForward();
//                current_direction = 'n';
//            } else if (x_go < current_x) {
//                log("Right turn");
//                API_turnRight();
//                API_moveForward();
//                current_direction = 's';
//            } else if (y_go > current_y) {
//                log("Front");
//                API_moveForward();
//            } else if (y_go < current_y) {
//                log("Back");
//                API_turnRight();
//                API_turnRight();
//                API_moveForward();
//                current_direction = 'w';
//            }
//        }
//            current_x = x_go;
//            current_y = y_go;
//    } else {
//        log("there is no cell to go");
//        return_0 = true;
//
//    }
//
//}


void reintialize_vis() {
    for (int i = 0; i < maze_size ; i++) {
        for (int j = 0; j < maze_size; j++) {
            vis[i][j] = false;
        }
    }
}

int main() {

    path[0].x =0;
    path[0].y=0;
    reintialize_vis();
    while (!solved) {
        set_walls(current_x, current_y);
        floodfill();
        move();
        reintialize_vis();
    }
//    while(!return_0){
//        return_00();
//    }


//    API_turnRight();
//    API_turnRight();
//API_moveForward();
    return 0;
}
