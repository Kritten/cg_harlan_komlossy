//////////////////////////////////////////////////////////////////////////
//Sources: http://openglbook.com/
////////// http://www.dhpoware.com/demos/glObjViewer.html
////////// http://www.arcsynthesis.org/gltut/
/////////////////////////////////////////////////////////////////////////
#define GLEW_STATIC
#define M_PI    3.14159265358979323846f

#include <iostream>
#include <vector>
#include <cmath>

#include <climits>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <TransformationStack.h>
#include <Shader.h>
//include gloost::Mesh wich is a geometry container
#include <Mesh.h>
//mesh object for loading planet geometry
gloost::Mesh* mesh = 0;

// loader for the wavefront *.obj file format
#include <ObjLoader.h>

//#include<FreeImage.h>

#define PI 3.1415926535897932384626433832795028841971

//
int windowWidth = 800;
int windowHeight = 600;
int windowHandle = 0;

//initial camera position
float camera_position[] = {0.0f, 6.0, 17.0f};

bool middleMouseButtonDown = false;
bool leftMouseButtonDown = false;
bool rightMouseButtonDown = false;

unsigned frameCount = 0;

//speed
float g_elapsed_virtual_time = 0.0;
int g_last_elapsed_time = 0;
float g_speed = 0.0001;

//navigation
float last_mouse_x = 0;
float last_mouse_y = 0;
glm::mat4 g_viewMatrix = glm::mat4(1.0);
float camera_x = 0;
float camera_y = 0;
glm::vec3 g_viewing_direction = glm::vec3(0.0f, 0.0f, -1.0f); 
float g_movement_speed = 0.2f;

bool g_key_w = false;
bool g_key_s = false;
bool g_key_a = false;
bool g_key_d = false;

//handles for shader variables
unsigned projectionMatrixUniformLocation = 0;
unsigned modelMatrixUniformLocation  = 0;
unsigned viewMatrixUniformLocation = 0;
unsigned normalMatrixUniformLocation     = 0;

unsigned orbitProjectionMatrixUniformLocation = 0;
unsigned orbitModelMatrixUniformLocation  = 0;
unsigned orbitViewMatrixUniformLocation = 0;
unsigned orbitColorUniformLocation     = 0;


//handles for all sort of geometry objects
unsigned vertexArrayObject = 0;
unsigned vertexBufferObject = 0;
unsigned elementArrayBuffer = 0;

bool g_draw_orbits = true;
std::vector<float> orbitVertices;
unsigned orbitVertexArrayObject = 0;
unsigned orbitVertexBufferObject = 0;
unsigned orbitElementArrayBuffer = 0;



//handles for shader program and shaders
unsigned shaderProgram = 0;
unsigned orbitShaderProgram = 0;

unsigned vertexShader = 0;
unsigned fragmentShader = 0;


// Scales
float g_neptunScale = 1.1;
float g_uranusScale = 2.0;
float g_saturnScale = 1.7;
float g_jupiterScale = 0.9;
float g_marsScale = 0.9;
float g_earthScale = 1.1;
float g_venusScale = 0.7;
float g_mercuryScale = 0.5;
float g_sunScale = 3.0;
float g_earthMoonScale = 0.2;

// Translations
glm::vec3 g_neptunTranslate = glm::vec3(19.0f, 0.0f, 0.0f);
glm::vec3 g_uranusTranslate = glm::vec3(17.0f, 0.0f, 0.0f);
glm::vec3 g_saturnTranslate = glm::vec3(15.0f, 0.0f, 0.0f);
glm::vec3 g_jupiterTranslate = glm::vec3(13.0f, 0.0f, 0.0f);
glm::vec3 g_marsTranslate = glm::vec3(11.0f, 0.0f, 0.0f);
glm::vec3 g_earthTranslate = glm::vec3(9.0f, 0.0f, 0.0f);
glm::vec3 g_mercuryTranslate = glm::vec3(5.0f, 0.0f, 0.0f);
glm::vec3 g_venusTranslate = glm::vec3(7.0f, 0.0f, 0.0f);
glm::vec3 g_earthMoonTranslate = glm::vec3(1.0f, 0.0f, 0.0f);

// Rotations 
 float g_neptunRotation  = 0.0f;
 float g_uranusRotation  = 0.0f;
 float g_saturnRotation  = 0.0f;
 float g_jupiterRotation = 0.0f;
 float g_marsRotation    = 0.0f;
 float g_earthRotation   = 0.0f;
 float g_venusRotation   = 0.0f;
 float g_mercuryRotation = 0.0f;

 float g_earthMoonRotation = 0.0f;

//the three different matrices for projection, viewing and model transforming

TransformationStack modelTransformationStack;
TransformationStack cameraTransformationStack;

//Function callbacks
void initialize(int, char*[]);
void initWindow(int, char*[]);
void resizeFunction(int, int);
void idleFunction(void);
void compute_viewMatrix();
//forward declaration of functions
void drawSolarsystem();
void drawOrbits();
void generateOrbit();

void mouseInput(int button, int state, int x, int y);
void specialKeyRelease(int keyEvent, int x, int y);
void specialKeyPress(int keyEvent, int x, int y);
void keyRelease(unsigned char keyEvent, int x, int y);
void keyPress(unsigned char keyEvent, int x, int y);

void timerFunction(int);
void cleanup(void);
void loadModel(void);
void setupShader();
void draw(void);
void renderFunction(void);
glm::vec3 compute_viewing_direction(glm::mat4 matrix);
void print_matrix(glm::mat4 matrix);
void navigate();
void drawPlanet(glm::mat4 const& model_matrix);

/////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
	initialize(argc, argv);

	//start the glut event system
	glutMainLoop();

	exit(EXIT_SUCCESS);
}


/////////////////////////////////////////////////////////////////////////////////////////


//called every frame this functions draw
void draw(void)
{
	navigate();
	compute_viewMatrix();
	drawSolarsystem();
	if(g_draw_orbits)
	{
		drawOrbits();
	}
}

void navigate()
{
	if(g_key_w)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += g_viewing_direction[0] * g_movement_speed;
		camera_position[1] += g_viewing_direction[1] * g_movement_speed;
		camera_position[2] += g_viewing_direction[2] * g_movement_speed;
	}
	
	if(g_key_s)
	{
	    g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] -= g_viewing_direction[0] * g_movement_speed;
		camera_position[1] -= g_viewing_direction[1] * g_movement_speed;
		camera_position[2] -= g_viewing_direction[2] * g_movement_speed;
	}
	if(g_key_a)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += g_viewing_direction[2] * g_movement_speed;
		// camera_position[1] += g_viewing_direction[1] * g_movement_speed;
		camera_position[2] += -g_viewing_direction[0] * g_movement_speed;
	}
	if(g_key_d)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += -g_viewing_direction[2] * g_movement_speed;
		// camera_position[1] += g_viewing_direction[1] * g_movement_speed;
		camera_position[2] += g_viewing_direction[0] * g_movement_speed;
	}
}

void compute_viewMatrix()
{
	cameraTransformationStack.clear();

	//translate the camera in positive z direction to actually see the geometry residing in the coordinate origin
	cameraTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), camera_x, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	cameraTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -camera_y, glm::vec3(1.0f, 0.0f, 0.0f) ) );
    cameraTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), glm::vec3(camera_position[0], camera_position[1], camera_position[2]) ) );
    // cameraTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(camera_position[0], camera_position[1], camera_position[2]) ) );

	//rotate the camera

	//invert the camera transformation to move the vertices
	g_viewMatrix = glm::inverse(cameraTransformationStack.topMatrix() );

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE,  glm::value_ptr(g_viewMatrix) );

	glUseProgram(orbitShaderProgram);
	glUniformMatrix4fv(orbitViewMatrixUniformLocation, 1, GL_FALSE,  glm::value_ptr(g_viewMatrix) );
}

void drawOrbits()
{
	glUseProgram(orbitShaderProgram);
	glBindVertexArray(orbitVertexArrayObject);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_mercuryTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.48f, 0.48f, 0.48f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_venusTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.97f, 0.65f, 0.22f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_earthTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.35f, 0.48f, 0.820f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);
    
	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_earthMoonTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), g_earthTranslate ) );
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), g_earthRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.8f, 0.8f, 0.8f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_marsTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.76f, 0.58f, 0.34f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_jupiterTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.84f, 0.71f, 0.53f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_saturnTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.81f, 0.71f, 0.55f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_uranusTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.44f, 0.62f, 0.67f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_neptunTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.24f, 0.39f, 0.86f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
}

//////////////////////////////////////////////////////////////////////////

void drawSolarsystem()
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	int time_difference = now - g_last_elapsed_time;
	g_last_elapsed_time = now;
	g_elapsed_virtual_time += (float)time_difference * g_speed;


	// Rotation
	g_neptunRotation  = g_elapsed_virtual_time;
	g_uranusRotation  = g_neptunRotation *2.0;
	g_saturnRotation  = g_uranusRotation *3.0;
	g_jupiterRotation = g_saturnRotation *5.0/2.0;
	g_marsRotation    = g_jupiterRotation*6.0;
	g_earthRotation   = g_marsRotation   *2.0;
	g_venusRotation   = g_earthRotation  *13.0/8.0;
	g_mercuryRotation = g_venusRotation  *2.5;

	g_earthMoonRotation = g_earthRotation*12.0;

	glUseProgram(shaderProgram);

	//sun
	//scale the sun
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_sunScale) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//mercury
	//scale the mercury
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_mercuryScale) ) );
	//translate the mercury
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_mercuryTranslate) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_mercuryRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//venus
	//scale the venus
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_venusScale) ) );
	//translate the venus
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_venusTranslate) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_venusRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//earth
	//scale the earth
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_earthScale) ) );
	//translate the earth
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_earthTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_earthRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

    //earthmoon
	//scale the earthmoon
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_earthMoonScale) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_earthMoonTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_earthMoonRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_earthTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_earthRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//mars
	//scale the mars
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_marsScale) ) );
	//translate the mars
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_marsTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_marsRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//jupiter
	//scale the jupiter
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_jupiterScale) ) );
	//translate the jupiter
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_jupiterTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//saturn
	//scale the saturn
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_saturnScale) ) );
	//translate the saturn
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_saturnTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_saturnRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//uranus
	//scale the uranus
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_uranusScale) ) );
	//translate the uranus
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_uranusTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_uranusRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//neptun
	//scale the neptun
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_neptunScale) ) );
	//translate the neptun
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_neptunTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_neptunRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	glBindVertexArray(0);
	glUseProgram(0);
}

/////////////////////////////////////////////////////////////////////////////////////////
void mouseInput(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_UP)
		{
			last_mouse_x = 0;
			last_mouse_y = 0;
		}
	}
}

void mouseMovement(int x, int y)
{
	if(last_mouse_x != 0)
	{
		camera_x = camera_x + last_mouse_x - (float)x;
	}
	last_mouse_x = x;

	if(last_mouse_y != 0)
	{
		camera_y = camera_y + last_mouse_y - (float)y;
	}
	last_mouse_y = y;
}
/////////////////////////////////////////////////////////////////////////////////////////
void specialKeyRelease(int keyEvent, int x, int y)
{
	//handle key release events of special keys here (like arrow keys)
}

/////////////////////////////////////////////////////////////////////////////////////////
void specialKeyPress(int keyEvent, int x, int y)
{
	//handle key press events of special keys here (like arrow keys)
}

/////////////////////////////////////////////////////////////////////////////////////////
void keyRelease(unsigned char keyEvent, int x, int y)
{
	if(keyEvent == ' ')
	{
		setupShader();
	    resizeFunction(windowWidth, windowHeight);
	}
	if(keyEvent == 'f' || keyEvent == 'F')
	{
		glutFullScreenToggle();
	}
    if(keyEvent == 27) //escape key
	{
		cleanup();
		glutExit();
	}
	if(keyEvent == 'w' || keyEvent == 'W')
	{
		g_key_w = false;
	}
	if(keyEvent == 's' || keyEvent == 'S')
	{
		g_key_s = false;
	}
	if(keyEvent == 'a' || keyEvent == 'A')
	{
		g_key_a = false;
	}
	if(keyEvent == 'd' || keyEvent == 'D')
	{
		g_key_d = false;
	}
	if(keyEvent == 'q' || keyEvent == 'Q')
	{
		g_speed *= 0.8;
	}
	if(keyEvent == 'e' || keyEvent == 'E')
	{
		g_speed *= 1.2;
	}
	if(keyEvent == 'o' || keyEvent == 'O')
	{
		g_draw_orbits = !g_draw_orbits;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void keyPress(unsigned char keyEvent, int x, int y)
{
	if(keyEvent == 'w' || keyEvent == 'W')
	{
		g_key_w = true;
	}
	if(keyEvent == 's' || keyEvent == 'S')
	{
		g_key_s = true;
	}
	if(keyEvent == 'a' || keyEvent == 'A')
	{
		g_key_a = true;
	}
	if(keyEvent == 'd' || keyEvent == 'D')
	{
		g_key_d = true;
	}
}




/////////////////////////////////////////////////////////////////////////////////////////
void timerFunction(int value){
	if(0 != value)
	{
		int fps = frameCount * 4;
		glutSetWindowTitle( (gloost::toString(fps) + " fps").c_str());

	}
	frameCount = 0;
	glutTimerFunc(250, timerFunction, 1);
}


void renderFunction(void)
{
	++frameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw();

	glutSwapBuffers();
	glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////////////


void setupShader()
{

	// LOAD AND LINK SHADER
	shaderProgram = glCreateProgram();
	{
		//load a shader (of the given type) and compile it in the convenience class 'Shader'
		vertexShader = Shader::loadShader("../../../data/shaders/simpleVertexShader.vs", GL_VERTEX_SHADER);
		fragmentShader = Shader::loadShader("../../../data/shaders/simpleFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attach the different shader components to the shader program ...
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
	}
	//... and compile it
	glLinkProgram(shaderProgram);

	    //program is linked, so we can detach compiled shaders again
		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);


	// LOAD AND LINK SHADER
	orbitShaderProgram = glCreateProgram();
	{
		//load a shader (of the given type) and compile it in the convenience class 'Shader'
		vertexShader = Shader::loadShader("../../../data/shaders/orbitVertexShader.vs", GL_VERTEX_SHADER);
		fragmentShader = Shader::loadShader("../../../data/shaders/orbitFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attach the different shader components to the shader program ...
		glAttachShader(orbitShaderProgram, vertexShader);
		glAttachShader(orbitShaderProgram, fragmentShader);
	}
	//... and compile it
	glLinkProgram(orbitShaderProgram);

	    //program is linked, so we can detach compiled shaders again
		glDetachShader(orbitShaderProgram, vertexShader);
		glDetachShader(orbitShaderProgram, fragmentShader);


	//obtain shader variable locations
	modelMatrixUniformLocation  = glGetUniformLocation(shaderProgram, "ModelMatrix");
	viewMatrixUniformLocation = glGetUniformLocation(shaderProgram, "ViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
	normalMatrixUniformLocation     = glGetUniformLocation(shaderProgram, "NormalMatrix");

	//obtain shader variable locations
	orbitModelMatrixUniformLocation  = glGetUniformLocation(orbitShaderProgram, "ModelMatrix");
	orbitViewMatrixUniformLocation = glGetUniformLocation(orbitShaderProgram, "ViewMatrix");
	orbitProjectionMatrixUniformLocation = glGetUniformLocation(orbitShaderProgram, "ProjectionMatrix");
	orbitColorUniformLocation     = glGetUniformLocation(orbitShaderProgram, "OrbitColor");
}


/////////////////////////////////////////////////////////////////////////////////////////


void loadModel()
{

	//load a wavefront *.obj file
	gloost::ObjLoader loader("../../../data/objects/sphere.obj");
	mesh = loader.getMesh();

	//IMPORTANT: use this to increase the reference counter
	//gloost::meshes have a garbage collector which throws
	//the mesh away otherwise
	mesh->takeReference();

	mesh->generateNormals();

	//normalizes the mesh
	mesh->scaleToSize(1.0);
	//puts the meshdata in one array
	mesh->interleave();

	//mesh->printMeshInfo();

	//create V(ertex)A(rray)O(object) which stores the specified attributes of our V(ertex)B(uffer)O(bject)
	glGenVertexArrays(1, &vertexArrayObject);
	//bind VAO - scope begins
	glBindVertexArray(vertexArrayObject);


	//create a vertex buffer object
	glGenBuffers(1, &vertexBufferObject);
	//and bind it as a array buffer target
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	//load data that resides in CPU RAM into video RAM.
	glBufferData(GL_ARRAY_BUFFER, //??
		sizeof(float) * mesh->getInterleavedAttributes().size(), //??
		&mesh->getInterleavedAttributes().front(), //??
		GL_STATIC_DRAW); //??

	//enable vertex attribute at location 0 (compare with vertex shader input)
	glEnableVertexAttribArray(0);

	//specify where to read the data for attribute at location 0
	glVertexAttribPointer(0,
		GLOOST_MESH_NUM_COMPONENTS_VERTEX,
		GL_FLOAT, //datatype
		GL_FALSE, //should the data be normalized?
		mesh->getInterleavedInfo().interleavedPackageStride, //size of attribute stride for one primitive
		(GLvoid*)(mesh->getInterleavedInfo().interleavedVertexStride)); //offset in stride


	//enable vertex attribute at location 1 (compare with vertex shader input)
	glEnableVertexAttribArray(1);

	//specifies where in the GL_ARRAY_BUFFER our data(the vertex normal) is exactly (compare with vertex shader input)
	glVertexAttribPointer(1,
		GLOOST_MESH_NUM_COMPONENTS_NORMAL,
		GL_FLOAT, GL_FALSE,
		mesh->getInterleavedInfo().interleavedPackageStride,
		(GLvoid*)(mesh->getInterleavedInfo().interleavedNormalStride));


	//the buffer that becomes the element array object is created
	glGenBuffers(1, &elementArrayBuffer);
	//and then bound to the element array buffer target
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
	//the triangle indices are loaded and associated with the bound array element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(gloost::TriangleFace) * mesh->getTriangles().size(),
		&mesh->getTriangles().front(),
		GL_STATIC_DRAW);

	// unbind the VAO - scope ends
	glBindVertexArray(0);

}

/////////////////////////////////////////////////////////////////////////////////////////


void generateOrbit()
{
	for (int i = 0; i<360; ++i)
	{
		orbitVertices.push_back(std::cos(i*M_PI/180.0f));
		orbitVertices.push_back(std::sin(i*M_PI/180.0f));
		orbitVertices.push_back(0.0);
	}

	glGenVertexArrays(1, &orbitVertexArrayObject);
	glBindVertexArray(orbitVertexArrayObject);

	glGenBuffers(1, &orbitVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, orbitVertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER
			   , sizeof(float)*orbitVertices.size()
			   , &orbitVertices[0]
			   , GL_STATIC_DRAW
			   );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0
    					, 3
    					, GL_FLOAT
    					, GL_FALSE
    					, sizeof(float)* 3
    					, (GLvoid*)(0)
    					);


}

/////////////////////////////////////////////////////////////////////////////////////////


void cleanup()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);

	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &elementArrayBuffer);

	glDeleteVertexArrays(1, &vertexArrayObject);
}


/////////////////////////////////////////////////////////////////////////////////////////


void idleFunction(void)
{
	glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////////////


void resizeFunction(int Width, int Height)
{
	windowWidth = Width;
	windowHeight = Height;
	glViewport(0, 0, windowWidth, windowHeight);

	                           //create a projection matrix
	glm::mat4 projectionMatrix = glm::perspective(90.0f, //FOV 60.0°
		                                         (float)windowWidth/windowHeight, //aspect ratio of the projection
												 1.0f, //near clipping plane
												 100.0f); //far clipping plane

	glUseProgram(shaderProgram); //bind shader program
	//upload projection matrix to the shader.
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix) ); //upload projection matrix to bound shader
	glUseProgram(0); //unbind shader program

	glUseProgram(orbitShaderProgram); //bind shader program
	//upload projection matrix to the shader.
	glUniformMatrix4fv(orbitProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix) ); //upload projection matrix to bound shader
	glUseProgram(0); //unbind shader program
}


/////////////////////////////////////////////////////////////////////////////////////////


void initWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(windowWidth, windowHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	windowHandle = glutCreateWindow("");

	if(windowHandle < 1)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		glutExit();
	}

	//GLUT function callbacks. This means: register functions which are called for the specific glut events. Mind the function signature!
	//??
	glutMouseFunc(mouseInput);
	glutMotionFunc(mouseMovement);
	//??
	glutSpecialFunc(specialKeyPress);
	//??
	glutSpecialUpFunc(specialKeyRelease);
	//??
	glutKeyboardFunc(keyPress);
	//??
	glutKeyboardUpFunc(keyRelease);

	//??
	glutTimerFunc(0, timerFunction, 0);
	//??
	glutReshapeFunc(resizeFunction);
	//??
	glutDisplayFunc(renderFunction);
	//??
	glutIdleFunc(idleFunction);
}

/////////////////////////////////////////////////////////////////////////////////////////

void initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	initWindow(argc, argv);

	glewExperimental = GL_TRUE;
	//initialize glew for extensions
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
			);
		glutExit();
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

	glGetError();
	//set color to clear the frame buffer with
	glClearColor(0.20f, 0.2f, 0.2f, 0.0f);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//create shaders
	setupShader();
	//load model and fill buffer objects
	loadModel();
	generateOrbit();
}


void drawPlanet(glm::mat4 const& model_matrix)
{
		// transfer model matrix to shader by the associated uniform location
		glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(model_matrix) );

		//calculate the normal transfomrations from modelview matrix
	    glm::mat4 normalMatrix = glm::mat4(1.0f);
		normalMatrix = g_viewMatrix*modelTransformationStack.topMatrix();
		normalMatrix = glm::inverse(normalMatrix);
		normalMatrix = glm::transpose(normalMatrix);

		// transfer NormalMatrix for Geometry to Shaders
		glUniformMatrix4fv(normalMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix) );

		//bind the planet geometry (the VAO!)
		glBindVertexArray(vertexArrayObject);

		// draw object according to (the EAO!). Note, that the geometry type is triangles.
		glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);

}

glm::vec3 compute_viewing_direction(glm::mat4 matrix)
{
	// std::cout << -matrix[2][0] << " " << -matrix[2][1] << " " << -matrix[2][2] << std::endl << std::endl;
	return glm::vec3(-matrix[2][0], -matrix[2][1], -matrix[2][2]);
}

void print_matrix(glm::mat4 matrix)
{
	// std::cout << matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << " " << matrix[3][0] << std::endl;
	// std::cout << matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << " " << matrix[3][1] << std::endl;
	// std::cout << matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << " " << matrix[3][2] << std::endl;
	// std::cout << matrix[0][3] << " " << matrix[1][3] << " " << matrix[2][3] << " " << matrix[3][3] << std::endl << std::endl;
	// std::cout << -matrix[2][0] << " " << -matrix[2][1] << " " << -matrix[2][2] << std::endl << std::endl;
}