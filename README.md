# named-chess

# Simple cross platform Chess game made using C++ and SDL

[Download link of the Windows version for those who are only interested in playing the game (Google Drive)](https://drive.google.com/file/d/1aefaZnWF5-7VGSXPbMOYXdQwjZLKRsmR/view?usp=sharing)

<h3>Controls</h3>

| Control           | Function                        |
| ----------------- | ------------------------------- |
| Left mouse button | Select, unselect and move piece |

<h3>Not Implemented</h3>

- Special moves: Rochade, En passant
- UI elements
- Pawns, which reached the last row, will always be changed to a King 

<h3>Sources</h3>

- I made the board myself using [GIMP](https://www.gimp.org/)  
- [Chess figures](https://opengameart.org/content/colorful-chess-pieces)  
- [Movement rules source (in German)](https://www.brettspielnetz.de/spielregeln/schach.php)

<h3>Code Metrics (as of 23.09.2019)</h3>

<pre>
C:\Users\Valentin\Dev\named-chess>cloc --exclude-dir=.vscode,bin,misc,res,README.md .
      18 text files.
      18 unique files.
       2 files ignored.

github.com/AlDanial/cloc v 1.82  T=0.50 s (36.0 files/s, 3060.0 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                              9            195             32            982
C/C++ Header                     8             50             25            223
CMake                            1              2              6             15
-------------------------------------------------------------------------------
SUM:                            18            247             63           1220
-------------------------------------------------------------------------------
</pre>

<h3>Small Code Guide</h3>

- Use standardized C++
- SDL libraries use raw pointers since it's a C library
- Every line of code is limited to 80 characters
- Extensively use comments, as long as they're necessary
- Every file should end with an empty line

<h3>Current state</h1>

As of 03.06.2019, the project isn't actively developed by myself anymore. The code just seems too nested for my taste. I gladly accept good contributions, if some are made. I might make some contributions if I have an idea. For example, this project just switched to CMake!

Von Valentin Pickel
