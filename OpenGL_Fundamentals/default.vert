#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec3 aColor; // the color variable has attribute position 1
layout (location = 1) in vec2 aTexCoord; // the texture coordinates
layout (location = 3) in vec3 aNormal; // the normal
layout (location = 4) in vec2 bPos; // for the cursor
  
out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord; // sending Texture coordinates to fragment shader
out vec3 Normal;
out vec3 FragPos; // the lighting calculations must happen in world space


uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isCursor;

void main()
{
    if(isCursor){
        gl_Position = mat4(1.0f) * vec4(bPos, 0.0, 1.0);
    }
    else{
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor; // set ourColor to the input color we got from the vertex data
        TexCoord = aTexCoord;
        Normal = mat3(transpose(inverse(model))) * aNormal;  
        FragPos = vec3(model * vec4(aPos, 1.0)); // multiplying the vertex position attribute with the model matrix to transform it to world space
    }

}      