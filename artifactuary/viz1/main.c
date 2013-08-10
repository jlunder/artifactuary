#include <stdio.h>
#include <stdint.h>

#include "GLES2/glus.h"


static GLUSshaderprogram g_program;

static GLint g_viewProjectionMatrixLocation;
static GLint g_modelMatrixLocation;
static GLint g_colorLocation;

static GLint g_vertexLocation;

static GLuint g_verticesVBO;

static GLfloat g_aspectRatio;


#define ARRAY_WIDTH 110
#define ARRAY_HEIGHT 18


uint8_t array_colors[ARRAY_HEIGHT][ARRAY_WIDTH][3];


void process(void)
{
    for(int j = 0; j < ARRAY_HEIGHT; ++j) {
        for(int i = 0; i < ARRAY_WIDTH; ++i) {
            array_colors[j][i][0] = i * 256 / ARRAY_WIDTH;
            array_colors[j][i][1] = j * 256 / ARRAY_HEIGHT;
            array_colors[j][i][2] = rand() % 256;
        }
    }
}


GLUSboolean init(GLUSvoid)
{
    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;
    
    glusLoadTextFile("light.vert.glsl", &vertexSource);
    glusLoadTextFile("light.frag.glsl", &fragmentSource);
    glusBuildProgramFromSource(&g_program, (const GLUSchar**)&vertexSource.text, (const GLUSchar**)&fragmentSource.text);
    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);
    
    g_viewProjectionMatrixLocation = glGetUniformLocation(g_program.program, "u_viewProjectionMatrix");
    g_modelMatrixLocation = glGetUniformLocation(g_program.program, "u_modelMatrix");
    g_colorLocation = glGetUniformLocation(g_program.program, "u_color");
    
    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    
    //glEnable(GL_DEPTH_TEST);
    
    // premultiplied alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    //glClearDepthf(1.0f);
    
    GLfloat lightVertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f,
    };
    
    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof (GLfloat), (GLfloat*)lightVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return GLUS_TRUE;
}


GLUSvoid reshape(GLUSint width, GLUSint height)
{
    // Set the viewport depending on the width and height of the window.
    glViewport(0, 0, width, height);
    
    g_aspectRatio = (GLfloat)width / (GLfloat)height;
}


GLUSboolean update(GLUSfloat time)
{
    GLfloat viewMatrix[16];
    GLfloat viewProjectionMatrix[16];
    GLfloat modelMatrix[16];
    
    
    process();
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glusLookAtf(viewMatrix,
        0.0f, 0.0f, 8.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glusPerspectivef(viewProjectionMatrix, 45.0f, g_aspectRatio, 0.1f, 1000.0f);
    glusMatrix4x4Multiplyf(viewProjectionMatrix, viewProjectionMatrix, viewMatrix);
    
    
    glUseProgram(g_program.program);
    
    
    glUniformMatrix4fv(g_viewProjectionMatrixLocation, 1, GL_FALSE, viewProjectionMatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //
    glusMatrix4x4Identityf(modelMatrix);
    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
    
    glUniform4f(g_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //
    
    glDisableVertexAttribArray(g_vertexLocation);
    
    glUseProgram(0);
    
    return GLUS_TRUE;
}


GLUSvoid terminate(GLUSvoid)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &g_verticesVBO);
    
    glUseProgram(0);
    
    glusDestroyProgram(&g_program);
}


int main(int argc, char* argv[])
{
    EGLint eglAttributes[] = {
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE, 5,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    
    
    glusInitFunc(init);

    glusReshapeFunc(reshape);

    glusUpdateFunc(update);

    glusTerminateFunc(terminate);

    if (!glusCreateWindow("GLUS Example Window", 640, 480, eglAttributes, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusRun();

    return 0;
}
