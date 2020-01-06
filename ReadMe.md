# Game of Life

Implemented Conway's Game of Life for my master thesis course in "Efficient Programming" with OpenMP and OpenCL.

## Rules 

as by [Wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life):
>The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, alive or dead, (or populated and unpopulated, respectively). Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent. At each step in time, the following transitions occur:
>
>- Any live cell with fewer than two live neighbours dies, as if by underpopulation.
>- Any live cell with two or three live neighbours lives on to the next generation.
>- Any live cell with more than three live neighbours dies, as if by overpopulation.
>- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
>
>The initial pattern constitutes the seed of the system. The first generation is created by applying the above rules simultaneously to every cell in the seed; births and deaths occur simultaneously, and the discrete moment at which this happens is sometimes called a tick. Each generation is a pure function of the preceding one. The rules continue to be applied repeatedly to create further generations.

## Development

1. Implement Conway's Game of Life in C++.
1. Improve the implementation in runtime speed.
1. Implement multi-processing using the OpenMP framework.
1. Implement GPU-processing using the OpenCL framework.
