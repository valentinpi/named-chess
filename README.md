# named-chess

# Simple cross platform Chess game made using C++ and SDL

<h3>Controls</h3>

| Control           | Function                        |
| ----------------- | ------------------------------- |
| Left mouse button | Select, unselect and move piece |

<h3>Not implemented</h3>

- Special moves: Rochade, En passant
- UI elements
- Pawns, which reached the last row, will always be changed to a King 

<h3>Sources</h3>

- I made the board myself using [GIMP] (https://www.gimp.org/)  
- [Chess figures](https://opengameart.org/content/colorful-chess-pieces)  
- [Movement rules source (in German)](https://www.brettspielnetz.de/spielregeln/schach.php)

<h3>Code metrics</h3>

<pre>
D:\Dev\named-chess>cloc --exclude-dir=bin,dep,obj,res,README.md,README.html,"Improvised changelog.md","Improvised changelog.html" .
      17 text files.
      17 unique files.
       6 files ignored.

github.com/AlDanial/cloc v 1.76  T=0.50 s (30.0 files/s, 3520.0 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                              8            209             32           1225
C/C++ Header                     6             44             25            218
make                             1              1              0              6
-------------------------------------------------------------------------------
SUM:                            15            254             57           1449
-------------------------------------------------------------------------------
</pre>

<h3>Code Guide</h3>

- Use standardized C++
- SDL libraries use raw pointers since it's a C library
- Every line of code is limited to 80 characters
- Extensively use comments, as long as they're necessary
- Every file should end with an empty line

Von Valentin Pickel
