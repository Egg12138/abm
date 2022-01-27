//#include <graphics.h>

#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define SLEEPTIME 1.1
#define NCOLUMN 30 // number of column
#define NROW 30 // number of rows
#define DEAD 0
#define SUCCESS 255 
#define ACTIVE 1
#define INIT 0 

typedef short STATUS;
//typedef STATUS *map[NCOLUMN];
void cli_show_map(STATUS map[][NCOLUMN]);
STATUS is_active(STATUS map[][NCOLUMN], int row_idx, int col_idx);
short  n_active_near(STATUS map[NROW][NCOLUMN], int row_idx, int col_idx);
int total_active(STATUS map[NROW][NCOLUMN]);
STATUS map_init(STATUS map[NROW][NCOLUMN]);
void show_top(int col);
void show_bottom(int col);
void iter_step(STATUS map[][NCOLUMN]);
bool is_stable(STATUS map[][NCOLUMN]);
//TODO:
void show_manual(void);
void show_version_info(void);
void scale_configuration(void); 
/*
STATUS **anothermain(void) 
{
	map_ptr = (STATUS *) malloc(NROW * sizeof(STATUS * NCOLUMN));
	if (map_ptr == NULL) {
		fputs("Memeroy Allocation failed.");
		exit(EXIT_FAILURE);
	} else {
		...
			return ....
	}	
}
*/
static unsigned step = 0;
int main( int argc, char * argv[]) 
{
	/*Simple Arg Parser*/	
	bool checkstable = false;
	bool start = true;
	if (argc > 1) {
		if ((*argv)[0] == '-') {//The first char of the first argument
			switch (argv[0][1]) {
			default:
				printf("Unknown option -%c\n\n", argv[0][1]); 
				break;
			case 'h':  start = false; show_manual(); break;	
			case 'c':  checkstable = true; 
				   break ;
			case 'v':  show_version_info(); 
				   break;
			case 's':  scale_configuration(); 
				   break;
		}
	}
	}
	/*  Random Generation */
	time_t t;	
	int repeat_times = 5;
	srand((unsigned) time(&t));
       	static STATUS map[NROW][NCOLUMN]; //A zero matrix.
	map_init(map);	

	if (checkstable) {
	while (start) {
		cli_show_map(map);
		rewind(stdout);
		iter_step(map);
	//printf("\033[2J\033[1;1H]]");
		printf("\033[1A\033[K");
		sleep(SLEEPTIME);
		if (is_stable(map))
			break;
	}
	} else {

	// 减少if分支，所以这里分了两套进行	
	while (start) {
	cli_show_map(map);
	rewind(stdout);
	iter_step(map);
	printf("\033[1A\033[K");
	sleep(SLEEPTIME);
	}

	return 0;
}
}

void iter_step(STATUS map[NROW][NCOLUMN]) {
	
	int row = 0, col = 0;
	short num_near;
//	static unsigned step = 0;
	for ( row = 0; row < NROW; row++) {
		for ( col = 0; col < NCOLUMN; col++) {
			num_near = n_active_near(map, row, col);
				switch (num_near) {
					case 3: map[row][col] = ACTIVE; break;
					case 2: break;
					default: map[row][col] = DEAD; break;
				}	



	}
}
	step++;
}
STATUS map_init(STATUS map[NROW][NCOLUMN]) {

	
	for (int row = 0; row < NROW; row++) {
		for (int col = 0; col < NCOLUMN; col++) {
			map[row][col] = rand() % 2;
			printf("%d ", map[row][col]);
		}
	}

}
STATUS is_active(STATUS map[][NCOLUMN], int row_idx, int col_idx) {
	return map[row_idx][col_idx];
} 

short n_active_near(STATUS map[NROW][NCOLUMN], int row_idx, int col_idx) {
	if (row_idx > NROW || col_idx > NCOLUMN)
		perror("Out of range");

	int active_cnt = 0;

	if (col_idx> 0) { 
		active_cnt += map[row_idx][col_idx - 1];

		if (row_idx > 0) {
			int tmpsum = map[row_idx - 1][col_idx - 1] 
				   + map[row_idx - 1][col_idx];
			active_cnt += tmpsum;
			
		}
		if (row_idx < NROW) {
			int tmpsum = map[row_idx + 1][col_idx -1]
				   + map[row_idx + 1][col_idx];
			active_cnt += tmpsum;
		}
	} else {
		if (row_idx > 0) 
			active_cnt += map[row_idx - 1][0];
			
		if (row_idx < NROW) 
			active_cnt += map[row_idx + 1][0];

	}

	if (col_idx < NCOLUMN) {
		if (row_idx > 0) 
		        active_cnt += map[row_idx - 1][col_idx + 1];
			
		
		if (row_idx < NROW) 
			active_cnt += map[row_idx + 1][col_idx + 1];
		
	} else {
		if (row_idx > 0) 
			active_cnt += map[row_idx - 1][NCOLUMN];
			
		if (row_idx < NROW) 
			active_cnt += map[row_idx + 1][NCOLUMN];

		}
	return active_cnt;
}
void show_top(int cols) {
	char * left = "\n/";
	char * centre = "=";
	char * right = "\\\n";
	fputs(left, stdout);
	
	for (int i = 0; i <= 2 * cols; i++)
		fputs(centre, stdout);
	fputs(right, stdout);


}
void show_bottom(int cols) {
	char * left = "\\";
	char * centre = "=";
	char * right = "/";
	fputs(left, stdout);
	for (int i = 0; i <= 2 * cols; i++)
		fputs(centre, stdout);
	fputs(right, stdout);
}	
void cli_show_map(STATUS map[NROW][NCOLUMN]) {
	int row = 0, col = 0;
	int num_active = total_active(map);
	int num_total = NROW * NCOLUMN;
	char *active_icon = " #";
	char *dead_icon = "  ";
	char *left = "|"; 
	char *right = " |\n";
	show_top(NCOLUMN);
	for (int row = 0; row < NROW; row++) {
		fputs(left, stdout);	
		for (int col = 0; col < NCOLUMN; col++) {
			switch (map[row][col]) {
				
				case ACTIVE : fputs(active_icon, stdout);
					      break;
				case DEAD   : fputs(dead_icon, stdout);
					      break;
			}		
		}
		fputs(right, stdout);
	}
	show_bottom(NCOLUMN);	
	for (int i = 0; i < (int)(NCOLUMN / 2); i++)
		fputc(' ', stdout);
	printf("[%d/%d]", num_active, num_total);
}

int total_active(STATUS map[NROW][NCOLUMN]) {
	int num = 0;
	for (int row = 0; row < NROW; row++) { 
		for (int col = 0; col < NCOLUMN; col++)
			num += map[row][col];
	}
	return num;
}


bool is_stable(STATUS map[NROW][NCOLUMN]) {
	static short stored_ele[NROW * NCOLUMN] = {0};
	bool isstable = true;
	short current_elements[NROW * NCOLUMN] = {0};
	for (int row = 0; row < NROW; row++) {
		for (int col = 0; col < NCOLUMN; col++) {
			if (stored_ele[row * NCOLUMN + col] != map[row][col]) {
			       isstable = false;	
			       stored_ele[row * NCOLUMN + col] = map[row][col]; //make a copy
			       
		}
	}
		
	}
	return isstable;

}

void show_manual(void){
	char * program_name = "gamelife_toy";	
	char * program_usage = "simulation_of_gamelife";
	printf("\nDescription:  %s ,\nUsage: 	%s [OPTION] ... [-c] CLOSE STABLE STATUS \n", program_name, program_usage);

}

void show_version_info(void) {
	char * version = "0.0.1";
	printf("Conway's CLI Gamelife --Version(%.5s)", version);
}

void scale_configuration(void) {
	printf("Unfinished");
}
