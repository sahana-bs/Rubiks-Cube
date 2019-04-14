
#include <GL/glut.h>  
#include < GL/freeglut.h>
#include<iostream>
#include<vector>

#define FOR(i,a,b,increment) for(float i=a;i<=b;i=i+increment)
#define FORD(i,a,b,decrement) for(float i=a;i>=b;i=i-decrement)
#define REP(i,n) for(int i=0;i<n;++i)
int winw =640 , winh=640;

/*stuff to do
make simulatenous face rotations possible use quaternions or something
make it presentable
bfs
*/
typedef void(*ButtonCallback)();
void drawText(void *, const char*, float , float );
void displayTitle();

struct Mouse
{
	float x;			//the x coordinate of the mouse cursor
	float y;			//the y coordinate of the mouse cursor	
	int ButtonClicked;	//is the left button pressed?	
};
typedef struct Mouse Mouse;
Mouse mouse = {0,0,0};
GLdouble winX,winY,winZ;

void WindowCoordinates(GLdouble x, GLdouble y)
{
	
	GLint viewport[4]; //var to hold the viewport info
	GLdouble modelview[16]; //var to hold the modelview info
	GLdouble projection[16]; //var to hold the projection matrix info
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info
	gluProject(x,y,0.0,modelview,projection,viewport,&winX,&winY,&winZ);
}
class Button {
	public:
	    float x;				
		float   y;			
		float   w;
		float   h;							
		int	  state;						 
		int	  highlighted;					
		const char* label;						
	    ButtonCallback callbackFunction;

	Button(const int px, const int py, const int pw,const int ph, int pstate, int phighlighted,const char* plabel,ButtonCallback pcallbackFunction)
	{
		x = px;							//top left x coord of the button 
	    y = py;							// top left y coord of the button 
		w = pw;					        // the width of the button 
		h = ph;							// the height of the button 
		state =pstate;					// the state, 1 if pressed, 0 otherwise 
		highlighted =phighlighted;	    // is the mouse cursor over the control? 
		label = plabel;				    // the text label of the button 
		callbackFunction = pcallbackFunction;
	}

	void ButtonDraw()
	{
		float fontx=0;
		float fonty=0;
		glColor3f(0, 0.8, 0);
		//draw background for the button.
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y + h);
		glVertex2f(x + w, y + h);
		glVertex2f(x + w, y);
		glEnd();	
		//Draw an outline around the button
		ButtonPassive();
		if (!highlighted)
		{
			glColor3f(0.0f, 0.3f, 0.0f);
			glLineWidth(6);
			glBegin(GL_LINE_STRIP);
			glVertex2f(x + 0.006, y + h);
			glVertex2f(x + w+0.006, y + h);
			glEnd();
			glLineWidth(10);
			glBegin(GL_LINE_STRIP);
			glVertex2f(x + w, y + h + 0.03);
			glVertex2f(x + w, y);
			glEnd();
		}
		fontx = x + w/5.5;
		fonty = y + h / 2.6;
		//printf_s("x %f \n y %f \n", fontx, fonty);
		glColor3f(1, 0, 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(0, 0, 0);
		drawText(GLUT_BITMAP_TIMES_ROMAN_24, label, fontx, fonty );
	}
	int ButtonArea()
	{
		double WminX, WminY, WmaxX, WmaxY;
		WindowCoordinates(x, -y);
		WminX = winX;
		WminY = winY;
		WindowCoordinates((x+w), -(y+h));
		WmaxX = winX;
		WmaxY = winY;
		printf_s(" Window x %f \n Window y  %f \n Window x1 %f  Window y1 %f \n", WminX, WminY, WmaxX,WmaxY);
		if (mouse.x > WminX      &&
			mouse.x < WmaxX		 &&
			mouse.y > WminY      &&
			mouse.y < WmaxY)
		
			return 1;
				
		return 0;
	}
	void OnButtonClicked()
	{
		//printf_s(" x %f \n y %f \n", mouse.x, mouse.y);
			//if the mouse click was within the buttons client area, set the state to true
		

		if (ButtonArea() && mouse.ButtonClicked)
		{
			//printf_s("inside x %f \n inside y %f \n", mouse.x, mouse.y);
			state = 1;
			callbackFunction();
		}
			else
				state = 0;
		glutPostRedisplay();
	}

	void ButtonPassive()
	{
		
		printf_s("x %f \n y %f \n", mouse.x, mouse.y);
			//if the mouse moved over the control
			if (ButtonArea())
			{
				//if (highlighted == 0) {
					//highlighted = 1;
					printf_s("inside x %f \n inside y %f \n", mouse.x, mouse.y);
					//glutPostRedisplay();
				
			}
			//else
			//{
				if (highlighted == 1)
				{
					highlighted = 0;
					glutPostRedisplay();
				}
			//}
	}
};
void draw_cube(float, float, float);
void TheButtonCallback()
{
	printf("I have been called\n");
}
const double cube_size = 1;
double gap = 0.04;
int n = 0;
double minValue;
char title[] = "Cube";
int refreshMills = 500;
char selector = ' ';
float selector_value ;
int rotX = 0,rotY=0;
int angle;
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void drawCube()
{
	double x, y, z;
	if (n & 1)
		minValue = -((n - 1) / 2 * cube_size);
	else
		minValue = -(n / 2 * cube_size - cube_size / 2);
	int count = 0;
	
	glPushMatrix();
	glRotatef(rotX, 1.0, 0.0, 0.0); // rotate in y axis
	glRotatef(rotY, 0.0, 1.0, 0.0); // rotate in x axis
	//GLfloat matrixf[16];
   //Generic rubiks cube
	FOR(i, minValue, -minValue, cube_size) {// step through x axis				

		FOR(j, minValue, -minValue, cube_size) {// step through y axis

			FOR(k, minValue, -minValue, cube_size)// step through z axis
			{

				x = i + i * gap;
				y = (j)+j * gap;
				z = (k)+k * gap;
				glPushMatrix();

				switch (selector)
				{
				case 'i': if (i == selector_value) glRotatef(angle, 1, 0, 0); break;
				case 'j': if (j == selector_value) glRotatef(angle, 0, 1, 0); break;
				case 'k': if (k == selector_value) glRotatef(angle, 0, 0, 1); break;

				}

				//glGetFloatv(GL_MODELVIEW_MATRIX, matrixf);
				//glLoadMatrixf(matrixf);
				draw_cube(x, y, z);
				glPopMatrix();
				//std::cout << x<< std::endl << y<< std::endl << z<< std::endl;
			}
		}
	}
	
	glPopMatrix();

}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	glLoadIdentity(); 
	glTranslatef(0, 0, -10);
	Button bstart = Button(-1, -1, 2.5, 1.15, 0, 0, "START", TheButtonCallback);
	bstart.ButtonDraw();
	//Button bstart1 = Button(-3, -3, 2.5, 1.15, 0, 0, "START1", TheButtonCallback);
	//bstart1.ButtonDraw();
	displayTitle();
	drawCube();
	glutSwapBuffers();
}

void draw_cube(float x, float y, float z)
{

	//vector<cube_rotate> rot = cubeRotations[x][y][z];
	//glLoadIdentity();
	//glRotatef(angle, 0, 0, 0);
	glPushMatrix();
	 glTranslatef(x,y,z);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);  // front
	glNormal3f(0.0, 0.0, 1.0);  // face normal
	glVertex3f(cube_size / 2, cube_size / 2, cube_size / 2);
	glVertex3f(-cube_size / 2, cube_size / 2, cube_size / 2);
	glVertex3f(-cube_size / 2, -cube_size / 2, cube_size / 2);
	glVertex3f(cube_size / 2, -cube_size / 2, cube_size / 2);
	glEnd();

	glColor3f(1.0f, 0.5f, 0.0f);
	glBegin(GL_QUADS);  // back
	glNormal3f(0.0, 0.0, -1.0);  // face normal
	glVertex3f(cube_size / 2, cube_size / 2, -cube_size / 2);
	glVertex3f(cube_size / 2, -cube_size / 2, -cube_size / 2);
	glVertex3f(-cube_size / 2, -cube_size / 2, -cube_size / 2);
	glVertex3f(-cube_size / 2, cube_size / 2, -cube_size / 2);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);  // left
	glNormal3f(-1.0, 0.0, 0.0);  // face normal 
	glVertex3f(-cube_size / 2, cube_size / 2, cube_size / 2);
	glVertex3f(-cube_size / 2, cube_size / 2, -cube_size / 2);
	glVertex3f(-cube_size / 2, -cube_size / 2, -cube_size / 2);
	glVertex3f(-cube_size / 2, -cube_size / 2, cube_size / 2);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);  // right
	glNormal3f(1.0, 0.0, 0.0);  // face normal
	glVertex3f(cube_size / 2, cube_size / 2, cube_size / 2);
	glVertex3f(cube_size / 2, -cube_size / 2, cube_size / 2);
	glVertex3f(cube_size / 2, -cube_size / 2, -cube_size / 2);
	glVertex3f(cube_size / 2, cube_size / 2, -cube_size / 2);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);  // top
	glNormal3f(0.0, 1.0, 0.0);  // face normal
	glVertex3f(-cube_size / 2, cube_size / 2, -cube_size / 2);
	glVertex3f(-cube_size / 2, cube_size / 2, cube_size / 2);
	glVertex3f(cube_size / 2, cube_size / 2, cube_size / 2);
	glVertex3f(cube_size / 2, cube_size / 2, -cube_size / 2);
	glEnd();

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);  // bottom
	glNormal3f(0.0, -1.0, 0.0);  // face normal
	glVertex3f(-cube_size / 2, -cube_size / 2, -cube_size / 2);
	glVertex3f(cube_size / 2, -cube_size / 2, -cube_size / 2);
	glVertex3f(cube_size / 2, -cube_size / 2, cube_size / 2);
	glVertex3f(-cube_size / 2, -cube_size / 2, cube_size / 2);
	glEnd();

	glPopMatrix();

}

//to display text on the screen
void drawText(void *font, const char *text, float x, float y)
{
	//glPushMatrix();
	
	glRasterPos2f(x, y);

	while (*text != '\0')
	{
		glutBitmapCharacter(font, *text);
		++text;
	}
	//glPopMatrix();
}
//mouse functions
void MouseButton(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse.ButtonClicked = 1;
	}
	else
		mouse.ButtonClicked = 0;
	mouse.x = x;
	mouse.y = y;
	glutPostRedisplay();
}
void displayTitle() {
	//glPushMatrix();
	//glTranslatef(0.5, 0, 0);
	glColor3f(1.0, 0.27, 0.0);
	drawText(GLUT_BITMAP_TIMES_ROMAN_24, "R",-2+0.5, 2.5);
	glColor3f(0.9, 0, 0);
	drawText(GLUT_BITMAP_TIMES_ROMAN_24, "U",-1.6+0.5, 2.5);
	glColor3f(1, 1.0, 0);
	drawText(GLUT_BITMAP_TIMES_ROMAN_24, "B" ,-1.2+0.5, 2.5);
	glColor3f(0.3, 0.8, 0);
	drawText(GLUT_BITMAP_TIMES_ROMAN_24,"I",-0.8+0.5, 2.5);
	glColor3f(0, 0.5, 1);
	drawText(GLUT_BITMAP_TIMES_ROMAN_24,"K", -0.4+0.5, 2.5);
	glColor3f(0.4, 0.7, 0.1);
	drawText(GLUT_BITMAP_TIMES_ROMAN_24,"S",0.0+0.5,2.5);
	//glPopMatrix();
}
void MousePassiveMotion(int x, int y)
{
	//double normalizedX = -1.0 + 2.0 * (double)x / 640;
	//double normalizedY = 1.0 - 2.0 *modelview info;
	mouse.x = x;
	mouse.y = y;
	glutPostRedisplay();
}

//keyboard function
void keyboardFunc(unsigned char key, int x, int y)
{

	switch (key) {

	case '1':
		n = 2;
		break;

	case '9':
		
		break;

	case '2':
		n = 3;
		break;
	
	case 'R': // right
	case 'r':
		rotY += 24;
		std::cout << "l" << std::endl;
		break;
	
	case 'L': // left
	case 'l':
		rotY -= 24;
		std::cout << "r" << std::endl;
		break; 
	
	case 'U': //Up
	case 'u':
		std::cout << "u" << std::endl;
		rotX += 24;
			break;

	case 'D': // Down
	case 'd':
		std::cout << "d" << std::endl;
		rotX -= 24;
			break;
	case 'i'://side face antiClockwise
		selector = 'i';
		selector_value = minValue;
		angle -= 90;
		break;
	case 'I'://side face clockwise
		selector = 'i';
		selector_value = minValue;
			angle+= 90;
			break;


	case 'j':
		selector = 'i';
		selector_value = minValue+cube_size;
		angle -= 90;
		break;
	case 'J':
		selector = 'i';
		selector_value = minValue+cube_size;
		angle += 90;
		break;

	case 'k': // up
		selector = 'i';
		selector_value = minValue + 2*cube_size;
		angle -= 90;
		break;

	case 'K':
		selector = 'i';
		selector_value = minValue + 2*cube_size;
		angle += 90;
		break;
		//rot_x = (rot_x - crement) % 360;
		break;
		// end of view rotation

		// cube movements

		// select cube face
		// x-axis faces
	case 'q':
		selector = 'j';
		selector_value = minValue;
		angle -= 90;
		break;
	case 'Q':
		selector = 'j';
		selector_value = minValue;
		angle += 90;
		break;
	
	case 'm':
		selector = 'j';
		selector_value = minValue +  cube_size;
		angle -= 90;
		break;
	case 'M':
		selector = 'j';
		selector_value = minValue +  cube_size;
		angle+= 90;
		break;
	case 'b':
		selector = 'j';
		selector_value = minValue + 2 * cube_size;
		angle -= 90;
		break;

	case 'B':
		selector = 'j';
		selector_value = minValue + 2 * cube_size;
		angle -= 90;
		break;

	
	case 'w':
		selector = 'k';
		selector_value = minValue + 2 * cube_size;
		angle -= 90;
		break;

	case 'W':
		selector = 'k';
		selector_value = minValue + 2 * cube_size;
		angle += 90;
		break;

	case 'E':
		selector = 'k';
		selector_value = minValue + cube_size;
		angle += 90;
		break;
	case 'e':
		selector = 'k';
		selector_value = minValue + cube_size;
		angle -= 90;
		break;

		// y-axis faces
	case 'A':
		selector = 'k';
		selector_value = minValue ;
		angle += 90;
	case 'a':
		selector = 'k';
		selector_value = minValue;
		angle -= 90;
		
		break;

	

		// z-axis faces
	case 'C':
	case 'c':

		break;

	case 'X':
	case 'x':
	
		break;

	case 'Z':
	case 'z':
		
		break;

		// move selected face
	

		// end of cube movements

	default:
		break;

	}

	glutPostRedisplay();

}




void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	winw = width;
	winh = height;
	// Set the viewport to cover the new window
	glViewport(0, 0, winw, winh);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);						 // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH); // Enable double buffered mode
	glutInitWindowSize(winw, winh);				// Set the window's initial width & height
	glutInitWindowPosition(50, 50);				// Position the window's initial top-left corner
	glutCreateWindow(title);					// Create window with the given title
	glutDisplayFunc(display);					// Register callback handler for window re-paint event
	glutReshapeFunc(reshape);					// Register callback handler for window re-size event
	//initGL();									// Our own OpenGL initialization
	//glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(MouseButton);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutMainLoop();								// Enter the infinite event-processing loop
	return 0;
}