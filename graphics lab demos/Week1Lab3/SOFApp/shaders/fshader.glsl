#version 330

in vec3 pixelPos;
in vec3 pixelNorm;
in vec2 pixelUV;
in vec3 pixelTangent;
in vec4 fragColor;

out vec4 finalColor;
uniform vec3 lightK; // intensities for ambient, diffuse, specular
uniform float alpha; // specular power
uniform vec3 lightPos; // light position, world space
uniform vec3 cameraPos; // camera position, world space
uniform vec4 ambientColor;
uniform vec4 lightColor; // assume diffuse and specular the same
uniform float attenuation; // 1/d_a**2
uniform sampler2D textureSampler; 
uniform sampler2D normalSampler;

void main()
{
	vec4 materialColor = texture(textureSampler, pixelUV); 
	vec3 l = normalize(lightPos - pixelPos); // light vector
	vec3 n = normalize(pixelNorm); // surface normal

    vec3 t = normalize(pixelTangent); // surface tangent 
	vec3 b = cross(t, n); // bitangent 

	vec4 normalColor = texture(normalSampler, pixelUV); 
	n = 2*(normalColor.r-0.5)*t + 2*(normalColor.g-0.5)*b + 2*(normalColor.b-0.5)*n; 
	n = normalize(n); 

	vec3 e = normalize(cameraPos - pixelPos); // eye vector
	vec3 r = -reflect(l,n); // reflected light vector
	float d = length(lightPos - pixelPos);
	float fAttenuation = 1.0f/(1.0f + d*d*attenuation);
	float kambient = lightK.x;
	float kdiffuse = max(dot(l,n),0) * lightK.y * fAttenuation;
	float kspecular = pow(max(dot(e,r),0.0f),alpha) * lightK.z * fAttenuation;
	finalColor = materialColor * ( lightColor*kdiffuse + ambientColor*kambient ) +
	kspecular*lightColor;
	finalColor.a = 1.0f;
}