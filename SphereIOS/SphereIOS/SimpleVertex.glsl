attribute vec3 Position; 
attribute vec4 SourceColor; 

varying vec4 DestinationColor; 

uniform mat4 Projection;
uniform mat4 Modelview;
uniform vec3 SurfaceCenter;

attribute vec2 TexCoordIn;
varying vec2 TexCoordOut;

void main(void) { 
    DestinationColor = SourceColor; 
    gl_Position = Projection * Modelview * vec4(Position,1);

//    TexCoordOut = TexCoordIn;
//    TexCoordOut.x = dot( (Position - SurfaceCenter), Position );
//    TexCoordOut.y = dot( (Position - SurfaceCenter), cross( Position, SurfaceCenter ) );


    vec3 O = ( Modelview * vec4(0,0,0,1) ).xyz;

//    vec3 R = SurfaceCenter;
    vec3 R = vec3( Modelview * vec4(SurfaceCenter,1) ) - O;
    vec3 N = vec3( 0, -1, 0 );
//    vec3 X = Position;
    vec3 X = vec3( Modelview * vec4(Position,1) ) - O;
    
    vec3 nR = normalize(R);
    vec3 nN = normalize(N);
    vec3 nX = normalize(X);
    
    vec3 Tr = R * dot( nR, N / length(R) );
//    vec3 Tn = N * length( cross( nR, nN ) );
    
    vec3 T = N - Tr;
    vec3 nT = normalize(T);
    
    TexCoordOut.y = 0.5 + dot( nT, normalize( X - R ) ) / 2.0;
    TexCoordOut.x = -0.5 - dot( cross( nR, nT ), normalize( X - R ) ) / 2.0;
}






