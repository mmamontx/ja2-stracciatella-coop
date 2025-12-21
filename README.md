# JA2-Stracciatella Cooperative Multiplayer

The goal of the project is to enable the original JA2 campaign walkthrough with multiple players.

## How to start the game

1. Install original Jagged Alliance 2 game on your computer.  Data files from the original game will be used by JA2-Stracciatella.

2. [Download JA2-Stracciatella](http://ja2-stracciatella.github.io/download/) or [compile](COMPILATION.md) it from the cloned git repository. Follow instructions to point JA2S to the directory where the original game was installed during step 1.

3. Download the latest release of this project. Move sti files to Data/Interface subdirectory of the original game. Run ja2.exe.

## Changes to the original gameplay

The following alterations are dictated by the multiplayer specifics and convenience. If possible, their number should be minimized. Some of the items below remain to be subjects for reconsideration.

1. The intro videos at the beginning of the game are disabled.
2. The starting screen is the strategic map (lobby) instead of the laptop.
3. There is an additional table on the strategic map screen with the list of players, their respective readiness statuses, and a dedicated button to change the latter.
4. The original message box at the bottom left of the strategic map screen also shows multiplayer-specific system messages as well as chat messages from players (there is a text input field added just below it).
5. The helicopter drop-off animation at Omerta is disabled.

It also worth mentioning a change compared to 1.13 experience:

In the turn-based mode all the players share a single turn - the turns are not separated among them. This complies with the original game design by keeping the ability for players to manage sequence of their collaborative actions. The 'end turn' button marks the individual player turn, once all the players hit this button, the turn is passed to the enemy and other NPC teams.

## License

All the changes of this particular project are released to the public domain. It uses and ships with [RakNet](https://github.com/facebookarchive/RakNet) (the license can be found under the link), and a button [image](https://github.com/1dot13/gamedir/blob/master/Data/Interface/MPGOLDPIECEBUTTONS.sti) from 1.13. For the rest, please refer to JA2-Stracciatella [license](https://github.com/ja2-stracciatella/ja2-stracciatella?tab=readme-ov-file#license).
