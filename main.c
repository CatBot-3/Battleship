#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>


const int MAX_SHIPS = 5;
const int MAX_SHOTS = 100;
const int MAX_STR_LEN = 50;
const int BOARD_ROWS = 10;
const int BOARD_COLS = 10;

struct ship {
	char name[51];     // ship's name (e.g. "Battleship")
	char sym;          // symbol for use on the board (e.g. 'B')
	bool is_vert;      // true if positioned vertically, false if horizontal
	int length;        // number of spaces occupied
	int top_left_row;  // row index of top-leftmost occupied square
	int top_left_col;  // column index of top-leftmost occupied square
	int hits;          // number of times this ship has been hit
};

// print_ship(shp) prints the fields of the ship pointed to by shp to stdout
// requires: shp is a valid ship
// effects: produces output
void print_ship(const struct ship *shp) {
	assert(shp);
	printf("Name: %s, Symbol: %c, Vertical?: %d, ",
	    	shp->name, shp->sym, shp->is_vert);
	printf("Length: %d, Top Left Row: %d, Top Left Col: %d\n",
			shp->length, shp->top_left_row, shp->top_left_col);
}

// read_ship(shp) reads the values of a ship from stdin and assigns them to the
//   ship pointed to by shp if and only if successful. Returns true if successful
//   and false if not.
// requires: shp is a valid struct ship
// effects: reads input,
//          may mutate the ship pointed to by shp
bool read_ship(struct ship *shp) {
    // TODO: your implementation here
    assert(shp);
    char name[51] = {0};
    char sym = '\0';
    char place = '\0';
    int length;
    int top_left_row;
    int top_left_col; //initialize readings
    bool exit_state = ((1 == scanf("%50s", name)) && (1 == scanf(" %c", &sym)) &&
                       (1 == scanf(" %c", &place)) && (1 == scanf("%d", &length)) &&
                       (1 == scanf("%d", &top_left_row)) &&
                       (1 == scanf("%d", &top_left_col)));
    if (exit_state) { //correct reading
        bool range_check = ((length > 0) && (top_left_col >= 0) && 
                            (top_left_row >= 0) && ((sym >= 'A') && (sym <= 'Z')) &&
                            ((place == 'v') || (place == 'h')));
        if (range_check) { //is within the range, then mutate
            strcpy(shp->name, name);
            shp->sym = sym;
            shp->length = length;
            shp->top_left_row = top_left_row;
            shp->top_left_col = top_left_col;
            shp->hits = 0;

            if (place == 'v') {
                shp->is_vert = true;
            } else {
                shp->is_vert = false;
            }
			return true;
        } else {
            return false; //not within the range
        }
    } else return false;
}


// TODO: your design recipe here
// print_board(board) prints the current board state to the screen
// requires: *board should be an array of size 100 and not NULL
// effect: will print to screen
void print_board(char *board) {
	// TODO: your implementation here
	assert (board);
	int row = 0;
	while ((row + 1) <= BOARD_ROWS) {
		for (int col = 0; (col + 1) <= BOARD_COLS; ++col) {
			printf("%c", *(board + col + BOARD_ROWS * row));
		}
		printf("\n");
		row += 1;
	}
}


// TODO: your design recipe here
// place_ship (shp, board) will place the ship on to the corresponding board
//    	if the ship size exceeds the edge of the board, place_ship will return false,
//		if the place was successful, place_ship will update the board state and return true
// effect: might mutate *board
// requires: shp and board are not NULL, *shp contains valid data and board shoule be valid of length 100
bool place_ship(const struct ship *shp, char *board) {
	// TODO: your implementation here
	assert(shp);
	assert(board);
	int len = shp->length;
	int row = shp->top_left_row;
	int col = shp->top_left_col;
	if (shp->is_vert) { // vertical
		if (row + len <= BOARD_ROWS) { //within range
			while (len >= 1) {
				if (board[row * BOARD_COLS + col] != '.') return false;
				board[row * BOARD_COLS + col] = shp->sym;
				++row;
				--len;
			}
			return true;

		} else return false;
	} else { //horizontal
		if (col + len <= BOARD_COLS) { //within range
			while (len >= 1) {
				if (board[row * BOARD_COLS + col] != '.') return false;
				board[row * BOARD_COLS + col] = shp->sym;
				col += 1;
				len -= 1;
			}
			return true;
		} else return false;
	}
}


// fire_shot(board, shot_row, shot_col) simulates firing a shot on the given
//   board at row, shot_row, and column, shot_col. Changes the symbol at that
//   position to '-' if a miss, and '*' if a hit.
// requires: 0 <= shot_row < BOARD_ROWS,
//           0 <= shot_col < BOARD_COLS,
//           board is not NULL,
//           board has length BOARD_ROWS * BOARD_COLS (not asserted)
// effects: mutates the board pointed to by board
char fire_shot(char *board, const int shot_row, const int shot_col) {
	// TODO: your implementation here
	assert (0 <= shot_row < BOARD_ROWS);
	assert (0 <= shot_col < BOARD_COLS);
	assert (board);
	int index = shot_col + shot_row * BOARD_COLS;
	char piece = *(board + index);
	if (piece == '*') {
		return '*';
	} else if (('A' <= piece) && (piece <= 'Z')) { // hit
		*(board + index) = '*';
		return '*';
	} else {
		*(board + index) = '-';
		return '-';
	}
}


// play_battleship(ship_arr, num_ships, board, num_shots, shot_rows, shot_cols)
//   simulates a (single-player) game of Battleship on the given board. ship_arr
//   contains a number of ships equal to num_ships. shot_rows and 
//   shot_cols each contain num_shots items. The row of shot i is given by
//   shot_rows[i], and the column by shot_cols[i].
//   Prints a message to stdout for each shot that misses, hits,
//   and each ship sunk.
// requires: ship_arr is not NULL,
//           ships in ship_arr are already added to board (not asserted)
//           board is not NULL,
//           shot_rows is not NULL,
//           shot_cols is not NULL,
//           shot_rows and shot_cols have length num_shots (not asserted)
// effects: mutates the board pointed to by board
//          may mutate the ships in ship_arr
//          may produce output
void play_battleship(struct ship *ship_arr, const int num_ships, char *board,
					const int num_shots, const int *shot_rows,
					const int *shot_cols) {
	// TODO: your implementation here
	assert (ship_arr);
	assert (board);
	assert (shot_rows);
	assert (shot_cols);

    for (int i = 0; i < num_shots; ++i) {
		int row = *(shot_rows + i);
		int col = *(shot_cols + i);
		char piece = *(board + col + row * BOARD_COLS);
		char piece_after = fire_shot(board, row, col);
		if (piece_after == '-') { // miss
			printf("Miss at R%d C%d\n", row, col);
		} else if ((piece_after == '*' && (piece == '*'))) { // hit before
			printf("Miss at R%d C%d\n", row, col);
		} else { // hit for first time
			printf("Hit at R%d C%d\n", row, col);
			for (int j = 0; j < num_ships; ++j) {
				if ((ship_arr + j)->sym == piece) { // this boat got hit
					++(ship_arr + j)->hits;
					if ((ship_arr + j)->hits == (ship_arr + j)->length) { //sunk
						printf("%s was sunk!\n", (ship_arr + j)->name);
					}
				}
			}
		}
	}
}

int main(void) {
	// Hard coded test harness will dictate what .in and .expect files 
	// are like for .in file:
	// first line is 1 integer, N, giving the number of ships
	// the next N lines give 1 ship each with 6 values:
	//   a string, the ship's name
	//   a char, a capital letter for representing the ship on the board
	//   a char, either h (horizontal placement) or v (vertical placement)
	//   an int, the length of the ship
	//   an int, the row of the top left square occupied by the ship
	//   an int, the column of the top left square occupied by the ship
	// until the end of the file, read lines beginning with a char:
	//   b: print the board
	//   s: print a ship
	//      followed by an int, the index of the ship to be printed
	//   p: play Battleship
	//      followed by int M, giving the number of shots
	//      followed by 2*M ints giving the row and column of shots
	//        (in the order row_0 col_0 row_1 col_1 row_2 col_2 ...)
	//   any other char: exit program
	// See sample .in and .expect files for examples.

	int num_ships = 0;
	char board[10 * 10];
	struct ship ship_arr[5];

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < BOARD_COLS; ++j) {
			board[i * BOARD_COLS + j] = '.';
		}
	}

	// read in ships
	if (!(scanf("%d", &num_ships) == 1)) {
		printf("Failed to read number of ships.\n");
		return 0;
	}
	if (num_ships > MAX_SHIPS) {
		printf("Number of ships exceeds maximum.\n");
		return 0;
	}
	for (int i=0; i<num_ships; ++i) {
		if (!read_ship(&(ship_arr[i]))) {
			printf("Invalid ship.\n");
			return 0;
		} else if (!place_ship(&(ship_arr[i]), board)) {
			printf("Invalid ship placement.\n");
			return 0;
		}
	}
	

	char command = '\0';
	int print_index = 0;
	int num_shots = 0;
	int shot_rows[100];
	int shot_cols[100];

	// read in commands
	while (scanf(" %c", &command) == 1) {
		if (command == 'b') {
			print_board(board);
		} else if (command == 's') {
			if (scanf("%d", &print_index) == 1 && print_index >= 0 &&
			   print_index < MAX_SHIPS) {
				print_ship(&(ship_arr[print_index]));
			} else {
				printf("Given invalid ship index to print.\n");
				return 0;
			}
		} else if (command == 'p') {
			if (scanf("%d", &num_shots) == 1 && num_shots >= 0 &&
			   num_shots < MAX_SHOTS) {
				for (int i = 0; i < num_shots; ++i) {
					bool read_success = scanf("%d", &(shot_rows[i])) == 1;
					read_success = read_success &&
					               scanf("%d", &(shot_cols[i])) == 1;
					if (!read_success) {
						printf("Failed to read shot coordinates.\n");
						return 0;
					}
				}
				play_battleship(ship_arr, num_ships, board,
				                num_shots, shot_rows, shot_cols);
			} else {
				printf("Given invalid number of shots.\n");
				return 0;
			}
		} else {
			printf("Exiting.\n");
			return 0;
		}
	}
}
