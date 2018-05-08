# Execução dos programas

## Para executar:

`gcc -fopenmp -c main.c`<br>
`gcc -fopenmp -c nqueens.c`<br>
`gcc -fopenmp -c nqueens_1.c`<br>
`gcc -fopenmp -c nqueens_2.c`<br><br>

`gcc -fopenmp main.o nqueens.o -o queensmain`<br>
`gcc -fopenmp main.o nqueens_1.o -o queensmain_1`<br>
`gcc -fopenmp main.o nqueens_2.o -o queensmain_2`<br>

`./nqueens size`<br>
`./nqueens_1 size nthreads`<br>
`./nqueens_2 size nthreads`<br>

