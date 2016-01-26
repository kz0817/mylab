#set key bmargin left horizontal Right noreverse enhanced autotitle box lt black linewidth 1.000 dashtype solid
#set title "Weights"
#set title  font ",20" norotate
#x = 0.0
plot [-120:120] 'weigth-move1.dat' using 2:1 with linespoints, 'weigth-move2.dat' using 2:1 with linespoints, 'weigth-move3.dat' using 2:1 with linespoints, 'weigth-move4.dat' using 2:1 with linespoints, 'weigth-move5.dat' using 2:1 with linespoints
