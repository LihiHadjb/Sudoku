# Sudoku
This is a Sudoku game that has the following functionalities:
- Solving Sudoku puzzles
- Editing existing and creating new Sudoku puzzles
- Supporting puzzles of any size
- Validating the board
- Saving and loading puzzles from files
- Randomly generating new puzzles
- Finding the number of solutions for a given board
- Auto-filling option for obvious cells (where only a single value is valid) 

The available commands are (an argument with [] is optional):
1. solve X

  Starts a puzzle in Solve mode, loaded from a file with the name "X"

2. edit [X]

  Starts a puzzle in Edit mode, loaded from a file with the name "X". If no parameter is supplied, the program enters Edit mode with an empty 9x9 board.

3. mark_errors X

  x=0 or x=1 detemines whether error values will be marked to the user

4. print_board

5. set X Y Z

  Sets the value of cell <X,Y> to Z.

6. validate

  Validates the current board using ILP, ensuring it is solvable.

7. guess X

  Guesses a solution to the current board using LP , with threshold X. The command fills all cell values with a score of X or greater.

8. generate X Y

   Generates a puzzle by randomly filling X empty cells with legal values, running ILP to solve the board, and then clearing all but Y random cells.

9. save X

   Saves the current game board to the specified file, where X includes a full or relative path to the file.

10. hint X Y

  Give a hint to the user by showing the solution of a single cell X,Y.

11. guess_hint X Y

  Show a guess to the user for a single cell X,Y, using LP.

12. num_solutions

  Print the number of solutions for the current board.

13. autofill

  Automatically fill "obvious" values – cells which contain a single legal value.

14. exit
