#include <iostream>
using namespace std;
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "math.h"
#include <assert.h>

float func_map(int height, int width) {
	//int x = height*width;
	float* map = new float[height*width];
	for (int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			map[width*i + j]=(float)rand()/(float)RAND_MAX;
			return (float)rand()/(float)RAND_MAX;
		}
	}
	delete [] map;
}

void function_BicubicInterpolator(int height, int width) {
	height = 10;
	width = 10;
	float* a = new float[height*width];
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			a[width*i + j] = func_map(height, width)/(height+width);
			cout << func_map(height, width)/(height+width) << endl;
			
		}
	}
}

typedef float vec4[4];




bool BicubicInterpolation(float* img, int width, int height, int nw, int nh, float* res, float* img_buf=NULL, vec4* param_buf=NULL)
{
	//Выделяем память, если необходимо
	float* ib; 
	vec4* qb;
	if (img_buf) ib=img_buf;
	else ib=new float[nw*height];
	int max_s=width;
	if (height>width) max_s=height;
	if (param_buf) qb=param_buf;
	else qb=new vec4[max_s];
	if (!ib || !qb)
	{
		if (!img_buf) delete[] ib;
		if (!param_buf) delete[] qb;
		return false;
	}
	//Необходимая память выделена, можно начать вычисления
	int e_s=width-2; //Количество уравнений в системе по x
	qb[1][0]=-0.25f;
	for (int i=2; i<e_s; i++) qb[i][0]=-0.166666666f/(0.666666666f+0.166666666f*qb[i-1][0]);
	qb[e_s][0]=0.0f;
	//Переменные для интерполяции
	float ws=(float)width/(float)nw;
	float hs=(float)height/(float)nh;
	float x;
	int k;
	float xk_x;
	float x_xk1;
	int from;
	int to;
	//Интерполируем каждую строку
	for (int i=0; i<height; i++)
	{
		int j;
		//Заполняем вектор правых частей системы
		for (j=0; j<e_s; j++) qb[j][2]=img[i*width+j+2]-2.0f*img[i*width+j+1]+img[i*width+j];
		//Находим вспомагательный вектор B (qb[j][1])
		qb[1][1]=qb[0][2]*1.5f;
		for (j=2; j<=e_s; j++) qb[j][1]=(qb[j-1][2]-0.166666666f*qb[j-1][1])/(0.666666666f+0.166666666f*qb[j-1][0]);
		//Решаем систему, находим производные в каждой точке qb[j][3]
		qb[0][3]=0.0f;
		qb[e_s+1][3]=0.0f;
		for (j=e_s; j>=1; j--) qb[j][3]=qb[j][0]*qb[j+1][3]+qb[j][1];
		//Вычисляем результат интерполяции строки, записываем его в буффер
		from=(int)(0.5/ws)+1;
		to=nw-from-1;
		for (j=from; j<=to; j++)
		{
			x=((float)j+0.5f)*ws;
			k=(int)(x+0.5f);
			xk_x=k+0.5f-x;
			x_xk1=1.0f-xk_x;
			ib[i*nw+j]=qb[k-1][3]*xk_x*xk_x*xk_x*0.166666666f+qb[k][3]*x_xk1*x_xk1*x_xk1*0.166666666f+
				(img[i*width+k-1]-qb[k-1][3]*0.166666666f)*xk_x+(img[i*width+k]-qb[k][3]*0.166666666f)*x_xk1;			
		}
		for (j=0; j<nw; j++)
		{
			if (j==from) j=to+1;
			x=((float)j+0.5f)*ws;
			k=(int)(x+0.5f);
			if (k==0) k=1;
			if (k>width-1) k=width-1;
			xk_x=k+0.5f-x;
			x_xk1=1.0f-xk_x;
			ib[i*nw+j]=qb[k-1][3]*xk_x*xk_x*xk_x*0.166666666f+qb[k][3]*x_xk1*x_xk1*x_xk1*0.166666666f+
				(img[i*width+k-1]-qb[k-1][3]*0.166666666f)*xk_x+(img[i*width+k]-qb[k][3]*0.166666666f)*x_xk1;			
		}		
	}
	//SaveImageAsJPEG("hor_inter.jpg",ib,nw,h);
	e_s=height-2; //Количество уравнений в системе по x
	qb[1][0]=-0.25f;
	for (int i=2; i<e_s; i++) qb[i][0]=-0.166666666f/(0.666666666f+0.166666666f*qb[i-1][0]);
	qb[e_s][0]=0.0f;
	//Интерполируем каждый столбец - молодец
	for (int j=0; j<nw; j++)
	{	
		int i;
		//Заполняем вектор правых частей системы
		for (i=0; i<e_s; i++) 
			qb[i][2]=ib[(i+2)*nw+j]-2.0f*ib[(i+1)*nw+j]+ib[i*nw+j];
		//Находим вспомагательный вектор B (qb[i][1])
		qb[1][1]=qb[0][2]*1.5f;
		for (i=2; i<=e_s; i++) qb[i][1]=(qb[i-1][2]-0.166666666f*qb[i-1][1])/(0.666666666f+0.166666666f*qb[i-1][0]);
		//Решаем систему, находим производные в каждой точке qb[j][3]
		qb[0][3]=0.0f;
		qb[e_s+1][3]=0.0f;
		for (i=e_s; i>=1; i--) 
			qb[i][3]=qb[i][0]*qb[i+1][3]+qb[i][1];
		//Вычисляем результат интерполяции строки, записываем его в буффер
		from=(int)(0.5/hs)+1;
		to=nh-from-1;
		for (i=from; i<=to; i++)
		{
			x=((float)i+0.5f)*hs;
			k=(int)(x+0.5f);
			xk_x=k+0.5f-x;
			x_xk1=1.0f-xk_x;
			res[i*nw+j]=qb[k-1][3]*xk_x*xk_x*xk_x*0.166666666f+qb[k][3]*x_xk1*x_xk1*x_xk1*0.166666666f+
				(ib[(k-1)*nw+j]-qb[k-1][3]*0.166666666f)*xk_x+(ib[k*nw+j]-qb[k][3]*0.166666666f)*x_xk1;			
		}
		for (i=0; i<nh; i++)
		{
			if (i==from) i=to+1;
			x=((float)i+0.5f)*hs;
			k=(int)(x+0.5f);
			if (k==0) k=1;
			if (k>height-1) k=height-1;
			xk_x=k+0.5f-x;
			x_xk1=1.0f-xk_x;
			res[i*nw+j]=qb[k-1][3]*xk_x*xk_x*xk_x*0.166666666f+qb[k][3]*x_xk1*x_xk1*x_xk1*0.166666666f+
				(ib[(k-1)*nw+j]-qb[k-1][3]*0.166666666f)*xk_x+(ib[k*nw+j]-qb[k][3]*0.166666666f)*x_xk1;			
		}		
	}
	if (!img_buf) delete[] ib;
	if (!param_buf) delete[] qb;
	return true;
}
bool BilinearInterpolation(float* img, int w, int h, int nw, int nh, float* res, float* img_buf=NULL)
{
	float* ib; 	
	if (img_buf) ib=img_buf;
	else ib=new float[nw*h];	
	if (!ib) return false;	

	float ws=(float)w/(float)nw;
	float hs=(float)h/(float)nh;

	float rx;
	int r_id;
	int n_id;
	int nid;
	float fk,ik;
	//Масштабируем по x
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<nw; j++)
		{
			rx=(j+0.5f)*ws;
			r_id=(int)rx;
			nid=r_id+0.5>rx?-1:1;
			n_id=r_id+nid;
			if (n_id<0 || n_id>w-1) n_id=r_id;
			ik=fabs(r_id+0.5f-rx);
			fk=1.0f-ik;
			ib[i*nw+j]=img[i*w+r_id]*fk+img[i*w+n_id]*ik;
		}
	}
	//Масштабируем по y
	for (int j=0; j<nw; j++)
	{
		for (int i=0; i<nh; i++)
		{
			rx=(i+0.5f)*hs;
			r_id=(int)rx;
			nid=r_id+0.5>rx?-1:1;
			n_id=r_id+nid;
			if (n_id<0 || n_id>h-1) n_id=r_id;
			ik=fabs(r_id+0.5f-rx);
			fk=1.0f-ik;
			res[i*nw+j]=ib[r_id*nw+j]*fk+ib[n_id*nw+j]*ik;
		}
	}
	if (!img_buf) delete[] ib;	
	return true;
}
float GetNormal(float mx, float sigma)
{
	float x,y,s;
	do
	{
		x=(float)rand()/(float)RAND_MAX*2.0f-1.0f;
		y=(float)rand()/(float)RAND_MAX*2.0f-1.0f;
		s=x*x+y*y;
	}
	while (s>1.0 || s<=0.0);
	float z=x*sqrtf(-2.0*log(s)/s);
	return mx+sigma*z;
}
void AddNoise(float* base_map, int bw, int bh, float from, float to, int nw, int nh)
{
	if (nw>=bw || nh>=bh) return;
	printf("%d x %d level generation...",nw,nh);
	float* noise_map=new float[nw*nh];
	float* addition=new float[bw*bh];

	for (int i=0; i<nw*nh; i++)	
		noise_map[i]=(float)rand()/(float)RAND_MAX*(to-from)+from;
	
	BicubicInterpolation(noise_map, nw, nh, bw, bh, addition);
	//BilinearInterpolation(noise_map, nw, nh, bw, bh, addition);
	
	for (int i=0; i<bw*bh; i++) 	
		base_map[i]+=addition[i];				
		
	delete[] noise_map;
	delete[] addition;
	printf("generated!\n");
}
void AddNoiseN(float* base_map, int bw, int bh, float mx, float sigma, int nw, int nh)
{
	if (nw>=bw || nh>=bh) return;
	printf("%d x %d level generation...",nw,nh);
	float* noise_map=new float[nw*nh];
	float* addition=new float[bw*bh];

	for (int i=0; i<nw*nh; i++)	
		noise_map[i]=(float)GetNormal(mx,sigma);//(float)rand()/(float)RAND_MAX*(to-from)+from;
	
	BicubicInterpolation(noise_map, nw, nh, bw, bh, addition);
	//BilinearInterpolation(noise_map, nw, nh, bw, bh, addition);
	
	for (int i=0; i<bw*bh; i++) 	
		base_map[i]+=addition[i];				
		
	delete[] noise_map;
	delete[] addition;
	printf("generated!\n");
}

float* base_map=new float[2048*2048];
float getHeight(int x, int y) {
	assert(x < 2048 && y < 2048);
	return base_map[x+y*2048];
}
#define FUNCTION(ARGA,ARGB,BODY) struct { void operator()(ARGA,ARGB) { BODY } }

struct _ {
	float eye[3];
	void update() {
		gluLookAt(eye[0], eye[1], eye[2], 0, 0,0, 0, 1, 0);
	}
	_() {
		eye[0] = -8, eye[1] = -164, eye[2] = 30;
	}
} camera;

void Draw(void){
	const int N = 2047;

	glEnable(GL_FOG);
	float FogCol[3]={0.74,0.7,1.0};
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 10.f);
	glFogf(GL_FOG_END, 2200.f);
	glFogfv(GL_FOG_COLOR,FogCol);

	glDepthRange(0.001, 100000);
	glClearColor(FogCol[0], FogCol[1], FogCol[2], 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, 1, 1.0f, 15000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-N/2,0,0);
	//gluLookAt(1,1,1,0,0,0,0,0,1);
	//glRotatef(1.5, 0.3, 0.5, 0.15);
	//gluLookAt( 4.0, 2.0, 1.0, 2.0, 4.0, -3.0, 2.0, 2.0, -1.0 );
	camera.update();
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1,0,0);
	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < N; ++j) {
			glColor3f(/*(rand()%100)/100.0f,(rand()%100)/100.0f,(rand()%100)/100.0f*/getHeight((i+230)%2048,(j+531)%2048)*0.5,getHeight(i,j),getHeight((i+641)%2048,(j+862)%2048)*0.2);
			glColor3f(getHeight(i,j)*0.5,getHeight(i,j),getHeight(i,j)*0.2);
			FUNCTION(int i, int j, glVertex3f(i,j, 100*getHeight(i,j));) point;
			point(i+1,j);
			point(i,j);
			point(i,j+1);
			//glVertex2f(i,j);
			//glVertex2f(i,j+1);
			point(i+1,j);
			point(i,j+1);
			point(i+1,j+1);

			//glVertex2f(i+1,j);
			//glVertex2f(i,j+1);
			//glVertex2f(i+1,j+1);
		}
	}
	glEnd();
	glutSwapBuffers();
	glFlush();
}

void MouseCallback(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		cout << x << "  "<<y<<endl;
		camera.eye[1] += 10;
		Draw();
	}
}

int main(int a, char* b[]) {
	glutInit(&a, b);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Terragen");
	
	glClearColor(1,1,1,1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutDisplayFunc(Draw);
	glutMouseFunc(MouseCallback);
	
	memset(base_map,0,sizeof(float)*2048*2048);


	//AddNoise(base_map,2048,2048,-1.0,1.0,10,10);
	AddNoiseN(base_map,2048,2048,0.0,3.0,10,10);
	//AddNoise(base_map,2048,2048,-0.25,0.25,40,40);
	AddNoiseN(base_map,2048,2048,0.0,0.5,25,25);
	AddNoise(base_map,2048,2048,-0.125,0.125,80,80);
	AddNoise(base_map,2048,2048,-0.0625,0.0625,160,160);
	AddNoise(base_map,2048,2048,-0.03125,0.03125,320,320);
	AddNoise(base_map,2048,2048,-0.015,0.015,640,640);
	
	float min=1E30;
	float max=-1E30;
	for (int i=0; i<2048*2048; i++)
	{
		if (base_map[i]<min) min=base_map[i];
		if (base_map[i]>max) max=base_map[i];
	}
	float kf=1.0f/(max-min);
	for (int i=0; i<2048*2048; i++)
		base_map[i]=(base_map[i]-min)*kf;

	glutMainLoop();

	//SaveImage("height_map.jpg",base_map,2048,2048);	

	delete[] base_map;
	return 0;


	




	return 0;
}