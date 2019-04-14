set size square 1,1
set arrow nohead from 300,292.5 to 297,300   	
set arrow nohead from 300,292.5 to 303,300   			#set the aspect ratio to 1:1
#set object circle at 300,300 size 100 
#set object circle at 300,300 size 50
set label "1 m" at 300,250 boxed		#label the 3 circles
set label "2 m" at 300,200 boxed
set label "20cm by 20cm" at 340,220 boxed		#find out the actual distance
#set label "4 m" at 300,100 boxed
set pointsize 0.5

#grid code
set style line 11 lc rgb '#ffffff' lt 1
set border 0 back ls 11
set tics out nomirror scale 0,0.001
set format ''
set mxtics 4
set mytics 4
set style line 12 lc rgb '#ddccdd' lt 1 lw 3
set style line 13 lc rgb '#ddccdd' lt 1 lw 1.5
set grid xtics mxtics ytics mytics back ls 12, ls 13

#grid scale: 20cm by 20cm
plot [250:350] [250:350] "lidar_reading4.dat" using 1:2 with points pointtype 4
     #"<echo '300 300'" pt 9 linecolor rgb "black"