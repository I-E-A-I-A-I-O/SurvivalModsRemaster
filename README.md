# Description

As the name indicates, this mod aims to simulate the survival minigame from GTA Online in Singleplayer. Each wave the enemy's aim gets better and their armor is increased, increasing the difficulty. By default, survivals end after 10 waves, but they can also be started in infinite waves mode.

In the first 2 waves, enemies will only spawn on foot at the different spawn points. From wave 3 enemies will also spawn in vehicles, and from wave 5 enemies will use helicopters too.

Around the survival area, there are armor, health, and weapon pickups to aid the player. Pickups are regenerated after each wave.

If the player goes too far away from the survival area, the survival will be automatically canceled after 15 seconds, the player will receive a warning indicating how much time is left before this happens.

# Requirements
- Latest version of ScriptHookV.
- Microsoft Visual C++ Redistributables.


# Installation
Extract the compressed file and copy the contents to your GTA V directory

# Available survivals
- Construction site survival
- Del Perro survival
- Desert alien survival
- Groove street survival
- Junkyard survival
- Maze Bank Tower survival
- Sawmill Survival
- Industrial Zone Survival
- Halloween Survival
- Rancho Survival
- Xmas Survival
- Police Station Survival
- Cayo Perico Survival
- Cayo Perico Mansion Survival
- El Burro Heights
- Kortz Center Survival
- Meth Lab Survival
- Altruist Camp Survival
- Cargo Survival
- Vanilla Unicorn Survival


# How to start a survival
Survival starting points are marked in the map with a red skull, you just navigate there and press the assigned keys to start the survival

# Controls
### Mouse and Keyboard:

Press R to start the survival in 10 waves mode.

Press F to start the survival in infinite waves mode.

Hold F11 for 3 seconds to cancel the survival.

### Gamepad:

Press B to start the survival in 10 waves mode.

Press Y to start the survival in infinite waves mode.

Hold DPadRight for 3 seconds to cancel the survival.

#### Controls can be modified in the config.json file

# Timed mode
In this mode you start with 60 seconds, you can gain more time by killing enemies. This mode ends with either the player dying or the time running out.

In the first 20 minutes, every 2 minutes the difficulty is increased, this mean the enemies will get better weapons, increased accuracy and armor, vehicles, etc. Also, the reward is increased and the pickups are regenerated. After the first 20 minutes, only the reward is increased and the pickups regenerate, the difficulty stays the same.

### Kill rewards:
- Headshot: +5 seconds
- Torso: +2.5 seconds
- Limbs: +1 second
- Enemy vehicle: +10 seconds
- Juggernaut: +20 seconds
- Rottweiler: +8 seconds
- Jesus Christ: +15 seconds
- Rage-mode Sasquatch: +15 seconds


# Special enemies
- **Juggernaut:** This enemy is present in all the survivals, it starts spawning at wave 8 and is quite resistant to bullets and explosions. It also fires in fully automatic mode.
- **Sasquatch:** Enemy that so far is only present in the Halloween survival and starts spawning at wave 4. Normally it behaves like any other normal enemy, with the exception that it runs faster, but, it has a chance of spawning in "Rage Mode". When in Rage Mode, the sasquatch will charge towards the player to attack with melee and will be able to kill the player with one punch. It also will become slightly more resistant than a Juggernaut. The chances of spawning in Rage Mode increases every wave
- **Rottweiler:** By default it's only present in the Rancho Survival, it starts spawning at wave 7 and it's limited to 3 per wave. If it catches you, you are pretty much dead, so you better keep your distance.
- **Jesus Christ:** Only present in the Xmas Survival by default, he starts spawning at wave 7. This enemy has the same amount of health as the Juggernaut, but unlike the Juggernaut, Jesus doesn't attack the player at all, instead he runs toward dead enemies and resurrects them. There can only be one Jesus at a time in the survival, but hey may spawn again during the same wave once killed.
- **Suicide Bomber:** This enemy will start moving towards the player until they're within 2 meters from the player, then it will blow themselves up in an attempt to kill the player. If this enemy is killed with a headshot it will blow up instantly, otherwise, it will blow up after 3 seconds of dying.
