#version 330 core

in vec3 ourColor;

out vec4 FragColor;  // Output color

vec3 gray_colors[7] = vec3[7](
    vec3(0.2, 0.2, 0.2),
    vec3(0.3, 0.3, 0.3),
    vec3(0.4, 0.4, 0.4),
    vec3(0.5, 0.5, 0.5),
    vec3(0.6, 0.6, 0.6),
    vec3(0.7, 0.7, 0.7),
    vec3(0.8, 0.8, 0.8)
);

vec3 rgb[3] = vec3[3](
    vec3(1.0, 0.0, 0.0),  // red
    vec3(0.0, 1.0, 0.0),  // green
    vec3(0.0, 0.0, 1.0)  // blue
);

vec3 colors[6] = vec3[6](
    vec3(1.0, 0.0, 0.0),  // red
    vec3(1.0, 1.0, 0.0),  // yellow
    vec3(0.0, 1.0, 0.0),  // green
    vec3(0.0, 1.0, 1.0),  // cyan
    vec3(0.0, 0.0, 1.0),  // blue
    vec3(1.0, 0.0, 1.0)   // magenta
);

vec3 more_colors[12] = vec3[12](
    vec3(1.0, 0.0, 0.0),  // red
    vec3(1.0, 0.5, 0.0),  // orange (between red and yellow)
    vec3(1.0, 1.0, 0.0),  // yellow
    vec3(0.5, 1.0, 0.0),  // yellow-green (between yellow and green)
    vec3(0.0, 1.0, 0.0),  // green
    vec3(0.0, 1.0, 0.5),  // turquoise (between green and cyan)
    vec3(0.0, 1.0, 1.0),  // cyan
    vec3(0.0, 0.5, 1.0),  // sky blue (between cyan and blue)
    vec3(0.0, 0.0, 1.0),  // blue
    vec3(0.5, 0.0, 1.0),  // purple (between blue and magenta)
    vec3(1.0, 0.0, 1.0),  // magenta
    vec3(1.0, 0.0, 0.5)   // pink (between magenta and red)
);

vec3 even_more_colors[24] = vec3[24](
    vec3(1.0, 0.0, 0.0),  // red
    vec3(1.0, 0.25, 0.0),  // between red and orange
    vec3(1.0, 0.5, 0.0),  // orange
    vec3(1.0, 0.75, 0.0),  // between orange and yellow
    vec3(1.0, 1.0, 0.0),  // yellow
    vec3(0.75, 1.0, 0.0),  // between yellow and yellow-green
    vec3(0.5, 1.0, 0.0),  // yellow-green
    vec3(0.25, 1.0, 0.0),  // between yellow-green and green
    vec3(0.0, 1.0, 0.0),  // green
    vec3(0.0, 1.0, 0.25),  // between green and turquoise
    vec3(0.0, 1.0, 0.5),  // turquoise
    vec3(0.0, 1.0, 0.75),  // between turquoise and cyan
    vec3(0.0, 1.0, 1.0),  // cyan
    vec3(0.0, 0.75, 1.0),  // between cyan and sky blue
    vec3(0.0, 0.5, 1.0),  // sky blue
    vec3(0.0, 0.25, 1.0),  // between sky blue and blue
    vec3(0.0, 0.0, 1.0),  // blue
    vec3(0.25, 0.0, 1.0),  // between blue and purple
    vec3(0.5, 0.0, 1.0),  // purple
    vec3(0.75, 0.0, 1.0),  // between purple and magenta
    vec3(1.0, 0.0, 1.0),  // magenta
    vec3(1.0, 0.0, 0.75),  // between magenta and pink
    vec3(1.0, 0.0, 0.5),   // pink (between magenta and red)
    vec3(0.75, 0.0, 0.25)  // between pink and red (new color added)
);

void main() {
    // Use the Primitive ID to alternate colors
    int modid = gl_PrimitiveID % 24;
    FragColor = vec4(even_more_colors[modid], 1.0);
}
