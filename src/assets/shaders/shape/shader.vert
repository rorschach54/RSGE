/* MVP (Model, View, Projection) matrix uniform variable */
uniform mat4 MVP;

/* position uniform variable */
uniform vec3 position;

/* RGBA color */
attribute vec4 vCol;
varying vec4 color;

/* 3-float tupplet position */
attribute vec3 vPos;

void main() {
    /* Sets the position */
    gl_Position = vec4(vPos,1.0);
    
    /* Passes the color to the fragment shader */
    color = vCol;
}