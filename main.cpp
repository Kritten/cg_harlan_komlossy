//////////////////////////////////////////////////////////////////////////
//Sources: http://openglbook.com/
////////// http://www.dhpoware.com/demos/glObjViewer.html
////////// http://www.arcsynthesis.org/gltut/
/////////////////////////////////////////////////////////////////////////
#define GLEW_STATIC
#define M_PI    3.14159265358979323846f

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <chrono>

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

std::vector<glm::vec3> tangents;
std::vector<glm::vec3> bitangents;
std::vector<float> meshInfo;
// loader for the wavefront *.obj file format
#include <ObjLoader.h>

//#include<FreeImage.h>
#include "TextureLoader.h"

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

int greyscale = 0;
int horizontal_mirrowed = 0;
int vertical_mirrowed = 0;
int blur = 0;

//speed
float g_elapsed_virtual_time = 0.0;
int g_last_elapsed_time = 0;
float g_speed = 0.0001;
float g_toggle = 1.0;

//navigation
float last_mouse_x = 0;
float last_mouse_y = 0;
glm::mat4 g_viewMatrix = glm::mat4(1.0);
float camera_x = 0;
float camera_y = 0;
glm::vec3 g_viewing_direction = glm::vec3(0.0f, 0.0f, -1.0f);
float g_movement_speed = 0.2f;
float g_slow_movement_speed = 0.008f;
float g_mouse_speed = 0.4f;
float g_key_shift_multiplicator = 3.0f;
float g_ascending_speed = 0.3;
float g_descending_speed = 0.3;

bool g_key_w = false;
bool g_key_s = false;
bool g_key_a = false;
bool g_key_d = false;
bool g_key_q = false;
bool g_key_e = false;
bool g_key_shift = false;
bool g_key_ctrl = false;
bool g_key_space = false;
bool g_key_z = false;
bool g_key_h = false;
bool g_key_g = false;
bool g_key_j = false;
bool g_key_t = false;
bool g_key_u = false;

//handles for shader variables
unsigned projectionMatrixUniformLocation = 0;
unsigned modelMatrixUniformLocation  = 0;
unsigned viewMatrixUniformLocation = 0;
unsigned normalMatrixUniformLocation     = 0;
unsigned planetColorUniformLocation = 0;
unsigned shadingModeUniformLocation = 0;
unsigned lightPositionUniformLocation = 0;

unsigned sunProjectionMatrixUniformLocation = 0;
unsigned sunModelMatrixUniformLocation  = 0;
unsigned sunViewMatrixUniformLocation = 0;
unsigned sunColorUniformLocation0 = 0;
unsigned sunColorUniformLocation1 = 0;
unsigned sunTimeUniformLocation = 0;

unsigned orbitProjectionMatrixUniformLocation = 0;
unsigned orbitModelMatrixUniformLocation  = 0;
unsigned orbitViewMatrixUniformLocation = 0;
unsigned orbitColorUniformLocation     = 0;
unsigned orbitLightPositionUniformLocation = 0;

unsigned skyModelMatrixUniformLocation  = 0;
unsigned skyViewMatrixUniformLocation = 0;
unsigned skyProjectionMatrixUniformLocation = 0;
unsigned skyColorUniformLocation     = 0;

unsigned normalColorUniformLocation     = 0;
unsigned glossColorUniformLocation     = 0;
unsigned specularColorUniformLocation     = 0;
unsigned displacementColorUniformLocation     = 0;

unsigned screenQuadShaderGreyscaleUniformLocation = 0;
unsigned screenQuadShaderHorizontalMirrowedUniformLocation = 0;
unsigned screenQuadShaderVerticalMirrowedUniformLocation = 0;
unsigned screenQuadShaderBlurUniformLocation = 0;
unsigned screenQuadShaderColorTextureUniformLocation = 0;
unsigned screenQuadShaderScreenDimensionsUniformLocation = 0;

//handels for all kind of textures
unsigned whiteTexture = 0;

unsigned skyTexture = 0;

unsigned neptuneTexture = 0;
unsigned uranusTexture = 0;
unsigned saturnTexture = 0;
unsigned jupiterTexture = 0;
unsigned marsTexture = 0;
unsigned earthTexture = 0;
unsigned venusTexture = 0;
unsigned mercuryTexture = 0;

unsigned sunTexture0 = 0;
unsigned sunTexture1 = 0;

unsigned moonTexture = 0;

unsigned earthNormalTexture = 0;
unsigned planetNormalTexture = 0;
unsigned gasplanetNormalTexture = 0;
unsigned planetSpecularTexture = 0;
unsigned planetDisplacementTexture = 0;
unsigned planetGlossTexture = 0;
unsigned earthDisplacementTexture = 0;
unsigned earthGlossTexture = 0;
unsigned earthSpecularTexture = 0;

//handles for all sort of geometry objects
unsigned vertexArrayObject = 0;
unsigned vertexBufferObject = 0;
unsigned elementArrayBuffer = 0;

bool g_draw_orbits = true;
std::vector<float> orbitVertices;
unsigned orbitVertexArrayObject = 0;
unsigned orbitVertexBufferObject = 0;
unsigned orbitElementArrayBuffer = 0;

unsigned screenQuadVAO = 0;
unsigned screenQuadVBO = 0;


//handles for shader programs and shaders
unsigned shaderProgram = 0;
unsigned sunShaderProgram = 0;
unsigned orbitShaderProgram = 0;
unsigned skyShaderProgram = 0;
unsigned screenQuadShaderProgram = 0;

unsigned vertexShader = 0;
unsigned fragmentShader = 0;


// handles for the custom FBO
unsigned renderFBO = 0;
unsigned renderColorBuffer = 0;
unsigned renderDepthBuffer = 0;


// Scales
float g_neptunScale = 1.8;
float g_uranusScale = 2.0;
float g_saturnScale = 2.8;
float g_jupiterScale = 3.0;
float g_marsScale = 0.6;
float g_earthScale = 1.0;
float g_venusScale = 0.9;
float g_mercuryScale = 0.5;
float g_sunScale = 3.0;
float g_earthMoonScale = 0.2;
float g_ioScale = 0.2;
float g_europaScale = 0.2;
float g_ganymedeScale = 0.2;
float g_callistoScale = 0.2;

// Translations
glm::vec3 g_neptunTranslate = glm::vec3(30.0f, 0.0f, 0.0f);
glm::vec3 g_uranusTranslate = glm::vec3(25.0f, 0.0f, 0.0f);
glm::vec3 g_saturnTranslate = glm::vec3(20.0f, 0.0f, 0.0f);
glm::vec3 g_jupiterTranslate = glm::vec3(15.0f, 0.0f, 0.0f);
glm::vec3 g_marsTranslate = glm::vec3(11.0f, 0.0f, 0.0f);
glm::vec3 g_earthTranslate = glm::vec3(9.0f, 0.0f, 0.0f);
glm::vec3 g_mercuryTranslate = glm::vec3(5.0f, 0.0f, 0.0f);
glm::vec3 g_venusTranslate = glm::vec3(7.0f, 0.0f, 0.0f);
glm::vec3 g_earthMoonTranslate = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 g_ioTranslate = glm::vec3(1.8f, 0.0f, 0.0f);
glm::vec3 g_europaTranslate = glm::vec3(2.1f, 0.0f, 0.0f);
glm::vec3 g_ganymedeTranslate = glm::vec3(2.5f, 0.0f, 0.0f);
glm::vec3 g_callistoTranslate = glm::vec3(2.8f, 0.0f, 0.0f);

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
float g_ioRotation  = 0.0f;
float g_europaRotation = 0.0f;
float g_ganymedeRotation = 0.0f;
float g_callistoRotation = 0.0f;

int g_num_of_saturn_rings = 100;
int g_num_of_uranus_rings = 30;


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
void drawSun();
void drawSkySphere();
void drawSolarsystem();
void draw_jupiter_moons();
void drawOrbits();
void draw_saturn_rings();
void draw_uranus_rings();


void mouseInput(int button, int state, int x, int y);
void specialKeyRelease(int keyEvent, int x, int y);
void specialKeyPress(int keyEvent, int x, int y);
void keyRelease(unsigned char keyEvent, int x, int y);
void keyPress(unsigned char keyEvent, int x, int y);

void timerFunction(int);
void cleanup(void);

void loadModel(void);
void createQuad();
std::vector<glm::vec3> calculateTangents();
void generateOrbit();
void loadTextures();

void setupShader();
void setupFrameBuffer();
void draw(void);
void geometry_render_pass();
void screen_quad_pass();
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
	
	geometry_render_pass();
	screen_quad_pass();
}



/////////////////////////////////////////////////////////////////////////////////////////

void geometry_render_pass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
	drawSkySphere();
	drawSun();
	drawSolarsystem();
	if(g_draw_orbits)
	{
		drawOrbits();
	}
	draw_saturn_rings();
	draw_uranus_rings();
}

/////////////////////////////////////////////////////////////////////////////////////////

void screen_quad_pass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(screenQuadShaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderColorBuffer);

	glUniform1i(screenQuadShaderColorTextureUniformLocation, 0);
	glUniform2i(screenQuadShaderScreenDimensionsUniformLocation, windowWidth, windowHeight);
	
	glUniform1i(screenQuadShaderGreyscaleUniformLocation, greyscale);
	glUniform1i(screenQuadShaderHorizontalMirrowedUniformLocation, horizontal_mirrowed);
	glUniform1i(screenQuadShaderVerticalMirrowedUniformLocation, vertical_mirrowed);
	glUniform1i(screenQuadShaderBlurUniformLocation, blur);

	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

/////////////////////////////////////////////////////////////////////////////////////////


void navigate()
{
	float movement_speed = g_movement_speed;
	if(g_key_shift)
	{
		movement_speed = g_key_shift_multiplicator * movement_speed;
	}
	if(g_key_z)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += g_viewing_direction[0] * g_slow_movement_speed;
		camera_position[1] += g_viewing_direction[1] * g_slow_movement_speed;
		camera_position[2] += g_viewing_direction[2] * g_slow_movement_speed;
	}

	if(g_key_h)
	{
	    g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] -= g_viewing_direction[0] * g_slow_movement_speed;
		camera_position[1] -= g_viewing_direction[1] * g_slow_movement_speed;
		camera_position[2] -= g_viewing_direction[2] * g_slow_movement_speed;
	}
	if(g_key_g)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += g_viewing_direction[2] * g_slow_movement_speed;
		camera_position[2] += -g_viewing_direction[0] * g_slow_movement_speed;
	}
	if(g_key_j)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += -g_viewing_direction[2] * g_slow_movement_speed;
		camera_position[2] += g_viewing_direction[0] * g_slow_movement_speed;
	}
	if(g_key_w)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += g_viewing_direction[0] * movement_speed;
		camera_position[1] += g_viewing_direction[1] * movement_speed;
		camera_position[2] += g_viewing_direction[2] * movement_speed;
	}

	if(g_key_s)
	{
	    g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] -= g_viewing_direction[0] * movement_speed;
		camera_position[1] -= g_viewing_direction[1] * movement_speed;
		camera_position[2] -= g_viewing_direction[2] * movement_speed;
	}
	if(g_key_a)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += g_viewing_direction[2] * movement_speed;
		camera_position[2] += -g_viewing_direction[0] * movement_speed;
	}
	if(g_key_d)
	{
		g_viewing_direction = compute_viewing_direction(glm::inverse(g_viewMatrix));
		camera_position[0] += -g_viewing_direction[2] * movement_speed;
		camera_position[2] += g_viewing_direction[0] * movement_speed;
	}
	if(g_key_t)
	{
		camera_position[1] += g_ascending_speed/20.0;
	}
	if(g_key_u)
	{
		camera_position[1] -= g_descending_speed/20.0;
	}
	if(g_key_q)
	{
		camera_position[1] += g_ascending_speed;
	}
	if(g_key_e)
	{
		camera_position[1] -= g_descending_speed;
	}

}

void compute_viewMatrix()
{
	cameraTransformationStack.clear();

	//translate the camera in positive z direction to actually see the geometry residing in the coordinate origin
	cameraTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -camera_y, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	cameraTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), camera_x, glm::vec3(0.0f, 1.0f, 0.0f) ) );
    cameraTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), glm::vec3(camera_position[0], camera_position[1], camera_position[2]) ) );
    // cameraTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(camera_position[0], camera_position[1], camera_position[2]) ) );

	//rotate the camera

	//invert the camera transformation to move the vertices
	g_viewMatrix = glm::inverse(cameraTransformationStack.topMatrix() );

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE,  glm::value_ptr(g_viewMatrix) );

	glUseProgram(sunShaderProgram);
	glUniformMatrix4fv(sunViewMatrixUniformLocation, 1, GL_FALSE,  glm::value_ptr(g_viewMatrix) );

	glUseProgram(orbitShaderProgram);
	glUniformMatrix4fv(orbitViewMatrixUniformLocation, 1, GL_FALSE,  glm::value_ptr(g_viewMatrix) );

	glUseProgram(skyShaderProgram);
	glUniformMatrix4fv(skyViewMatrixUniformLocation, 1, GL_FALSE,  glm::value_ptr(g_viewMatrix) );

	glUseProgram(0);
}


void drawSun()
{
	glUseProgram(sunShaderProgram);

	//scale the sun
	glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(g_sunScale) );
	//draw the geometry
	glUniform1i(sunColorUniformLocation0, 0);
	glUniform1i(sunColorUniformLocation1, 1);
	glUniform1f(sunTimeUniformLocation, g_elapsed_virtual_time);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sunTexture1);

	// transfer model matrix to shader by the associated uniform location
	glUniformMatrix4fv(sunModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(model_matrix) );

	//bind the planet geometry (the VAO!)
	glBindVertexArray(vertexArrayObject);

	// draw object according to (the EAO!). Note, that the geometry type is triangles.
	glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);

	//clear the transformation stack
	modelTransformationStack.clear();

}


void drawSkySphere()
{
	glUseProgram(skyShaderProgram);

	glUniform1i(skyColorUniformLocation, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyTexture);

	auto modelmatrix = glm::translate(glm::mat4(1.0f), glm::vec3(camera_position[0], camera_position[1], camera_position[2]) )
					 * glm::scale 	 (glm::mat4(1.0f), glm::vec3(1.0         ) ) ;
	// auto modelmatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.0, 0.0, 0.0) )

	// transfer model matrix to shader by the associated uniform location
	glUniformMatrix4fv(skyModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelmatrix) );

	// glUniform3f(skyColorUniformLocation, 0.8f, 0.8f, 0.8f);

	//calculate the normal transfomrations from modelview matrix
    glm::mat4 normalMatrix = glm::mat4(1.0f);
	normalMatrix = g_viewMatrix*modelmatrix;
	normalMatrix = glm::inverse(normalMatrix);
	normalMatrix = glm::transpose(normalMatrix);

	//bind the planet geometry (the VAO!)
	glBindVertexArray(vertexArrayObject);

	// draw object according to (the EAO!). Note, that the geometry type is triangles.
	glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);

	// clear depth buffer to make the Sky Sphere Background
	glClear(GL_DEPTH_BUFFER_BIT);
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
	glUniform3f(orbitColorUniformLocation, 0.55f, 0.68f, 1.0f);
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
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_ioTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), g_jupiterTranslate ) );
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), g_jupiterRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.84f, 0.71f, 0.53f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_europaTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), g_jupiterTranslate ) );
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), g_jupiterRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.76f, 0.58f, 0.34f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_ganymedeTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), g_jupiterTranslate ) );
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), g_jupiterRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.81f, 0.71f, 0.55f);
	glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);

	modelTransformationStack.clear();
    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(g_callistoTranslate.x)) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), g_jupiterTranslate ) );
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), g_jupiterRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
	glUniform3f(orbitColorUniformLocation, 0.76f, 0.58f, 0.34f);
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

void draw_saturn_rings()
{
	glUseProgram(orbitShaderProgram);
	glBindVertexArray(orbitVertexArrayObject);

	float saturn_ring_parts = g_num_of_saturn_rings/10;
	for(int i = 0; i < g_num_of_saturn_rings; i++)
	{
		modelTransformationStack.clear();
	    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(1.7+i*0.009)) );
	    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) ) );
	    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), 30.0f, glm::vec3(0.0f, 0.0f, 1.0f) ) );
	    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -g_saturnRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), g_saturnTranslate ) );
		modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), g_saturnRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
		glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
		if(i<saturn_ring_parts*2) glUniform3f(orbitColorUniformLocation, 0.25f, 0.23f, 0.22f);
		else if(i<saturn_ring_parts*3) glUniform3f(orbitColorUniformLocation, 0.38f, 0.35f, 0.33f);
		else if(i<saturn_ring_parts*4) glUniform3f(orbitColorUniformLocation, 0.65f, 0.58f, 0.50f);
		else if(i<saturn_ring_parts*6) glUniform3f(orbitColorUniformLocation, 0.58f, 0.51f, 0.41f);
		else if(i<saturn_ring_parts*8) glUniform3f(orbitColorUniformLocation, 0.33f, 0.29f, 0.22f);
		else if(i<saturn_ring_parts*9) glUniform3f(orbitColorUniformLocation, 0.57f, 0.51f, 0.43f);
		else glUniform3f(orbitColorUniformLocation, 0.83f, 0.73f, 0.61f);

		glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);
	}

	modelTransformationStack.clear();
}

//////////////////////////////////////////////////////////////////////////

void draw_uranus_rings()
{
	glUseProgram(orbitShaderProgram);
	glBindVertexArray(orbitVertexArrayObject);

	float uranus_ring_parts = g_num_of_uranus_rings/3;
	for(int i = 0; i < g_num_of_uranus_rings; i++)
	{
		modelTransformationStack.clear();
	    modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0), glm::vec3(1.5+i*0.009)) );
	    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), -g_uranusRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
	    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0), g_uranusTranslate ) );
		modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0), g_uranusRotation, glm::vec3(0.0f, 1.0f, 0.0f) ) );
		glUniformMatrix4fv(orbitModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelTransformationStack.topMatrix()) );
		if(i<uranus_ring_parts*1) glUniform3f(orbitColorUniformLocation, 0.13f, 0.20f, 0.35f);
		else if(i<uranus_ring_parts*2) glUniform3f(orbitColorUniformLocation, 0.08f, 0.12f, 0.23f);
		else glUniform3f(orbitColorUniformLocation, 0.05f, 0.10f, 0.19f);

		glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size()/3);
	}

	modelTransformationStack.clear();
}

//////////////////////////////////////////////////////////////////////////

void drawSolarsystem()
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	int time_difference = now - g_last_elapsed_time;
	g_last_elapsed_time = now;
	g_elapsed_virtual_time += (float)time_difference * g_speed * g_toggle;


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
	g_ioRotation = g_jupiterRotation*30.0;
	g_europaRotation = g_jupiterRotation*25.0;
	g_ganymedeRotation = g_jupiterRotation*20.0;
	g_callistoRotation = g_jupiterRotation*15.0;


    // modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_ioScale) ) );
    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_europaTranslate ) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_europaRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
    modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_jupiterTranslate ) );
    modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
    glUseProgram(orbitShaderProgram);
    glUniform3f(orbitLightPositionUniformLocation, modelTransformationStack.topMatrix()[3][0], modelTransformationStack.topMatrix()[3][1], modelTransformationStack.topMatrix()[3][2]);
	glUseProgram(shaderProgram);
    glUniform3f(lightPositionUniformLocation, modelTransformationStack.topMatrix()[3][0], modelTransformationStack.topMatrix()[3][1], modelTransformationStack.topMatrix()[3][2]);
    modelTransformationStack.clear();

	glUniform1i(planetColorUniformLocation, 0);
	glUniform1i(normalColorUniformLocation, 1);
	glUniform1i(glossColorUniformLocation, 2);
	glUniform1i(specularColorUniformLocation, 3);
	glUniform1i(displacementColorUniformLocation, 4);

	//mercury
	//scale the mercury
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_mercuryScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_mercuryRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the mercury
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_mercuryTranslate) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_mercuryRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mercuryTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planetGlossTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, planetSpecularTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, planetDisplacementTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//venus
	//scale the venus
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_venusScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_venusRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the venus
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_venusTranslate) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_venusRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, venusTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planetGlossTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, planetSpecularTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, planetDisplacementTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//earth
	//scale the earth
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_earthScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_earthRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earth
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_earthTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_earthRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planetGlossTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, planetSpecularTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, earthDisplacementTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

    //earthmoon
	//scale the earthmoon
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_earthMoonScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_earthMoonRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_earthMoonTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_earthMoonRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_earthTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_earthRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planetGlossTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, planetSpecularTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, planetDisplacementTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//mars
	//scale the mars
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_marsScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_marsRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the mars
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_marsTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_marsRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, marsTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planetGlossTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, planetSpecularTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, planetDisplacementTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//jupiter
	//scale the jupiter
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_jupiterScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the jupiter
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_jupiterTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, jupiterTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	draw_jupiter_moons();

	//saturn
	//scale the saturn
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_saturnScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_saturnRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the saturn
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_saturnTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_saturnRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, saturnTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//uranus
	//scale the uranus
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_uranusScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_uranusRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the uranus
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_uranusTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_uranusRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, uranusTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	//neptun
	//scale the neptun
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_neptunScale) ) );
	//rotate around own axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), -(float)(3*g_neptunRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the neptun
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_neptunTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_neptunRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, neptuneTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gasplanetNormalTexture);
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

	glBindVertexArray(0);
	glUseProgram(0);
}

/////////////////////////////////////////////////////////////////////////////////////////

void draw_jupiter_moons()
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonTexture);

    //jupitermoon Io
	//scale Io
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_ioScale) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_ioTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_ioRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_jupiterTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

    //jupitermoon Europa
	//scale Europa
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_europaScale) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_europaTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_europaRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_jupiterTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

    //jupitermoon Ganymede
	//scale Ganymede
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_ganymedeScale) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_ganymedeTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_ganymedeRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_jupiterTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();

    //jupitermoon Callisto
	//scale Callisto
	modelTransformationStack.pushMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(g_callistoScale) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_callistoTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_callistoRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//translate the earthmoon
	modelTransformationStack.pushMatrix(glm::translate(glm::mat4(1.0f), g_jupiterTranslate ) );
	//rotate it slowly around the y-axis
	modelTransformationStack.pushMatrix(glm::rotate(glm::mat4(1.0f), (float)(g_jupiterRotation), glm::vec3(0.0f, 1.0f, 0.0f) ) );
	//draw the geometry
    drawPlanet(modelTransformationStack.topMatrix());
	//clear the transformation stack
	modelTransformationStack.clear();
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
		float new_camera_x = camera_x + (last_mouse_x - (float)x) * g_mouse_speed;
		camera_x = new_camera_x;
	}
	last_mouse_x = x;

	if(last_mouse_y != 0)
	{
		float new_camera_y = camera_y - (last_mouse_y - (float)y) * g_mouse_speed;
		if(new_camera_y >= -90 && new_camera_y <= 90)
		{
			camera_y = new_camera_y;
		}
	}
	last_mouse_y = y;
}
/////////////////////////////////////////////////////////////////////////////////////////
void specialKeyRelease(int keyEvent, int x, int y)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
void specialKeyPress(int keyEvent, int x, int y)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
void keyRelease(unsigned char keyEvent, int x, int y)
{
	if(keyEvent == '1')
	{
		glUseProgram(shaderProgram);
		glUniform1i(shadingModeUniformLocation, 1);
	}
	if(keyEvent == '2')
	{
		glUseProgram(shaderProgram);
		glUniform1i(shadingModeUniformLocation, 2);
	}
	if(keyEvent == '7')
	{
		if(greyscale == 0)
		{
			greyscale = 1;
		}
		else
		{
			greyscale = 0;
		}
	}
	if(keyEvent == '8')
	{
		if(horizontal_mirrowed == 0)
		{
			horizontal_mirrowed = 1;
		}
		else
		{
			horizontal_mirrowed = 0;
		}
	}
	if(keyEvent == '9')
	{
		if(vertical_mirrowed == 0)
		{
			vertical_mirrowed = 1;
		}
		else
		{
			vertical_mirrowed = 0;
		}
	}
	if(keyEvent == '0')
	{
		if(blur == 0)
		{
			blur = 1;
		}
		else
		{
			blur = 0;
		}
	}
	if(keyEvent == 'r')
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
		g_key_q = false;
	}
	if(keyEvent == 'e' || keyEvent == 'E')
	{
		g_key_e = false;
	}
	if(keyEvent == 'z' || keyEvent == 'Z')
	{
		g_key_z = false;
	}
	if(keyEvent == 'h' || keyEvent == 'H')
	{
		g_key_h = false;
	}
	if(keyEvent == 'g' || keyEvent == 'G')
	{
		g_key_g = false;
	}
	if(keyEvent == 'j' || keyEvent == 'J')
	{
		g_key_j = false;
	}
	if(keyEvent == 't' || keyEvent == 'T')
	{
		g_key_t = false;
	}
	if(keyEvent == 'u' || keyEvent == 'U')
	{
		g_key_u = false;
	}
	if(keyEvent == 'x' || keyEvent == 'X')
	{
		g_speed *= 0.8;
	}
	if(keyEvent == 'c' || keyEvent == 'C')
	{
		g_speed *= 1.2;
	}
	if(keyEvent == 'o' || keyEvent == 'O')
	{
		g_draw_orbits = !g_draw_orbits;
	}
	if(glutGetModifiers() != GLUT_ACTIVE_SHIFT)
	{
		g_key_shift = false;
	}
	if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		g_key_ctrl = false;
	}
	if(keyEvent == ' ')
	{
		g_key_space = false;
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
	if(keyEvent == 'q' || keyEvent == 'Q')
	{
		g_key_q = true;
	}
	if(keyEvent == 'e' || keyEvent == 'E')
	{
		g_key_e = true;
	}
	if(keyEvent == 'z' || keyEvent == 'Z')
	{
		g_key_z = true;
	}
	if(keyEvent == 'h' || keyEvent == 'H')
	{
		g_key_h = true;
	}
	if(keyEvent == 'g' || keyEvent == 'G')
	{
		g_key_g = true;
	}
	if(keyEvent == 'j' || keyEvent == 'J')
	{
		g_key_j = true;
	}
	if(keyEvent == 't' || keyEvent == 'T')
	{
		g_key_t = true;
	}
	if(keyEvent == 'u' || keyEvent == 'U')
	{
		g_key_u = true;
	}
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	{
		g_key_shift = true;
	}
	if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
	{
		g_key_ctrl = true;
	}
	if(keyEvent == ' ')
	{
		if (g_toggle == 0.0){
			g_toggle = 1.0;
		}else{
			g_toggle = 0.0;
		}

		g_key_space = true;
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
	// LOAD, COMPILE AND LINK PLANET SHADER
	shaderProgram = glCreateProgram();
	{
		//load a shader (of the given type) and compile it in the convenience class 'Shader'
		vertexShader = Shader::loadShader("../../../data/shaders/planetVertexShader.vs", GL_VERTEX_SHADER);
		fragmentShader = Shader::loadShader("../../../data/shaders/planetFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attach the different shader components to the shader program ...
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
	}
	glLinkProgram(shaderProgram);

	    //program is linked, so we can detach compiled shaders again
		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);


	// LOAD, COMPILE AND LINK PLANET SHADER
	sunShaderProgram = glCreateProgram();
	{
		//load a shader (of the given type) and compile it in the convenience class 'Shader'
		vertexShader = Shader::loadShader("../../../data/shaders/sunVertexShader.vs", GL_VERTEX_SHADER);
		fragmentShader = Shader::loadShader("../../../data/shaders/sunFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attach the different shader components to the shader program ...
		glAttachShader(sunShaderProgram, vertexShader);
		glAttachShader(sunShaderProgram, fragmentShader);
	}
	glLinkProgram(sunShaderProgram);

	    //program is linked, so we can detach compiled shaders again
		glDetachShader(sunShaderProgram, vertexShader);
		glDetachShader(sunShaderProgram, fragmentShader);


	// LOAD, COMPILE AND LINK ORBIT SHADER
	orbitShaderProgram = glCreateProgram();
	{
		//load a shader (of the given type) and compile it in the convenience class 'Shader'
		vertexShader = Shader::loadShader("../../../data/shaders/orbitVertexShader.vs", GL_VERTEX_SHADER);
		fragmentShader = Shader::loadShader("../../../data/shaders/orbitFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attach the different shader components to the shader program ...
		glAttachShader(orbitShaderProgram, vertexShader);
		glAttachShader(orbitShaderProgram, fragmentShader);
	}
	glLinkProgram(orbitShaderProgram);

	    //program is linked, so we can detach compiled shaders again
		glDetachShader(orbitShaderProgram, vertexShader);
		glDetachShader(orbitShaderProgram, fragmentShader);


	// LOAD, COMPILE AND LINK SKYSPHERE SHADER
	skyShaderProgram = glCreateProgram();
	{
		//load a shader (of the given type) and compile it in the convenience class 'Shader'
		vertexShader = Shader::loadShader("../../../data/shaders/skyVertexShader.vs", GL_VERTEX_SHADER);
		fragmentShader = Shader::loadShader("../../../data/shaders/skyFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attach the different shader components to the shader program ...
		glAttachShader(skyShaderProgram, vertexShader);
		glAttachShader(skyShaderProgram, fragmentShader);
	}
	glLinkProgram(skyShaderProgram);

	    //program is linked, so we can detach compiled shaders again
		glDetachShader(skyShaderProgram, vertexShader);
		glDetachShader(skyShaderProgram, fragmentShader);


	// LOAD, COMPILE AND LINK SCREEN QUAD SHADER
	screenQuadShaderProgram = glCreateProgram();
	{
		//load a shader (of the given type) and compile it in the convenience class 'Shader'
		vertexShader = Shader::loadShader("../../../data/shaders/screenQuadVertexShader.vs", GL_VERTEX_SHADER);
		fragmentShader = Shader::loadShader("../../../data/shaders/screenQuadFragmentShader.fs", GL_FRAGMENT_SHADER);

		//attach the different shader components to the shader program ...
		glAttachShader(screenQuadShaderProgram, vertexShader);
		glAttachShader(screenQuadShaderProgram, fragmentShader);
	}
	glLinkProgram(screenQuadShaderProgram);

	    //program is linked, so we can detach compiled shaders again
		glDetachShader(screenQuadShaderProgram, vertexShader);
		glDetachShader(screenQuadShaderProgram, fragmentShader);




	//obtain shader variable locations
	modelMatrixUniformLocation      = glGetUniformLocation(shaderProgram, "ModelMatrix");
	viewMatrixUniformLocation       = glGetUniformLocation(shaderProgram, "ViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
	normalMatrixUniformLocation     = glGetUniformLocation(shaderProgram, "NormalMatrix");
	shadingModeUniformLocation      = glGetUniformLocation(shaderProgram, "ShadingMode");
	lightPositionUniformLocation 	= glGetUniformLocation(shaderProgram, "LightPosition");
	planetColorUniformLocation 	 	= glGetUniformLocation(shaderProgram, "ColorTexture");
	normalColorUniformLocation      = glGetUniformLocation(shaderProgram, "NormalMapTexture");
	glossColorUniformLocation       = glGetUniformLocation(shaderProgram, "GlossMapTexture");
	specularColorUniformLocation    = glGetUniformLocation(shaderProgram, "SpecularMapTexture");
	displacementColorUniformLocation    = glGetUniformLocation(shaderProgram, "DisplacementMapTexture");

	sunModelMatrixUniformLocation      = glGetUniformLocation(sunShaderProgram, "ModelMatrix");
	sunViewMatrixUniformLocation       = glGetUniformLocation(sunShaderProgram, "ViewMatrix");
	sunProjectionMatrixUniformLocation = glGetUniformLocation(sunShaderProgram, "ProjectionMatrix");
	sunColorUniformLocation0           = glGetUniformLocation(sunShaderProgram, "ColorTexture0");
	sunColorUniformLocation1           = glGetUniformLocation(sunShaderProgram, "ColorTexture1");
	sunTimeUniformLocation             = glGetUniformLocation(sunShaderProgram, "Time");

	orbitModelMatrixUniformLocation      = glGetUniformLocation(orbitShaderProgram, "ModelMatrix");
	orbitViewMatrixUniformLocation       = glGetUniformLocation(orbitShaderProgram, "ViewMatrix");
	orbitProjectionMatrixUniformLocation = glGetUniformLocation(orbitShaderProgram, "ProjectionMatrix");
	orbitColorUniformLocation            = glGetUniformLocation(orbitShaderProgram, "OrbitColor");
	orbitLightPositionUniformLocation 	 = glGetUniformLocation(orbitShaderProgram, "LightPosition");

	skyModelMatrixUniformLocation      = glGetUniformLocation(skyShaderProgram, "ModelMatrix");
	skyViewMatrixUniformLocation       = glGetUniformLocation(skyShaderProgram, "ViewMatrix");
	skyProjectionMatrixUniformLocation = glGetUniformLocation(skyShaderProgram, "ProjectionMatrix");
	skyColorUniformLocation            = glGetUniformLocation(skyShaderProgram, "ColorTexture");

	screenQuadShaderColorTextureUniformLocation = glGetUniformLocation(screenQuadShaderProgram, "ColorTexture");
	screenQuadShaderScreenDimensionsUniformLocation = glGetUniformLocation(screenQuadShaderProgram, "ScreenDimensions");
	screenQuadShaderGreyscaleUniformLocation = glGetUniformLocation(screenQuadShaderProgram, "greyscale");
	screenQuadShaderHorizontalMirrowedUniformLocation = glGetUniformLocation(screenQuadShaderProgram, "horizontal_mirrowed");
	screenQuadShaderVerticalMirrowedUniformLocation = glGetUniformLocation(screenQuadShaderProgram, "vertical_mirrowed");
	screenQuadShaderBlurUniformLocation = glGetUniformLocation(screenQuadShaderProgram, "blur");

	glUseProgram(shaderProgram);
	glUniform1i(shadingModeUniformLocation, 1);
}


/////////////////////////////////////////////////////////////////////////////////////////

void setupFrameBuffer()
{
	// Create own Framebuffer
	glGenFramebuffers(1, &renderFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

	// Generate the Color Texture 
	glGenTextures(1, &renderColorBuffer);
	glBindTexture(GL_TEXTURE_2D, renderColorBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 
				 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	// bind the Color Attachment to the Framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderColorBuffer, 0);

	// generate Depth Buffer
	glGenRenderbuffers(1, &renderDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);

	// bind the depth buffer to the Framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderDepthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


/////////////////////////////////////////////////////////////////////////////////////////

void createQuad()
{
	std::vector<float> geometry = 
	{
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,

		-1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};

	glGenVertexArrays(1, &screenQuadVAO);
	glBindVertexArray(screenQuadVAO);
	
	glGenBuffers(1, &screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);

	glBufferData(GL_ARRAY_BUFFER, 6*3* sizeof(float), &(geometry[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3,	GL_FLOAT, GL_FALSE,	sizeof(float) * 3, (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////////////////////////


void loadModel()
{
	//load a wavefront *.obj file
	gloost::ObjLoader loader("../../../data/objects/sphere1.obj");
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

  	tangents = calculateTangents();
	for(unsigned i = 0; i < mesh->getNumInterleavedPackages(); ++i)
	{
		meshInfo.push_back(mesh->getVertices()[i][0]);
		meshInfo.push_back(mesh->getVertices()[i][1]);
		meshInfo.push_back(mesh->getVertices()[i][2]);

		glm::vec3 const& normal = glm::vec3(mesh->getNormals()[i][0],mesh->getNormals()[i][1], mesh->getNormals()[i][2]);
		// meshInfo.push_back(normal[0]);
		// meshInfo.push_back(normal[1]);
		// meshInfo.push_back(normal[2]);
		meshInfo.push_back(mesh->getTexCoords()[i][0]);
		meshInfo.push_back(mesh->getTexCoords()[i][1]);
		meshInfo.push_back(tangents[i][0]);
		meshInfo.push_back(tangents[i][1]);
		meshInfo.push_back(tangents[i][2]);
		glm::vec3 bitangent = glm::cross(normal, tangents[i]);
		meshInfo.push_back(bitangent[0]);
		meshInfo.push_back(bitangent[1]);
		meshInfo.push_back(bitangent[2]);
	}

	//load data that resides in CPU RAM into video RAM.
	glBufferData(GL_ARRAY_BUFFER, //??
		sizeof(float) * meshInfo.size(), //??
		&meshInfo.front(), //??
		GL_STATIC_DRAW); //??

	//enable vertex attribute at location 0 (compare with vertex shader input)
	glEnableVertexAttribArray(0);

	//specify where to read the data for attribute at location 0
	glVertexAttribPointer(0,
		GLOOST_MESH_NUM_COMPONENTS_VERTEX,
		GL_FLOAT, //datatype
		GL_FALSE, //should the data be normalized?
		sizeof(float) * 11, //size of attribute stride for one primitive
		(GLvoid*)(0)); //offset in stride

	// //enable vertex attribute at location 1 (compare with vertex shader input)
	// glEnableVertexAttribArray(1);

	// //specifies where in the GL_ARRAY_BUFFER our data(the vertex normal) is exactly (compare with vertex shader input)
	// glVertexAttribPointer(1,
	// 	GLOOST_MESH_NUM_COMPONENTS_NORMAL,
	// 	GL_FLOAT, GL_FALSE,
	// 	sizeof(float) * 14,
	// 	(GLvoid*)(sizeof(float) * 3));

	//enable vertex attribute at location 1 (compare with vertex shader input)
	glEnableVertexAttribArray(1);

	//specifies where in the GL_ARRAY_BUFFER our data(the vertex normal) is exactly (compare with vertex shader input)
	glVertexAttribPointer(1,
		GLOOST_MESH_NUM_COMPONENTS_TEXCOORD,
		GL_FLOAT, GL_FALSE,
    sizeof(float) * 11,
    (GLvoid*)(sizeof(float) * 3));


	// enable attribute at loaction 2 (tangent)
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(2,
		GLOOST_MESH_NUM_COMPONENTS_NORMAL,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 11,
		(GLvoid*)(sizeof(float) * 5));

	//enable attribute at loaction 3 (bitangent)
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(3,
		GLOOST_MESH_NUM_COMPONENTS_NORMAL,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 11,
		(GLvoid*)(sizeof(float) * 8));


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

std::vector<glm::vec3> calculateTangents()
{
  std::vector<glm::vec3> tangents;

  std::vector<glm::vec3> t1 = std::vector<glm::vec3>(mesh->getNumInterleavedPackages(), glm::vec3(0,0,0));
  std::vector<glm::vec3> t2 = std::vector<glm::vec3>(mesh->getNumInterleavedPackages(), glm::vec3(0,0,0));

  for(gloost::TriangleFace const& tri : mesh->getTriangles())
  {
    unsigned i1 = tri.vertexIndices[0];
    unsigned i2 = tri.vertexIndices[1];
    unsigned i3 = tri.vertexIndices[2];

    glm::vec3 vert1 = glm::vec3(mesh->getVertices()[i1][0], mesh->getVertices()[i1][1], mesh->getVertices()[i1][2]); 
    glm::vec3 vert2 = glm::vec3(mesh->getVertices()[i2][0], mesh->getVertices()[i2][1], mesh->getVertices()[i2][2]); 
    glm::vec3 vert3 = glm::vec3(mesh->getVertices()[i3][0], mesh->getVertices()[i3][1], mesh->getVertices()[i3][2]); 

    glm::vec3 hor = vert2 - vert1;
    glm::vec3 vert = vert3 - vert1;

    glm::vec2 tex1 = glm::vec2(mesh->getTexCoords()[i1][0], mesh->getTexCoords()[i1][1]);
    glm::vec2 tex2 = glm::vec2(mesh->getTexCoords()[i2][0], mesh->getTexCoords()[i2][1]);
    glm::vec2 tex3 = glm::vec2(mesh->getTexCoords()[i3][0], mesh->getTexCoords()[i3][1]);

    //horizontal vector
    glm::vec2 s = tex2 - tex1;
    //vertical vector
    glm::vec2 t = tex3 - tex1;

    float divisor = 1.0f / (s[0] * t[1] - s[1] * t[0]);

    glm::vec3 sDir = glm::vec3(t[1] * hor[0] - t[0] * vert[0], t[1] * hor[1] - t[0] * vert[1], t[1] * hor[2] - t[0] * vert[2]);
    sDir *= divisor;
    glm::vec3 tDir = glm::vec3(s[0] * vert[0] - s[1] * hor[0], s[0] * vert[1] - s[1] * hor[1], s[0] * vert[2] - s[1] * hor[2]);
    tDir *= divisor;

    // Vector3D sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
    // Vector3D tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,(s1 * z2 - s2 * z1) * r);

    t1[i1] = t1[i1] + sDir;
    t1[i2] = t1[i2] + sDir;
    t1[i3] = t1[i3] + sDir;
    t2[i1] = t2[i1] + tDir;
    t2[i2] = t2[i2] + tDir;
    t2[i3] = t2[i3] + tDir;
  }

  for(unsigned i = 0; i < mesh->getNumInterleavedPackages(); ++i)
  {
    glm::vec3 const& normal = glm::vec3(mesh->getNormals()[i][0],mesh->getNormals()[i][1], mesh->getNormals()[i][2]);
    //orthogonalize
    glm::vec3 tangent = (t1[i] - normal) * glm::normalize(glm::dot(normal, t1[i]));

    // correct handedness
    if(glm::dot(glm::cross(normal, t1[i]), t2[i]) < 0.0F)
    {
      tangent = -tangent;
    }

    tangents.push_back(tangent);
  }

  return tangents;
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

void loadTextures()
{
	// Sky Sphere Texture
	TextureLoader::loadImageToGLTexture(skyTexture, "../../../data/textures/suncore.jpg", GL_RGB8, GL_TEXTURE0);
	// TextureLoader::loadImageToGLTexture(skyTexture, "../../../data/textures/spaaace.jpg", GL_RGB8, GL_TEXTURE0);


	// Sun Textures
	TextureLoader::loadImageToGLTexture(sunTexture0, "../../../data/textures/suncore.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(sunTexture1, "../../../data/textures/sunmap.jpg", GL_RGB8, GL_TEXTURE1);

	// Earth Textures
	TextureLoader::loadImageToGLTexture(earthTexture, "../../../data/textures/earth_texture.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(earthNormalTexture, "../../../data/textures/earth_normal.jpg", GL_RGB8, GL_TEXTURE1);
	TextureLoader::loadImageToGLTexture(earthGlossTexture, "../../../data/textures/earth_gloss.jpg", GL_RGB8, GL_TEXTURE2);
	TextureLoader::loadImageToGLTexture(earthSpecularTexture, "../../../data/textures/earth_gloss.jpg", GL_RGB8, GL_TEXTURE3);
	TextureLoader::loadImageToGLTexture(earthDisplacementTexture, "../../../data/textures/earth_displacement.jpg", GL_RGB8, GL_TEXTURE4);

	// Color Textures
	TextureLoader::loadImageToGLTexture(neptuneTexture, "../../../data/textures/neptunemap.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(uranusTexture, "../../../data/textures/uranusmap.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(saturnTexture, "../../../data/textures/saturnmap.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(jupiterTexture, "../../../data/textures/jupitermap.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(marsTexture, "../../../data/textures/mars_1k_color.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(venusTexture, "../../../data/textures/venusmap.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(mercuryTexture, "../../../data/textures/mercurymap.jpg", GL_RGB8, GL_TEXTURE0);
	TextureLoader::loadImageToGLTexture(moonTexture, "../../../data/textures/moonmap.jpg", GL_RGB8, GL_TEXTURE0);

	// Normal Textures
	TextureLoader::loadImageToGLTexture(planetNormalTexture, "../../../data/textures/planet_normalmap.jpg", GL_RGB8, GL_TEXTURE1);
	TextureLoader::loadImageToGLTexture(gasplanetNormalTexture, "../../../data/textures/gas_planet_normalmap.jpg", GL_RGB8, GL_TEXTURE1);

	// Displacement Textures
	TextureLoader::loadImageToGLTexture(planetDisplacementTexture, "../../../data/textures/planet_normalmap.jpg", GL_RGB8, GL_TEXTURE1);

	// Gloss Textures
	TextureLoader::loadImageToGLTexture(planetGlossTexture, "../../../data/textures/planet_gloss.jpg", GL_RGB8, GL_TEXTURE2);

	// Specular Textures
	TextureLoader::loadImageToGLTexture(planetSpecularTexture, "../../../data/textures/planet_gloss.jpg", GL_RGB8, GL_TEXTURE2);
}


/////////////////////////////////////////////////////////////////////////////////////////


void cleanup()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);
	glDeleteProgram(sunShaderProgram);
	glDeleteProgram(orbitShaderProgram);
	glDeleteProgram(skyShaderProgram);

	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &elementArrayBuffer);

	glDeleteBuffers(1, &orbitVertexArrayObject);
	glDeleteBuffers(1, &orbitElementArrayBuffer);

	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteVertexArrays(1, &orbitVertexBufferObject);
}


/////////////////////////////////////////////////////////////////////////////////////////


void idleFunction(void)
{
	glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////////////
void resizeFBOtextures()
{
	glBindTexture(GL_TEXTURE_2D, renderColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	
	glBindTexture(GL_TEXTURE_2D, renderDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);
}
/////////////////////////////////////////////////////////////////////////////////////////


void resizeFunction(int Width, int Height)
{
	windowWidth = Width;
	windowHeight = Height;
	glViewport(0, 0, windowWidth, windowHeight);

	                           //create a projection matrix
	glm::mat4 projectionMatrix = glm::perspective(60.0f, //FOV 60.0°
		                                         (float)windowWidth/windowHeight, //aspect ratio of the projection
												 0.2f, //near clipping plane
												 100.0f); //far clipping plane

	glUseProgram(shaderProgram); //bind shader program
	//upload projection matrix to the shader.
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix) ); //upload projection matrix to bound shader

	glUseProgram(sunShaderProgram); //bind shader program
	//upload projection matrix to the shader.
	glUniformMatrix4fv(sunProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix) ); //upload projection matrix to bound shader

	glUseProgram(orbitShaderProgram); //bind shader program
	//upload projection matrix to the shader.
	glUniformMatrix4fv(orbitProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix) ); //upload projection matrix to bound shader

	glUseProgram(skyShaderProgram); //bind shader program
	//upload projection matrix to the shader.
	glUniformMatrix4fv(skyProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix) ); //upload projection matrix to bound shader

	glUseProgram(0); //unbind shader program
	resizeFBOtextures();
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
	glClearColor(0.07f, 0.07f, 0.07f, 0.0f);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//create shaders
	setupShader();
	setupFrameBuffer();
	//load and creating geometry
	createQuad();
	loadModel();
	generateOrbit();
	// load Textures
	loadTextures();
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
