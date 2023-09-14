#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define ROWS 10
#define COLUMNS 10


#define VISUAL_LEN 3

#define ALIVE_VISUAL "[#]"
#define DEAD_VISUAL "   "

#define ALIVE_SELECTED_VISUAL "[#]"
#define AlIVE_UNSELECTED_VISUAL " # "
#define DEAD_SELECTED_VISUAL "[_]"
#define DEAD_UNSELECTED_VISUAL " _ "


void initialise_map(int map[ROWS][COLUMNS]);
int check_alive(int map[ROWS][COLUMNS], int r, int c);
int update_map(int map[ROWS][COLUMNS]);
void switch_state(int map[ROWS][COLUMNS], int r, int c);

void display_map(int map[ROWS][COLUMNS]);
void display_map_cursor(int map[ROWS][COLUMNS], int cursor_r, int cursor_c);

void run_editor(int map[ROWS][COLUMNS]);
int main() {

	int map[ROWS][COLUMNS];
	initialise_map(map);

	//	Glider
	map[0][2] = 1;
	map[1][0] = 1;
	map[1][2] = 1;
	map[2][1] = 1;
	map[2][2] = 1;


	while (1){
		display_map(map);
		printf("\n(e) Edit map\n(r) Run map\n(x) Exit\n> ");
		char option;
		scanf(" %c", &option);

		if (option == 'x') {
			return 0;
		}

		if (option == 'e') {
			run_editor(map);
		}			
		if (option == 'r') {
			int iterations;
			printf("iterations\n> ");
			scanf(" %d", &iterations);

			display_map(map);
			int i = 0;
			while (update_map(map) && i < iterations) {

				Sleep(100);
				display_map(map);
				i++;
			}
		}
	}


return 0;
}


void initialise_map(int map[ROWS][COLUMNS]) {

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {
			map[r][c] = 0;
		}
	}
}

int check_alive(int map[ROWS][COLUMNS], int r, int c) {
	if (r < 0 || c < 0 || r > ROWS - 1 || c > COLUMNS - 1) {
		return 0;
	}


	return map[r][c];
}

int update_map(int map[ROWS][COLUMNS]) {

	int running = 0;
	int same = 1;

	int next[ROWS][COLUMNS];

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {

			// Counting the amount of alive neighbouring cells

			int neighbours =
				check_alive(map, r - 1, c - 1) + 
				check_alive(map, r - 1, c + 0) +
				check_alive(map, r - 1, c + 1) +

				check_alive(map, r + 0, c - 1) + 
				check_alive(map, r + 0, c + 1) +

				check_alive(map, r + 1, c - 1) + 
				check_alive(map, r + 1, c + 0) +
				check_alive(map, r + 1, c + 1);

			// If there is still activity, running is true
			if (neighbours) {
				running = 1;
			}

			next[r][c] = 0;

			if (neighbours == 3) {
				next[r][c] = 1;
			}

			if (neighbours == 2 && map[r][c] == 1) {
				next[r][c] = 1;
			}
		}
	}

	// update all values in map to the values in the next arr
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {

			if (map[r][c] != next[r][c]) {
				same = 0;
			}
			map[r][c] = next[r][c];
		}
	}

	// return the running status
	return running && !same;
}

void switch_state(int map[ROWS][COLUMNS], int r, int c) {
	if (r < 0 || c < 0 || r > ROWS - 1 || c > COLUMNS - 1) {
		return;
	}

	if (map[r][c] == 0) {
		map[r][c] = 1;
	} else {
		map[r][c] = 0;
	}
}

void display_map(int map[ROWS][COLUMNS]) {
	system("cls");

	for (int i = 0; i < (COLUMNS*VISUAL_LEN)+2; i++) {
		printf("=");
	}
	printf("\n");

	for (int r = 0; r < ROWS; r++) {	
		printf("|");

		for (int c = 0; c < COLUMNS; c++) {


			if (map[r][c] == 1) {
				printf(ALIVE_VISUAL);
			} else {
				printf(DEAD_VISUAL);
			}
		}
		printf("|\n");
	}

	for (int i = 0; i < (COLUMNS*VISUAL_LEN)+2; i++) {
		printf("=");
	}
}
void display_map_cursor(int map[ROWS][COLUMNS], int cursor_r, int cursor_c) {
	system("cls");
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {

			if (r == cursor_r && c == cursor_c) {

				if (map[r][c] == 1) {
					printf(ALIVE_SELECTED_VISUAL);
				} else {
					printf(DEAD_SELECTED_VISUAL);
				}
			} else {
				if (map[r][c] == 1) {
					printf(AlIVE_UNSELECTED_VISUAL);
				} else {
					printf(DEAD_UNSELECTED_VISUAL);
				}
			}
		}
		printf("\n");
	}
}


void run_editor(int map[ROWS][COLUMNS]){

	int cursor_r = 0;
	int cursor_c = 0;

	while (1) {
		display_map_cursor(map, cursor_r, cursor_c);
		printf("use wasd for movement, press space to flip cell, x to exit");

		char movement = getch();

		if (movement == ' ') {
			switch_state(map, cursor_r, cursor_c);
		}
		if (movement == 'x') {
			return;
		}


		if (movement == 'w') {
			cursor_r -=1;
		}
		if (movement == 's') {
			cursor_r += 1;
		}
		
		if (movement == 'a') {
			cursor_c -=1;
		}
		if (movement == 'd') {
			cursor_c += 1;
		}

		// KEEP CURSOR WITHIN BOUNDS
		if (cursor_r < 0) {
			cursor_r = 0;
		}
		if (cursor_r >= COLUMNS) {
			cursor_r = COLUMNS-1;
		}
		if (cursor_c < 0) {
			cursor_c = 0;
		}
		if (cursor_c >= COLUMNS) {
			cursor_c = COLUMNS-1;
		}
	}
}
