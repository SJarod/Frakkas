/!\ ---- /!\  
This repository is a "wiped" version of the original school-intern GitLab repository. It excludes the assets used by Frakkarena, whiches can be found on https://sjarod.itch.io/frakkarena.  
The assimp submodule is meant to be an assimp fork, it is replaced by the real branch in this repository.  
/!\ ---- /!\


All rights reserved for Frakkas team.  
Florian Marcellot  
Manon Méhalin  
Jarod Sengkeo  

# FRAKKAS ENGINE => 'FRAKKARENA'  

## Summary
1. [Frakkas](#frakkas)  
    1.1 [Description](#description)  
    1.2 [Open in an IDE](#open-in-an-ide)  
    1.3 [Update submodules](#update-submodules)  
2. [Frakkarena](#frakkarena)  
    2.1 [Controls](#controls)

<br>

# Frakkas  
  
## Description
---

The project's goal is to produce a game from an adapted homemade game engine. The project lasted 4 months, and we are a group of 3 second-year Game Programmer students, supervised by a fifth-year student.  
  
Our engine is called **_Frakkas_**, and our game **_Frakkarena_**. The game is a Top-Down Survival Beat'em up in a fantasy world, with a toon aspect. That was cool to code this game with the engine, and we could make whatever other type of game thanks to an efficient entity/component system.  
  
In short,  **_Frakkas_** is a 3D casual game engine, with animations, shadows, and toon shading.  

<br>

## Open in an IDE
---

If you are using CLion, just open the project directly with it.  
In case you are using Visual Studio Community/Enterprise 2022, check our tutorial on our wiki: accessible [here](https://gitlabstudents.isartintra.com/projets/2021_gp_2025_engine_gp_2025_engine-frakkas/-/wikis/Launch%20project%20with%20Visual%20Studio).  

<br>

## Update submodules
---

The engine integrate several submodules.  
They are normally automaticaly update with the main CMakeLists.txt but in case you encounter problems here is three solutions:
1. In the project source directory, open a Git Bash command prompt and execute this command: `git submodule update --init --recursive`
2. In each submodule directory located in extern/ (ex: Frakkas/extern/tracy), execute this command in a Git Bash command prompt: `git checkout`
3. Contact us!

<br>

# Frakkarena

## Controls

<br>

- __Gamepad _(Xbox binding)___

Input                          | Action
-------                        | ------
Left stick / D-PAD             | Move
A / X                          | Attack
Right Trigger / B / Y          | Run
Start                          | Play / Pause  
D-PAD / A                      | Menu navigation  
    
<br>  

- __Gamepad _(PS4 binding)___

Input                                   | Action
-------                                 | ------
Left stick / D-PAD                      | Move
cross / square                          | Attack
Right Trigger / circle / triangle       | Run
Options                                 | Play / Pause  
D-PAD / cross                           | Menu navigation  

<br>  

- __Mouse / Keyboard (QWERTY)__

Input                          | Action
-------                        | ------
WASD / Arrows                  | Move
Space bar / Left click         | Attack
E / Left Shift / Right click   | Run
P / Esc                        | Play / Pause  
Arrows / Mouse / Space         | Menu navigation  

<br>

---

Looking for additionnal informations? Look at our [wiki](https://gitlabstudents.isartintra.com/projets/2021_gp_2025_engine_gp_2025_engine-frakkas/-/wikis/home)!

<br>

[Head of page](#summary)
