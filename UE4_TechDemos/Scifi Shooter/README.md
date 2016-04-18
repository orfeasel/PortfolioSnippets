# Scifi Shooter [Platformer 3rd Person Shooter]
# Unreal Engine 4.10.2 ( C++ )

Scifi Shooter is a 3rd Person Platformer Shooter with roguelike elements. Your goal is to advance through the level and kill its Boss. You should upgrade your weapons by killing various minions first in order to face your ultimate challenge! Don’t forget to scout the level in order to unlock new weapons and get more ammo from various resources.

This project was solo developed, thus I created all the mechanics and game systems. Scifi Shooter contains the following features, all in C++:

- Animation Handling for all characters
- Destructible Environment
- A robust weapon system. The weapon system is created in order to be flexible and easy to extend. You can add a new weapon type by typing one line of code! 
- Three different weapons (knife, rifle, shotgun)
- Data Tables for each weapon. These tables are consumed by the weapon system. In order to balance the game you just tweak a few values inside the editor!
- A Combo system for melee weapons
- Ammo Pickups for every weapon (provide ammo for the unlocked weapons so far)
- Health Pickups (provide health)
- Weapon Pickups (unlock new weapons)
- A Battlecry function for the player – A Cage will get spawned around the player and all the projectiles from the range weapons have a bounce effect.
- Jump Nodes – the player can be attached to these nodes in order to fly from one area to another where he is unable to reach using a single jump
- AI minions with the following features:
- Patrol Mode. They patrol from node to node until they are notified by a custom trigger to chase down the player
- Hunting Mode. They chase the player until one of them is dead
- Custom Triggers which can be placed in the map and notify the game for things like hunting mode, enabling the boss fight etc
- A flexible Boss fight with intermission phases, custom pickups and AI spawns:
- You can specify the intermission phases, when they occur from within the Editor.
- You can specify where and what will get spawned during each intermission phase
