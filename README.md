# JA2-Stracciatella Cooperative Multiplayer

This is the fork of venerable JA2-Stracciatella project introducing cooperative multiplayer on top of it.

The goal of the project is to enable the original JA2 campaign walkthrough with multiple players.

At the moment I am developing this alone. To get in touch with me you can use the [mail](mailto:mmamontx@gmail.com).

## How to start the game

1. Install original Jagged Alliance 2 game on your computer.  Data files from the original game will be used by JA2-Stracciatella.

2. [Download JA2-Stracciatella](http://ja2-stracciatella.github.io/download/) or [compile](COMPILATION.md) it from the cloned git repository. Follow instructions to point JA2S to the directory where the original game was installed during step 1.

3. Download and run the latest release of this project.

## Development

All the original JA2S [ideas](https://github.com/ja2-stracciatella/ja2-stracciatella/blob/master/CONTRIBUTING.md) remain valid. The scope of this particular project is limited to cooperative multiplayer on top of JA2-Stracciatella - all the rest goes to the original repository. The changes leverage JA2S design and are implemented on top of its ideas. They strive to be lean by continuously reviewing and progressively minimizing the code. Note that the multiplayer specifics dictated the following changes to the original gameplay:

1. The intro videos at the beginning of the game are disabled.
2. The starting screen is the strategic map (lobby) instead of the laptop.
3. There is an additional table on the strategic map screen with the list of players, their respective readiness statuses, and a dedicated button to change the latter.
4. The original message box at the bottom left of the strategic map screen also shows multiplayer-specific system messages as well as chat messages from players (there is a text input field added just below it).
5. The helicopter drop-off animation at Omerta is disabled.
6. In the turn-based mode all the players share a single turn - they are not separated like it is done in 1.13. This complies with the original game design by keeping the ability for players to manage sequence of their collaborative actions. The 'end turn' button marks the individual player turn, once all the players hit this button, the turn is passed to the enemy and other NPC teams.

## License

All the changes of this particular project are released to the public domain. For the rest, please refer to JA2-Stracciatella [license](https://github.com/ja2-stracciatella/ja2-stracciatella?tab=readme-ov-file#license).
