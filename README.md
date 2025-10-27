

# Console Battleship (C)

A small, single-player Battleship engine written in C. It reads a fleet of ships, places them on a 10×10 board, then accepts simple commands to print the board, inspect ships, and play a sequence of shots. Output reports hits, misses, and when ships are sunk.

## Features at a Glance

- 10×10 board with text rendering
- Up to 5 ships; each ship is a single letter (A–Z) on the board
- Horizontal or vertical placement from a top-left anchor
- Shots mark misses with `-` and hits with `*`
- Reports “Hit”/“Miss” per shot and announces when a ship is sunk
- Simple command loop driven by stdin

---

## Build

Use any C11-compatible compiler.

```bash
gcc -std=c11 -Wall -Wextra -O2 battleship.c -o battleship
```

---

## Run

The program reads from standard input until an exit command or EOF.

```bash
./battleship < input.txt
# or interactive:
./battleship
```

---

## Input Format

Input has two phases:
1) Fleet definition (header + ships)
2) Command loop

### 1) Fleet definition

- First line: a single integer N, the number of ships (0 ≤ N ≤ 5)
- Next N lines: one ship per line with 6 values:
  1. name (string without spaces, up to 50 chars)
  2. sym (single uppercase letter A–Z)
  3. place ('h' = horizontal, 'v' = vertical)
  4. length (positive int)
  5. top_left_row (0-based row index)
  6. top_left_col (0-based col index)

Constraints and behavior:
- Ship names cannot contain spaces (scanned with %s).
- Symbols must be uppercase A–Z.
- Orientation 'h' extends right, 'v' extends downward from the top-left coordinate.
- Placement fails if the ship would run off the board or overlap an existing ship.
- If any ship line is invalid, the program prints an error and exits.

### 2) Command loop

After the ships, the program reads commands (one letter) until an invalid command is given or EOF occurs.

- b
  - Print the current 10×10 board.
  - Legend: '.' = empty, 'A'–'Z' = ship segments, '-' = miss, '*' = hit
- s i
  - Print the details of ship at index i.
  - Indexing is 0-based, typically 0 ≤ i < number_of_ships.
  - Output format:
    - Name, Symbol, Vertical? (1/0), Length, Top Left Row/Col
- p M r0 c0 r1 c1 ... r(M-1) c(M-1)
  - Play M shots; for each shot, provide row and column (0-based).
  - For each shot, prints:
    - "Miss at Rr Cc" if shot hits water or repeats a previously hit cell
    - "Hit at Rr Cc" if shot hits a ship for the first time at that cell
    - When a ship's total hits reach its length: "<Ship Name> was sunk!"
- any other char
  - Prints "Exiting." and terminates.

Notes:
- Indices are 0-based: valid rows/cols are 0–9.
- M must be 0 ≤ M < 100.

---

## Example

Input:
```text
3
Destroyer D h 2 0 0
Sub S v 3 2 5
Battleship B h 4 9 6
b
p 6 0 0 0 1 2 5 3 5 9 6 9 9
s 0
x
```

Possible Output:
```text
DD........
..........
.....S....
.....S....
.....S....
..........
..........
..........
..........
......BBBB
Hit at R0 C0
Hit at R0 C1
Hit at R2 C5
Hit at R3 C5
Hit at R9 C6
Miss at R9 C9
Destroyer was sunk!
Sub was sunk!
Name: Destroyer, Symbol: D, Vertical?: 0, Length: 2, Top Left Row: 0, Top Left Col: 0
Exiting.
```

Explanation:
- Initial board printed.
- 6 shots are fired.
  - The first two hit the Destroyer; the second hit sinks it.
  - The next two hit the Sub; the second hit sinks it.
  - The fifth hits the Battleship; not sunk yet.
  - The sixth is a miss.
- Then we inspect ship 0 (Destroyer).
- 'x' exits.

---

## What Each Function Does (High-Level)

- print_ship: Prints a ship’s fields to stdout (name, symbol, orientation, length, top-left row/col).
- read_ship: Reads one ship from stdin and validates basic fields (name, symbol A–Z, 'h'/'v', length>0, non-negative coords). Initializes hits=0 and sets orientation.
- print_board: Prints the 10×10 board as 10 lines of 10 characters each.
- place_ship: Tries to place a ship on the board. Fails if it would exceed board bounds or overlap a non-empty cell.
- fire_shot: Applies a shot at (row, col). Returns '*' on hit (and marks the cell '*') or '-' on miss (and marks the cell '-'). A repeat shot at an already-hit cell returns '*'.
- play_battleship: Iterates through shots, calls fire_shot, prints "Hit"/"Miss", and when a ship accumulates hits equal to its length, prints that it was sunk.
- main: Initializes the board, reads N ships, places them, then processes the command loop.

---

## Board and Symbols

- '.' (dot): empty/unshot water
- 'A'–'Z': ship segments before they’re hit (each ship uses a single symbol)
- '*': a cell that was a ship and has been hit
- '-': a cell that was water and was shot (miss)

---

## Constraints and Limits

- Board: 10 rows × 10 cols
- MAX_SHIPS: 5
- MAX_SHOTS: 100
- Ship name length: up to 50 chars, no spaces
- Coordinates: zero-based (0–9)

---
