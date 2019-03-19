set size square 1,1     				#set the aspect ratio to 1:1
set object circle at 300,300 size 200  	#draw 3 concentric circle centered on 160,160
set object circle at 300,300 size 100 
set object circle at 300,300 size 50
set label "1 m" at 300,230 boxed		#label the 3 circles
set label "2 m" at 300,180 boxed		#find out the actual distance
set label "4 m" at 300,80 boxed
set pointsize 1.5
plot [0:600] [0:600] "lidar_reading.dat" using 1:2 with points pointtype 4
     "<echo '300 300'" pt 9 linecolor rgb "black"