# EngineGoneRogue
Each folder is the name of a member, with their finished artifacts inside.

For sprint 1, there will be C++ SDL2 code that should create a small window that accepts keyboard input, and a paragraph explaining what a game engine is and the components that make up a game engine. The paragraph should also describe how each component works.

Each  member should have a readme in their folder that explains how to compile and run their code.


To build the game with cmake:
1) % cmake -B (buildDir)
2) % cd (buildDir)
3) % make all
4) % ./Engine

To build the editor with cmake:
1) % cmake -DEDITOR=ON -B (buildDir)
2) % cd (buildDir)
3) % make all
4) % ./Engine