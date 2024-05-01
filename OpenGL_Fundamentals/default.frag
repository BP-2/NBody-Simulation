#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;
out vec4 FragColor;  
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; // per fragment world position

uniform sampler2D texture1; // giving access to the texture object
uniform sampler2D texture2;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform bool isLight;
uniform vec3 viewPos;
uniform bool pass;
uniform vec3 invisColor;
uniform vec3 cursorColor;
uniform bool isCursor;
uniform vec3 orbColor;


  
void main()
{
    float ambientStrength = .1;
    // vec3 ambient = ambientStrength * lightColor;

    // ambient
    // vec3 ambient = lightColor * material.ambient * ambientStrength;
    vec3 ambient = orbColor * ambientStrength;
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  // this finds the direction between light source and fragment position
    float diff = max(dot(norm, lightDir), 0.0);
    diff *= diff; // I added this for more dramatc shadows

    //vec3 diffuse = lightColor * (diff * material.diffuse);
    vec3 diffuse = lightColor * (diff * orbColor);

    // specular
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = lightColor * (spec * material.specular); 
    
    //vec3 result = (ambient + diffuse + specular);
    vec3 result = (ambient + diffuse);
    if(isLight){
        //FragColor = texture(texture1, TexCoord);
        result = vec3(1.0f, 1.0f, 1.0f);
        FragColor = vec4(result, 0.0f);
    }
    else if(pass){
        FragColor = vec4(invisColor, 1.0f);
    }
    else if(isCursor){
        FragColor = vec4(cursorColor, 1.0f);
    }
    else{
        // result = vec3(1.0f, 1.0f, 1.0f);
        FragColor = vec4(result, 0.0f);
        // vec4(result, 1.0f) * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0);
         //FragColor = vec4(result, 1.0f);
    }
}