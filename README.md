Tower of Hanoi Solver (Extended Version)
Description

This project implements an advanced solution to the classic Tower of Hanoi problem.
It supports both the traditional 3-peg version and an extended version with up to 10 pegs.

The program computes and prints the optimal sequence of moves required to solve the puzzle.

⚙️ Features

✅ Classic 3-peg solution:

Recursive approach

Iterative approach

✅ Multi-peg solution (4–10 pegs):

Uses the Frame–Stewart algorithm

Computes optimal move counts using dynamic programming

✅ Efficient computation:

Precomputes minimal moves for all configurations

Stores optimal split points for reconstruction

🧠 Algorithms Used

Recursion

Iterative simulation

Dynamic Programming

Frame–Stewart algorithm (multi-peg Hanoi)

▶️ How to Run

Compile:

g++ main.cpp -o hanoi

Run:

./hanoi

Input:

Number of pegs (3–10)

Number of discs

📊 Example
Input:
3
3

Output:
1 to 3
1 to 2
3 to 2
1 to 3
2 to 1
2 to 3
1 to 3
