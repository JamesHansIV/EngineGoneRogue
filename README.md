# EngineGoneRogue
Each folder is the name of a member, with their finished artifacts inside.

For sprint 1, there will be C++ SDL2 code that should create a small window that accepts keyboard input, and a paragraph explaining what a game engine is and the components that make up a game engine. The paragraph should also describe how each component works.

Each  member should have a readme in their folder that explains how to compile and run their code.


To build the game with cmake:
```console
$ ./build-engine.sh
```

To build the editor with cmake:
```console
$ ./build-engine.sh editor
```

### Editor Keybinds
* Deselect current tool and all selected objects - **esc**
* Draw Tool - **d** (toggleable)
* Erase Tool - **e** (toggleable)
* Multi-tile select Tool - **t** (toggleable)
* Paint Bucket Tool - **p**
* Move Selection Tool - **m** (toggleable) or click and drag (a) selected tile(s) whith no Tools active
* Undo - **cmd + z**
* Redo - **cmd + shift + z**

### Attributions
<a href="https://www.flaticon.com/free-icons/edit" title="edit icons">Edit icons created by Freepik - Flaticon</a>