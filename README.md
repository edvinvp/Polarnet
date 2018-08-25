# Polarnet implementation as described in "The (new) Turing Omnibus"
Polarnet converts from polar coordinates to cartesian. A graph is produced after the training is complete showing the the error rate for each training iteration.

## Screens: X = Iteration, Y = Error size

5000 iterations, 25 neurons per layer, 0.01 learning rate
![](https://i.imgur.com/tELxFko.png "")

400 iterations, 40 neurons per layer, 0.1 learning rate
![](https://i.imgur.com/fuTxbkn.png "")


## USAGE
Requires the GNUPLOT_I C interface library available at
[https://people.sc.fsu.edu/~jburkardt/c_src/gnuplot_i/gnuplot_i.html](https://people.sc.fsu.edu/~jburkardt/c_src/gnuplot_i/gnuplot_i.html)

compile with
gcc -W polarnet gnuplot_i.c -o polar -lm
optional define arguments are
-D LEARNINGITERATIONS=<uint>, default 2500
-D RATE=<Double < 1>, default 0.01
-D MEDIALNEURONS=<uint>, defaul 25