#define GLFW_DLL
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <cstdio>

GLFWwindow* initWindow(const int resX, const int resY)
{
    if (!glfwInit()) { fprintf(stderr, "Failed to initialize GLFW\n"); return NULL; }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);    // Open a window and create its OpenGL context
    if (window == NULL) { fprintf(stderr, "Failed to open GLFW window.\n"); glfwTerminate(); return NULL; }
    glfwMakeContextCurrent(window);

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}

void drawCube()
{
    GLfloat vertices[] =
    {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
        1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

    static float angle = 0;
    glRotatef(angle, 0, 1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_QUADS, 0, 24);
   
    glDisableClientState(GL_VERTEX_ARRAY); // Cleanup states 
    angle += 1;
}

void display(GLFWwindow* window)
{
    while (!glfwWindowShouldClose(window))
    {
        // Scale to window size
        GLint windowWidth, windowHeight; glfwGetWindowSize(window, &windowWidth, &windowHeight); glViewport(0, 0, windowWidth, windowHeight);

        // Draw stuff
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0, 0, -5);

        drawCube();

        glfwSwapBuffers(window);  // Update Screen
        glfwPollEvents(); // Check for any input, or window movement
    }
}
/*
int main(int argc, char** argv)
{
    GLFWwindow* window = initWindow(1024, 620);
    if (NULL != window)
    {
        display(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}*/