uniform float tm;

varying vec3 v; //Вектор наблюдателя
varying vec3 l; //Вектор на свет
varying vec2 pp;

const float sk=0.1;



void main()
{
    vec3 n;
    n.x=cos((pp.x+tm)*sk)*sk*sin((pp.y+tm)*sk);
    n.y=sin((pp.x+tm)*sk)*sk*cos((pp.y+tm)*sk);
    n.z=-1.0;
    
    n=normalize(-n);
    n=gl_NormalMatrix*n;
    
    
    vec3 L=normalize(l);
    vec3 V=normalize(v);
    
    float dc=max(dot(n,L),0.0);
    vec3 r=reflect(-l,n);    
    float spec=pow(max(dot(r,V),0.0),32.0);
  
    vec4 color=vec4(fract(tm*0.001),fract(tm*0.002),abs(1.0-fract(tm*0.001)-fract(tm*0.002)),1.0);
    
    gl_FragColor=dc*color+vec4(spec);
}
