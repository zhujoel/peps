set terminal png transparent size 640,240
set size 1.0,1.0

set terminal png transparent size 640,480
set output 'lines_of_code_by_author.png'
set key left top
set yrange [0:]
set xdata time
set timefmt "%s"
set format x "%Y-%m-%d"
set grid y
set ylabel "Lines"
set xtics rotate
set bmargin 6
plot 'lines_of_code_by_author.dat' using 1:2 title "joel zhu" w lines, 'lines_of_code_by_author.dat' using 1:3 title "Philippe Morais" w lines, 'lines_of_code_by_author.dat' using 1:4 title "Joël Zhu" w lines, 'lines_of_code_by_author.dat' using 1:5 title "khounaIdo" w lines, 'lines_of_code_by_author.dat' using 1:6 title "joel" w lines, 'lines_of_code_by_author.dat' using 1:7 title "hraimk" w lines, 'lines_of_code_by_author.dat' using 1:8 title "eloise" w lines, 'lines_of_code_by_author.dat' using 1:9 title "Eloise Vrana" w lines
