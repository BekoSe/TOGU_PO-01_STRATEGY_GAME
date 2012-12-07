uniform float tm;

varying vec2 pp; //Положение на плоскости в системе координат объекта
varying vec3 v; //Вектор наблюдателя
varying vec3 l; //Вектор на свет

const vec4 lpos=vec4(200.0,200.0,100.0,1.0);
const float height=4.0;
const float sk=0.1;

void main()
{   
    vec3 vp=vec3(gl_ModelViewMatrix*gl_Vertex);
    l=normalize(vec3(gl_ModelViewMatrix*lpos)-vp);
    v=normalize(-vp);
    
    pp=gl_Vertex.xy;
    
    float h=height*(sin((pp.x+tm)*sk)*sin((pp.y+tm)*sk));
    
    gl_Position=gl_ModelViewProjectionMatrix*vec4(pp.x,pp.y,h,1);         
}
