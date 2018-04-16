/* RGBA color attribute */
varying vec4 color;

void main() {
    /* Sets the fragment color to the color attribute */
    gl_FragColor = color;
}