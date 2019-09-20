# named-chess

# Simple cross platform Chess game made using C++ and SDL

<h1>Announcement</h1>

As of 03.06.2019, the project isn't actively developed by myself anymore. The code just seems too nested for my taste. I gladly accept good contributions, if some are made. I might make some contributions if I have an idea.

The current makefile only supports building on Windows using MinGW-w64 (the DLLs in the bin directory are from the i686-8.1.0-release-posix-dwarf-rt_v6-rev0 build). There's a Linux makefile too, which can be used by passing it to make using the -f flag.

[Download link of the Windows version for those who are only interested in playing the game (Google Drive)](https://drive.google.com/file/d/19n1aBIcd6uEgoYQyR0xEYKLgi-3Jqoip/view?usp=sharing)

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

<h3>Code Metrics</h3>

<pre>
C:\Users\Valentin\Dev\named-chess>cloc --exclude-dir=.vscode,bin,misc,obj,res,.gitignore,README.md .
      21 text files.
      21 unique files.
       5 files ignored.

github.com/AlDanial/cloc v 1.82  T=0.50 s (40.0 files/s, 3374.0 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                             10            211             34           1104
C/C++ Header                     8             50             25            225
make                             2             10              8             20
-------------------------------------------------------------------------------
SUM:                            20            271             67           1349
-------------------------------------------------------------------------------
</pre>

<h3>Small Code Guide</h3>

- Use standardized C++
- SDL libraries use raw pointers since it's a C library
- Every line of code is limited to 80 characters
- Extensively use comments, as long as they're necessary
- Every file should end with an empty line

Von Valentin Pickel
