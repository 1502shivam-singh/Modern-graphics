#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;
float circle(in vec2 _st, in float _radius){
    vec2 l = _st-vec2(0.5);
    return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(l,l)*4.0);
}

/* This function tells you whether a fragCoord passed in, is in the square from (1.0,1.0) (left bottom) and (2.0,2.0) (right top)*/
float inbetween(in vec2 _st){
    float y = step(1.0,_st.x)*step(-2.0,-_st.x);
    y *= step(1.0,_st.y)*step(-2.0,-_st.y);
    return y;
}

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution;
    vec3 color = vec3(0.0);
    st *= 3.0;      // Scale up the space by 3
    
    float change = inbetween(st);
    
    st = fract(st); // Wrap around 1.0
    // Now we have 9 spaces that go from 0-1

    color = vec3(st,0.0);
    color = vec3(circle(st,0.5-change*sin(min(u_mouse.x,u_mouse.y))/2.0));

	gl_FragColor = vec4(color,1.0);
}
