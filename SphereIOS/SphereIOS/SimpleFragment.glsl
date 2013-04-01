varying lowp vec4 DestinationColor;

varying lowp vec2 TexCoordOut;
uniform sampler2D Texture;

void main(void) {
    //gl_FragColor = DestinationColor * texture2D(Texture, TexCoordOut); // New
    lowp vec4 a = vec4(1,1,1,1);
    lowp vec4 b = texture2D(Texture, TexCoordOut);
    
//    gl_FragColor = vec4(
//        a.y * b.z - a.z * b.y,
//        a.z * b.x - a.x * b.z,
//        a.x * b.y - a.y * b.x,
//        1
//    );

//    gl_FragColor = vec4(
//        a.z * b.y - a.y * b.z,
//        a.x * b.z - a.z * b.x,
//        a.y * b.x - a.x * b.y,
//        1
//    );

    gl_FragColor = vec4(
        b.x * a.x,
        b.y * a.y,
        b.z * a.z,
        1
    );
}