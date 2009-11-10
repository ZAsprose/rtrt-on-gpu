#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <GLee.h>
#include <GL/glfw.h>
#include <CL/cl.h>
#include <ShaderManager.h>

using namespace render;

void Init();
void Shut_Down(int return_code);
void Main_Loop();
void Draw();
void InitShader();

float rotate_y = 0,
      rotate_z = 0;
const float rotations_per_tick = .2;

char *VertexShaderSource,*FragmentShaderSource;

int VertexShader,FragmentShader;

int ShaderProgram;

GLfloat angle = 0.0;

ShaderManager * manager = NULL;

int main()
{
  Init();
  Main_Loop();
  Shut_Down(0);
}

void Init()
{
  const int window_width = 800,
            window_height = 600;

  if (glfwInit() != GL_TRUE)
    Shut_Down(1);
  // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
  if (glfwOpenWindow(window_width, window_height, 5, 6, 5,
                     0, 0, 0, GLFW_WINDOW) != GL_TRUE)
    Shut_Down(1);
  glfwSetWindowTitle("The GLFW Window");

  // set the projection matrix to a normal frustum with a max depth of 50
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect_ratio = ((float)window_height) / window_width;
  glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
  glMatrixMode(GL_MODELVIEW);

  InitShader ( );
}

void InitOpenCl ( void )
{
	cl_int clerr;

	cl_uint numplatforms;

	cl_platform_id * platformlist;

	clerr=clGetPlatformIDs ( 0, NULL, &numplatforms );

	platformlist = (cl_platform_id *) malloc(sizeof(cl_platform_id)*numplatforms);

	clerr=clGetPlatformIDs(numplatforms, platformlist, NULL);

	cl_uint i;

	for (i=0; i<numplatforms; i++)
	{
	  char platname[80];

	  clerr=clGetPlatformInfo ( platformlist [i],
							    CL_PLATFORM_NAME,
							    sizeof(platname),
							    (void *) platname,
							    NULL );

	  char platprofile[80];

	  clerr = clGetPlatformInfo ( platformlist [i],
							      CL_PLATFORM_PROFILE,
							      sizeof ( platprofile ),
							      (void *) platprofile,
							      NULL );

	  char platvendor[80];

	  clerr = clGetPlatformInfo ( platformlist [i],
							      CL_PLATFORM_VENDOR,
							      sizeof ( platvendor ),
							      (void *) platvendor,
							      NULL );

	  cl_uint numdevs;

	  clerr = clGetDeviceIDs ( platformlist [i],
						       CL_DEVICE_TYPE_ALL,
						       0,
						       NULL,
						       &numdevs );

	  char platforminfo[4096];

	  sprintf ( platforminfo, "OpenCL Platform[%d]: %s, %s  Devices: %u\n",
			    i, platname, platprofile, numdevs);

	  std::cout << platforminfo << std::endl;

	}

	free ( platformlist );
}

void InitShader ( void )
{
	manager = new ShaderManager();

	if (!manager->LoadVertexShader("Vertex.vert")) exit(-1);

	if (!manager->LoadFragmentShader("Fragment.frag")) exit(-1);

	if (!manager->BuildProgram()) exit(-1);

	manager->Bind();

	InitOpenCl ( );
}

void Shut_Down(int return_code)
{
  glfwTerminate();
  exit(return_code);
}

void Main_Loop()
{
  // the time of the previous frame
  double old_time = glfwGetTime();
  // this just loops as long as the program runs
  while(1)
  {
    // calculate time elapsed, and the amount by which stuff rotates
    double current_time = glfwGetTime(),
           delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
    old_time = current_time;
    // escape to quit, arrow keys to rotate view
    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
      break;
    if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
      rotate_y += delta_rotate;
    if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
      rotate_y -= delta_rotate;
    // z axis always rotates
    rotate_z += delta_rotate;

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the figure
    Draw();
    // swap back and front buffers
    glfwSwapBuffers();
  }
}

void Draw_Square(float red, float green, float blue)
{
  // Draws a square with a gradient color at coordinates 0, 10
  glBegin(GL_QUADS);
  {
    glColor3f(red, green, blue);
    glVertex2i(1, 11);
    glColor3f(red * .8, green * .8, blue * .8);
    glVertex2i(-1, 11);
    glColor3f(red * .5, green * .5, blue * .5);
    glVertex2i(-1, 9);
    glColor3f(red * .8, green * .8, blue * .8);
    glVertex2i(1, 9);
  }
  glEnd();
}

void Draw()
{
  // reset view matrix
  glLoadIdentity();
  // move view back a bit
  glTranslatef(0, 0, -30);
  // apply the current rotation
  glRotatef(rotate_y, 0, 1, 0);
  glRotatef(rotate_z, 0, 0, 1);
  // by repeatedly rotating the view matrix during drawing, the
  // squares end up in a circle
  int i = 0, squares = 15;
  float red = 0, blue = 1;
  for (i = 0; i < squares; ++i)
  {
    glRotatef(360.0/squares, 0, 0, 1);
    // colors change for each square
    red += 1.0/12;
    blue -= 1.0/12;
    Draw_Square(red, .6, blue);
  }
}
