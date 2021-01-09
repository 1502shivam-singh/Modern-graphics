  precision mediump float;  

  uniform vec2 u_resolution;
  uniform float u_time;
  uniform vec2 u_mouse;

  varying vec2 vUv;

  uniform sampler2D u_texture;

  void main() {
    vec2 st = gl_FragCoord.xy/u_resolution;
    vec3 texture = texture2D(u_texture, vUv).rgb;

    // This code section related to setting the mouse position and the circle size with accordance to the aspect ratio
    st.x *= (u_resolution.x/u_resolution.y);

    vec2 mcoord = u_mouse;
    mcoord.y = 1.0 - mcoord.y;  // Because the y coordinate system is reversed in WebGL, y axis goes downward
    mcoord.x += mcoord.x * (u_resolution.x/u_resolution.y - 1.0); 

    float circle = step((1.0/16.0), distance(mcoord, st));  // Radius is 1/16th of the NDC
    
    texture = vec3(texture.r, texture.g, circle);
    
	  gl_FragColor = vec4(texture, 1.0);
  }
