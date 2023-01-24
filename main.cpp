//----------------------------------------------------------------------------------------
/**
 * \file       main.cpp
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Main file.
 *
 *  Contains all callbacks and global variables.
 *
*/
//----------------------------------------------------------------------------------------

#include <iostream>

#include "pgr.h"
#include "source/Scene.h"

/// Scene object that keeps all the object
Scene scene;

/// Main Camera
Camera camera = Camera(70.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 1000.0f, glm::vec3(0.0f, 25.0f, 150.0f), glm::vec3(0.0f, 0.0f, -1.0f), 1.0f);

/// Global Variables
GLuint shaderProgram = 0;

/// Boolean array containing infrormation whether the key is pressed
bool keystates[256];

/// Load Shaders
bool loadShaders()
{
  GLuint shaders[] = {
      pgr::createShaderFromFile(GL_VERTEX_SHADER, vertexShaderPath),
      pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderPath),
      0
  };

  shaderProgram = pgr::createProgram(shaders);

  if (shaders[0] == 0 || shaders[1] == 0 || shaderProgram == 0)
  {
    std::cout << "Failed to load shader file" << std::endl;
    return false;
  }

  return true;
}

/// Callback on each frame
void drawCallback()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);

  camera.draw();
  scene.draw();
  
  CHECK_GL_ERROR();
  glutSwapBuffers();
}


void keyboardDownCallback(unsigned char key, int mouseX, int mouseY)
{
  switch (key)
  {
  case ESC_BUTTON:
    glutLeaveMainLoop();
    break;

  case UP_KEY:
    camera.move(Camera::Direction::UP);
    break;

  case DOWN_KEY:
    camera.move(Camera::Direction::DOWN);
    break;

  case 'f':
    scene.switchFlashLight();
    break;

  case 'g':
    scene.switchFog();
    break;

  case 'z':
    keystates['z'] = true;
    break;

  case '1':
    keystates['1'] = true;
    break;

  case '2':
    keystates['2'] = true;
    break;

  case '3':
    keystates['3'] = true;
    break;

  case '0':
    camera.unlockView();
    break;

  case '+':
    camera.startAnimation();
    break;

  }

  /// Key combinations
  if (keystates['z'] && keystates['1'])
    camera.switchStaticPosition(1);
  if (keystates['z'] && keystates['2'])
    camera.switchStaticPosition(2);
  if (keystates['z'] && keystates['3'])
    camera.switchStaticPosition(3);
}

void keyboardUpCallback(unsigned char keyPressed, int mouseX, int mouseY)
{
  switch (keyPressed)
  {
  case 'z':
    keystates['z'] = false;
    break;

  case '1':
    keystates['1'] = false;
    break;

  case '2':
    keystates['2'] = false;
    break;

  case '3':
    keystates['3'] = false;
    break;
  }
}

/// Keyboard event with arrow keys
void keyboardSpecialCallback(int key, int mouseX, int mouseY)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    camera.move(Camera::Direction::FORWARD);
    break;

  case GLUT_KEY_DOWN:
    camera.move(Camera::Direction::BACKWARD);
    break;

  case GLUT_KEY_LEFT:
    camera.move(Camera::Direction::LEFT);
    break;

  case GLUT_KEY_RIGHT:
    camera.move(Camera::Direction::RIGHT);
    break;

  default:
    break;
  }
}


void mouseClickCallback(int button, int state, int mouseX, int mouseY)
{

  if (state == GLUT_UP)
  {
    /// Find the object id by clicking
    int objectId = 404;
    glReadPixels(mouseX, WINDOW_HEIGHT - mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &objectId);
    std::cout << "Selected: " << objectId << std::endl;

    if (objectId == 258)
    {
      scene.pushDoor();
      camera.disableCollision();
    }
    else if (objectId == 262)
    {
      scene.touchMouse();
    }
  }
}

/// OnMouseMove. Calculate the vector of movement, rotate the camera and set curson on center
void mousePassiveCallback(int mouseX, int mouseY)
{
  int centerX = WINDOW_WIDTH / 2;
  int centerY = WINDOW_HEIGHT / 2;

  float deltaX = (mouseX - centerX) * mouseSensitivity;
  float deltaY = (centerY - mouseY) * mouseSensitivity;

  camera.rotate(deltaX, deltaY);
  glutWarpPointer(centerX, centerY);
}

void timerCallback(int time)
{
  glutPostRedisplay();
  glutTimerFunc(timerDelay, timerCallback, 0);
}

/// Simple GUI
void menuCallback(int menuId)
{
  switch (menuId)
  {
  case 1:
    camera.startAnimation();
    break;

  case 2:
    camera.switchStaticPosition(1);
    break;

  case 3:
    camera.switchStaticPosition(2);
    break;

  case 4:
    camera.switchStaticPosition(3);
    break;
  default:
    break;
  }
}

void init()
{
  glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

  if (!loadShaders()) 
    std::cout << "Shaders are not loaded" << std::endl;
  
  camera.init(shaderProgram);
  CHECK_GL_ERROR();
  scene.init(shaderProgram);
  CHECK_GL_ERROR();
}


int main(int argc, char* argv[]) 
{
  glutInit(&argc, argv);

  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow(WINDOW_TITLE);

  glutKeyboardFunc(keyboardDownCallback);
  glutKeyboardUpFunc(keyboardUpCallback);
  glutSpecialFunc(keyboardSpecialCallback);
  glutDisplayFunc(drawCallback);
  glutMouseFunc(mouseClickCallback);
  glutPassiveMotionFunc(mousePassiveCallback);

  glutCreateMenu(menuCallback);
  glutAddMenuEntry("Start Animation", 1);
  glutAddMenuEntry("Static Position 1", 2);
  glutAddMenuEntry("Static Position 2", 3);
  glutAddMenuEntry("Static Position 3", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutTimerFunc(timerDelay, timerCallback, 0);

  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");
  scene.loadObjects();

  init();
  glutMainLoop();
  return 0;
}
