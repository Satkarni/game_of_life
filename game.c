#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FRAME_RATE	( 10 )
#define FRAME_DELAY	( 1000000/FRAME_RATE )

#define SZW		( 60 )
#define SZH		( 30 )
#define DENSITY	( 0.2f )

static int cells[SZW*SZH];
static int ncells[SZW*SZH];

void init_cells()
{
	for(int x=0;x<SZW;x++){
		for(int y=0;y<SZH;y++){
			float f = ( (float)rand()/(float)(RAND_MAX) );
			if(f <= DENSITY){
				cells[x + y*SZH] = 1;
			}
		}
	}
}

int is_valid(int x,int y)
{
	return ( x >= 0 && x < SZW && y >= 0 && y < SZH );
}

int get_nbrs(int x,int y)
{
	int nbrs = 0;
	
	for(int i=-1;i<= 1;i++){
		for(int j=-1;j<=1;j++){
			int nx = i + x; int ny = j + y; 
			if(nx == x && ny == y) { continue; }
			if(is_valid(nx,ny)){
				nbrs += cells[nx + ny*SZH];
			}
		}
	}

	return nbrs;
}
void update()
{
	memset(ncells,0,sizeof(ncells));

	memcpy(ncells,cells,sizeof(cells));

	for(int x=0;x<SZW;x++){
		for(int y=0;y<SZH;y++){
			int nbrs = get_nbrs(x,y);
			if(cells[x+y*SZH] == 1){
				if(nbrs < 2 || nbrs > 3){
					ncells[x + y*SZH] = 0;
				}	
			}else{
				if(nbrs == 3){
					ncells[x + y*SZH] = 1;
				}
			}
		}
	}

	memcpy(cells,ncells,sizeof(cells));
}


void draw()
{
	for(int x=0;x<SZW;x++){
		for(int y=0;y<SZH;y++){
			if(cells[x+y*SZH] == 1){
				mvprintw(y,x,"#");
			}
		}
	}
}
int main()
{
	initscr();
	cbreak();
	nodelay(stdscr,TRUE);	
	keypad(stdscr,TRUE);
	scrollok(stdscr,TRUE);
	noecho();
	curs_set(FALSE);
	srand(time(NULL));

	init_cells();
	while(1){
		clear();
		draw();
		update();	
		refresh();
		usleep(FRAME_DELAY);
	}
	
	endwin();
	return 0;
}
