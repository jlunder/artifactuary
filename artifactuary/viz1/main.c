/**
 * OpenGL ES 2.0 - Example 01
 *
 * @author	Norbert Nopper norbert@nopper.tv
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <stdio.h>

#include "GLES2/glus.h"


static GLUSshaderprogram g_program;

static GLint g_viewProjectionMatrixLocation;
static GLint g_modelMatrixLocation;

static GLint g_vertexLocation;
static GLint g_colorLocation;
static GLint g_texCoordLocation;

static GLuint g_verticesVBO;
static GLuint g_colorsVBO;
static GLuint g_texCoordsVBO;
static GLuint g_indicesVBO;

static GLUSshaderprogram g_trivialProgram;
static GLint g_trivialVertexLocation;
static GLuint g_trivialVerticesVBO;

//

static GLuint g_numberIndices;


/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;
    
    glusLoadTextFile("light.vert.glsl", &vertexSource);
    glusLoadTextFile("light.frag.glsl", &fragmentSource);
    glusBuildProgramFromSource(&g_program, (const GLUSchar**)&vertexSource.text, (const GLUSchar**)&fragmentSource.text);
    //printf("Vertex shader:\n%s\n--------\n", vertexSource.text);
    //printf("Fragment shader:\n%s\n--------\n", fragmentSource.text);
    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);
    
    g_viewProjectionMatrixLocation = glGetUniformLocation(g_program.program, "u_viewProjectionMatrix");
    g_modelMatrixLocation = glGetUniformLocation(g_program.program, "u_modelMatrix");
    
    printf("uniforms: %d %d\n", g_viewProjectionMatrixLocation, g_modelMatrixLocation);
    
    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    g_colorLocation = glGetAttribLocation(g_program.program, "a_color");
    g_texCoordLocation = glGetAttribLocation(g_program.program, "a_texCoord");
    
    printf("attributes: %d %d %d\n", g_vertexLocation, g_colorLocation, g_texCoordLocation);
    
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClearDepthf(1.0f);
    
    /*
    GLfloat lightVertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f,
    };
    
    GLfloat lightColors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };
    
    GLfloat lightTexCoords[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    
    GLushort lightIndices[] = {0, 1, 2, 3};
    
    g_numberIndices = 4;
    
    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof (GLfloat), (GLfloat*)lightVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &g_colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof (GLfloat), (GLfloat*)lightColors, GL_STATIC_DRAW);
    
    glGenBuffers(1, &g_texCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof (GLfloat), (GLfloat*)lightTexCoords, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &g_indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_numberIndices * sizeof (GLushort), (GLuint*)lightIndices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    printf("Buffers: %d %d %d %d\n", g_verticesVBO, g_colorsVBO, g_texCoordsVBO, g_indicesVBO);
    
    glUseProgram(g_program.program);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_colorsVBO);
    glVertexAttribPointer(g_colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glVertexAttribPointer(g_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
*/


    glusLoadTextFile("trivial.vert.glsl", &vertexSource);
    glusLoadTextFile("trivial.frag.glsl", &fragmentSource);
    glusBuildProgramFromSource(&g_trivialProgram, (const GLUSchar**)&vertexSource.text, (const GLUSchar**)&fragmentSource.text);
    printf("Vertex shader:\n%s\n--------\n", vertexSource.text);
    printf("Fragment shader:\n%s\n--------\n", fragmentSource.text);
    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);
    
    g_trivialVertexLocation = glGetAttribLocation(g_trivialProgram.program, "a_vertex");
    printf("a_vertex: %d\n", g_trivialVertexLocation);
    
    GLfloat trivialVertices[] = {
        0.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
    };
    
    glGenBuffers(1, &g_trivialVerticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_trivialVerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof (GLfloat), (GLfloat*)trivialVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(g_trivialVertexLocation, 4, GL_FLOAT, GL_FALSE, 4 * sizeof (GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    
/*    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslate(0.0f, 0.0f, -8.0f);
*/
    return GLUS_TRUE;
}

/**
 * Function is called before first update and every time when the window is resized.
 *
 * @param w	width of the window
 * @param h	height of the window
 */
GLUSvoid reshape(GLUSint width, GLUSint height)
{
    GLfloat viewMatrix[16];
    GLfloat viewProjectionMatrix[16];
    
    // Set the viewport depending on the width and height of the window.
    glViewport(0, 0, width, height);
    
    glusLookAtf(viewMatrix,
        0.0f, 0.0f, 8.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glusPerspectivef(viewProjectionMatrix, 45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
    glusMatrix4x4Multiplyf(viewProjectionMatrix, viewProjectionMatrix, viewMatrix);
    
    glUniformMatrix4fv(g_viewProjectionMatrixLocation, 1, GL_FALSE, viewProjectionMatrix);
}

/**
 * Function to render and display content. Swapping of the buffers is automatically done.
 *
 * @return true for continuing, false to exit the application
 */
GLUSboolean update(GLUSfloat time)
{
    GLfloat modelMatrix[16];
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*
    glUseProgram(g_program.program);
    
    glusMatrix4x4Identityf(modelMatrix);
    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
    
    glEnableVertexAttribArray(g_vertexLocation);
    glEnableVertexAttribArray(g_colorLocation);
    glEnableVertexAttribArray(g_texCoordLocation);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
    
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(g_vertexLocation);
    glDisableVertexAttribArray(g_colorLocation);
    glDisableVertexAttribArray(g_texCoordLocation);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    */
    
    glUseProgram(g_trivialProgram.program);
    glEnableVertexAttribArray(g_trivialVertexLocation);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(g_trivialVertexLocation);
    
    
    return GLUS_TRUE;
}

/**
 * Function to clean up things.
 */
GLUSvoid terminate(GLUSvoid)
{
}

/**
 * Main entry point.
 */
int main(int argc, char* argv[])
{
    EGLint eglAttributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
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
