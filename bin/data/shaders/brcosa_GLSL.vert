varying vec2 texcoord;

void main (void)

{
    gl_Position = ftransform();

   gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
    texcoord    = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
}
