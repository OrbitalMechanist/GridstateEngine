//based heavily on https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadowFaces[6]; //matrices for each face in a cubemap around a point light

out vec4 fragPos;

void main() {
	for(int i = 0; i < 6; ++i){
		gl_Layer = i; //sets the current face of the cubemap
		for(int j = 0; j < 3; ++j){ //for each vertex in a face, place a copy of it in the correct
		//location relative to the current face of the cubemap
			fragPos = gl_in[j].gl_Position;
			gl_Position = shadowFaces[i] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}