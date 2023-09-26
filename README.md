# Gridstate Engine #
### COMP 8551 ###

This will, eventually, be an engine for turn-based tactics games and/or CRPGs with
turn based combat.

For now, however, it is just the rendering backend (we will hopefully hook up actual
gameplay to it at some point.)

Currently only works on Windows due to certain libraries being DLLs, might be able to
change that later though.

## Frameworks/Libraries Used ##
- OpenGL
- GLFW
- glad
- glm
- assimp
- stb_image
- OpenAL
- libsndfile

## Group Composition
- Max
- Joe
- Anthony
- Shendao
- Daniil
- Gurjot
- Mike
- Calvin
- Winston
- Harley
- Joshua

## Architecture

This will eventually be mostly an ECS, but generally speaking any gameplay systems are yet to be implemented.
So far a lot of the work has been around the presentation backbone of graphics and audio.

### Universe
The "Universe" system is planned to serve as a gateway that would essentially take orders on what to render
from various subsystems like the game logic, game object list and/or the conceptualized but unlikely to
be implemented continious-space physics subsystem. It will also translate various positioning information 
from the discrete grid used by game logic to continious 3d space coordinates used by the graphics.

### Graphics
Due to the nature of this project, the graphics system is designed to be as easy to use and as hard to
accidentally break as possible. To this end, everything is built around a Renderer god-class that handles
practically all interaction with the OpenGL backend. This makes the process of what actually goes on when
working with OpenGL a lot easier to follow if you don't already know what it's *supposed* to do.

There are also classes for various logical groups that describe certain things, such as the way a mesh is 
stored in OpenGL memory. They serve as datastores and are populated by the Renderer, which creates the 
relevant OpenGL objects and feeds them to. As an added bonus, if we end up moving away from the god-class 
architecture, they will provide a good scaffolding to build the self-loading classes.

The string-map system serves the same purpose: it's easier to work with once finished.
The performance suffers but not *that* badly, and assigning easy human-readable names to things makes life
a lot easier for whoever will end up designing the game itself. There's a good chance we change the Renderer
to render based on references instead once we have gameobjects that live for more than one frame at a time.
In that case, the Mesh ECS component will look up a reference when it's assigned a loaded model instead of
the renderer doing it every frame.

Generally speaking, processing efficiency is prioritized over memory efficiency when it comes to graphics,
especially since most work machines only have integrated graphics.
