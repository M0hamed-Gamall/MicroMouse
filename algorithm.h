void floodfill();
void move();
void set_walls();
void reintialize_vis();

typedef struct {
    int x;
    int y;
} Pair;

typedef struct {
    Pair data[260];
    int front;
    int back;
} Queue;




extern int current_x;
extern int current_y ;
extern bool solved ;

#define maze_size 6

extern int maze[maze_size][maze_size] ;
extern bool vis[maze_size][maze_size] ;
extern int wall_exist[maze_size][maze_size];
extern char current_direction;

