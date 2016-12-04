# Puzzle Game [1st Person Game]
# Unreal Engine 4.13.2 ( C++ )

Puzzle Game is a 1st person game that simulates escape rooms puzzles. You have to solve a puzzle in order to escape from the space you're currently locked in. 
The goal of the programming design is to end up with a scalable puzzle game that relies on various components.

View a playable demo the game here: https://youtu.be/6PxVoMtMPmM

This project was solo developed, meaning that all the mechanics and game systems were programmed by me. 

Here is a breakdown of all the systems and features that reside in this project:

- Animation Handling for all characters
- Players movement
- An inventory system that relies on its own component (BackpackComponent - this system contains partial Blueprint code for texture binding)
- The BackpackComponent relies on the CharacterRaycastComponent that essentially informs the player if he is able (or not) to interact with the item he's currently seeing
- The FootstepComponent communicates with the animation instances in order to handle desired movement SFX
- An interactable item system that contains all the necessary information about interaction and equip status on the backpack component
- Three different puzzles:
- Puzzle #1 (Melody of Torment). The goal of this puzzle is to play the right tune (using interactable items - in the demo this is a piano) in order for a door to open. This puzzle is displayed in the uploaded demo on YouTube
- Puzzle #2 (Poetic Concerns). The goal of this puzzle is to rotate certain objects in a specific direction in order to escape the room you're currently in.
- Puzzle #3 (Room Sliding Puzzle). The goal of this puzzle is to change the position of a set of tiles, however, every tile represents a room inside the game. This means that every time you change the position of a tile, a room inside the game is changing its position too. You have to solve this puzzle in order to escape the house that you're currently in.
- Basic AI that guides the player to the next stage of the game.