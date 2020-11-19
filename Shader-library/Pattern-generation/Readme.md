# Pattern-generation 

This shader illustrates the basics of pattern generation by multiplying <br>
the draw space from (0.0,0.0), (1.0,1.0) to (n,n), (n,n) to create n^2 repetitions of<br>
the drawn pattern. 

As an added bonus, I have created an efficent funtion (without if's, for faster execution)<br>
to detect the fragCoord postition in the scaled 2D space, by localising the coord in a square of<br>
user defined dimensions.
