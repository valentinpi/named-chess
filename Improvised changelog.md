<h3>Improvised changelog</h3>

- 02.11.2018: Several fixes and massive progress towards starting the AI development. The code seems very complicated though. Player.cpp now counts 541 lines and the actual game logic seems to be spread all over the place. The winning condition isn't implemented properly yet.
- 03.11.2018: Reorganization of code, massive code reduction due to new Player::check_position function. Started playing with AI algorithms.
- 05.11.2018: Massive code simplification.
- 06.-12.11.2018: Code rework, decided to use the minimax algorithm for the AI logic (didn't start implementing yet).
- 12.11.2018: (in German) Das Spiel wird nun primär für meine 5. Pk im Abitur entwickelt. Dabei untersuche ich, ob es sich bei Spielcomputern um wirkliche Intelligenzen handelt, oder ob sie lediglich durch ihre größeren, rechnerischen Kapazitäten in der Lage sind, Menschen im Schach zu besiegen. Das Spiel dient unter anderem dazu, Statistiken darüber zu sammeln, wann der Durchschnittsspieler vom Durchschnittsschachcomputer geschlagen wird.
- 15.11.2018: TODO: Move the movement logic from the Player class into the Piece class. Also, add a property describing the current status to the FieldPosition type, maybe rename it to Tile.
- 24.11.2018: Decided to remove the Rochade and En passant code because it's too buggy. Started implementing the AI.
- 25.11.2018-29.11.2018: Implemented basic AI, a statistics system and made a seperate statistics viewer.
- 27.12.2018: After a long break, I got back into the game and started by reorganising.  
- 25.01.2019: Reorganisation done, still some problems in the structure of the game.  
- 07.02.2019: Continued working on the AI algorithm and improving replayability, preparing for gathering statistics.
- 09.02.2019: Implemented brute force AI. Very slow gameplay and ignores maximizer-minimizer principle. Again, some code improvements.
- 22.02.2019: In the past few days I implemented a Minimax algorithm with Alpha-Beta-Pruning. It's still slow and doesn't perform very well, so the optimization continues.
- 02.03.2019: I'm still not quite satisfied with the result, but development is finished for now. However, I'll temporarily change some code for the presentation.
- 30.04.2019: Starting to clean up code and to implement a multiplayer mode using SDL. This is the last entry in the improvised changelog.
