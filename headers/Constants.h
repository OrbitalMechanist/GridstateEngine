#pragma once

//Number of lights in the scene - some may be turned off, but
//there can never be more than this.
//You can try moving them around to fake it, though.
#define NUM_LIGHTS 8
//Maximum number of bones in a skeleton. Anything that doesn't fit will be cut off.
//Must match what's defined in the shaders, and this has to be done by hand.
#define MAX_BONES 256

//Pixel width of the starting window. Current window size may be different!
#define WINDOW_WIDTH 800
//Pixel height of the starting window. Current window size may be different!
#define WINDOW_HEIGHT 600

