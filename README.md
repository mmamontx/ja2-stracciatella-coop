# JA2-Stracciatella Multiplayer

This project is a fork of JA2 Stracciatella introducing multiplayer to Jagged Alliance 2. It is based on JA2 Stracciatella project and leverages JA2 1.13 multiplayer experience. However, here the approach is different from 1.13 MP: rather than introducing a separate multiplayer game mode - rewritten and limited to individual skirmishes - this particular implementation directly alters singleplayer campaign (code) to make it support cooperative play.

## Vision

Basically, JA2 Stracciatella multiplayer is the same thing as the singleplayer, but enabling other players to observe everything from their PCs and use their own controls. It is just as if the players would play hotseat (like some of us did it as kids when we took turns to tell "our" merc what to do), but with 2+ PCs instead of a single one. Considering this the changes are not expected to be too complex and mostly would consist of minor alterations caused by the fact that the original code is not designed for multiplayer. At the same time the code is rather "immersive", which makes it relatively easy to be adapted for handling MP logic. The cornerstone of this is that somehow all the game status has to be synchronized between the players. JA2 1.13 handles this rather plainly by a number of callbacks that manually reflect parameters of individual objects to other players when various game events happen. Since the code is big, as the game session moves forward, the inability to track down everything causes discrepancies between what players observe and, eventually, crashes. Moreover, this approach would probably require too much "manual" effort to enable the campaign. Here, RakNet object replication mechanism is used to address this problem and handle it in a more natural fashion. The implementation approach is straightforward: start the regular game from the beginning and introduce multiplayer functionality where it is naturally expected. Fix crashes caused by changes; if necessary, add supporting code and structures; and move forward step by step.

## Manifesto

- Introduce as little custom code as possible.
- Stick to the original JA2 (JA2 Stracciatella) code. Leverage the original design and implement on top of its ideas.
- All the new weapons and extra features of 1.13 and other modifications on top of the vanilla JA2 (JA2 Stracciatella vanilla) are outside of the scope of this project.
- The axioms of style from the original CONTRIBUTING.md of JA2S mostly remain valid (except for the rule about dead code - some of the original lines are commented out and remain for reference and convenient roll back). Assimilate with coding styles that surround introduced changes. Don't try to unify or refactor unless it's really necessary - it is outside of the scope as well.

## Roadmap

1. Multiplayer on the strategic screen. So that players can see squad members and chat messages of each other. **Basic functionality - for the game beginning before the helicopter drop in Omerta - is done.**
2. Multiplayer on the tactical screen:
    1. In the real-time mode. So that players can observe actions of each other and interact. **The object replication concept works well. Now offloading all the possible client (merc) actions to the server side so it can then be broadcasted to everyone - mostly done.**
    2. In the battle/turn-based mode.
3. Moving to another sector:
    1. From the tactical screen.
    2. From the strategic screen.
4. Saving and loading.
5. Alpha demo - from the helicopter in Omerta to Drassen capture.
6. All the rest.

## Backlog

- Bugs:
    - The rectangle move cursor when it is hovered on the selected merc is shown on top instead of behind the soldier (for the client).
    - Some the original squares where mercs occur become unavailable for moving to (for the client).
    - Fix the bug when client mercs do not arrive in time and become inaccessible.
    - Fix "burst fatality" crash.
    - Sometimes a just connected client doesn't see himself in the player list until he clicks 'ready'.
    - Still, animation surfaces are not loaded for the client (it doesn't cause any crash).
- Regular priority:
    - Implement the following RPC actions from the client:
        - Opening doors.
        - Picking up items (after ground items are replicated - see below).
        - Interacting with chests and taking/placing items there.
        - Healing.
        - Unloading ammo.
    - Replicate ground items - from the server to client and from the client to server (replicate LEVELNODEs with pItemPool?).
    - Multi-selection actions.
    - Propagate time (including time compressions) from the server to clients.
    - Hiring by clients.
- Low priority:
    - Implement the following RPC actions from the client:
        - Throwing grenades.
        - Placing charges.
        - Talking (so that the effects of talking propagate to the server).
        - Cutting fences.
        - Attaching items.
        - Merging items (should work - verify).
        - Shopping.
        - Etc.
    - Keep players in separate squads. Let players create squads, but first ensure that they aren't already used by others. Prevent selecting and controlling mercs from squads of other players.
    - Verify repetitive starts and connections/disconnections within a single run.
    - Testing: manual and automated. Enable GitHub CI.
    - Remove RakNet from source code and use it as a binary.
    - Handle connections/disconnections after the game gets started (after the first time compression button click).
    - Add binary release(s).
    - Pass merc top left corner speech to the client.
    - Investigate how come ST::string direct assignment (see Soldier_Control.h) causes crashes at random locations (buffer overflow?).
    - Check if the same RPC call from multiple clients gets executed in parallel. If so, find a way to disable it, or implement respective item pointers on the server side for multiple clients: replace gpItemPointerRPC.
    - Ensure that the buttons selected in the game init options screen won't affect client experience - obtain and use these values from the server.
    - Adapt for all resolutions (including the widescreens).
    - Verify correctness of the IP address stored in the corresponding field of the game init option screen - and don't let the player to the strategic map screen until it is fixed.
    - Prevent players from having identical names (don't allow to connect or modify the name).
- For fun (remove it from the release or make it optional):
    - It seems that originally developers considered enabling jumps over the windows and left the corresponding code in place. Try to extend climbing with this ability.
    - Remove the lines that disable women enemies below elite level (see Soldier_Create.cc).

## Compiling

1. Get the original JA2 and install it. I would suggest to also install the latest patch, however probably this is not required (it is only the data files that JA2 Stracciatella uses - not sure if they are affected by the patches).
2. Get the custom images and put them into the original game directory (see the details below).
3. So far there are no releases, so one has to build it on his own to generate the binaries. Clone this repo and build it according to the original JA2S COMPILATION.md file (I use Visual Studio).
4. First run the original JA2 Stacciatella Laucher to setup the directories pointing to the original game (it somehow creates a local config or setups environment that would also be automatically used by "our" JA2S).
5. Now you can run your binary directly. I would suggest using windowed mode so you can observe and handle the server and the client at the same time.

## Etc.

Things worth mentioning and "dark knowledge":

- Custom images to be put inside Data/Interface of the base JA2 directory:
    - The modification requires images of buttons from 1.13 MP. They are located under the following path in 1.13: https://github.com/1dot13/gamedir/blob/master/Data/Interface/MPGOLDPIECEBUTTONS.sti
    - It also requires a custom image for the left panel on the strategic screen. For now it can be obtained from here: https://drive.google.com/file/d/1o886bf7AbPi2BnRnvrCQnACXLWaQl19d
- So far didn't verify or adapt for resolutions other than 640x480 - recommend using it for now. Changing it would probably affect the game init options, and strategic map screens.

Changes compared to the vanilla game (from the player perspective):

- Starting screen is the strategic map (lobby) instead of the laptop.
- The intro videos at the beginning of the game are disabled.
- Helicopter drop-off animation is disabled (see the FIXME in Merc_Entering.cc) - it accelerates debugging, and it's overall questionable whether for multiplayer it makes sense to keep it enabled.
- The additional table on the strategic map (re-consider later).

For debugging purposes:

- There is a custom function - HireRandomMercs() - that is automatically called in the beginning (accelerates debugging so one doesn't have to always visit AIM). This code is only executed for the server, the client receives replicated mercs.
- The enemies are disabled (see gEnemyEnabled var) - until implementation of the battle/turn-based mode is started.

Contact: mmamontx@gmail.com
