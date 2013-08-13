#include "gles2_harness.h"

#include "GLES2/glus.h"


GLUSboolean gles2_harness_init(GLUSvoid);
GLUSvoid gles2_harness_reshape(GLUSint width, GLUSint height);
GLUSboolean gles2_harness_update(GLUSfloat time);
GLUSvoid gles2_harness_terminate(GLUSvoid);


static GLUSshaderprogram g_program;

static GLint g_viewProjectionMatrixLocation;
static GLint g_modelMatrixLocation;
static GLint g_colorLocation;

static GLint g_vertexLocation;

static GLuint g_verticesVBO;

static GLfloat g_aspectRatio;


GLUSboolean gles2_harness_init(GLUSvoid)
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
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    
    artifactuary_init();
    
    return GLUS_TRUE;
}


GLUSvoid gles2_harness_reshape(GLUSint width, GLUSint height)
{
    // Set the viewport depending on the width and height of the window.
    glViewport(0, 0, width, height);
    
    g_aspectRatio = (GLfloat)width / (GLfloat)height;
}


GLUSboolean gles2_harness_update(GLUSfloat time)
{
    GLfloat viewMatrix[16];
    GLfloat viewProjectionMatrix[16];
    GLfloat modelMatrix[16];
    
    struct timespec process_start_time;
    struct timespec process_end_time;
    int64_t process_nsec;
    
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_start_time);
    artifactuary_process(time);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &process_end_time);
    
    process_nsec = process_end_time.tv_nsec + (int64_t)process_end_time.tv_sec * 1000000000 -
        (process_start_time.tv_nsec + (int64_t)process_start_time.tv_sec * 1000000000);
    
    printf("frame time: %7.3fms/%7.3fms\n", (double)process_nsec * 1.0e-6, time * 1000.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glusLookAtf(viewMatrix,
        0.0f, 0.0f, 10.0f,
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
    
    for(int j = 0; j < ARRAY_HEIGHT; ++j) {
        for(int i = 0; i < ARRAY_WIDTH; ++i) {
            glusMatrix4x4Identityf(modelMatrix);
            glusMatrix4x4Translatef(modelMatrix, 0.1f * (i - 0.5f * ARRAY_WIDTH), 0.1f * (-j + 0.5f * ARRAY_HEIGHT), 0.0f);
            glusMatrix4x4Scalef(modelMatrix, 0.05f, 0.05f, 1.0f);
            glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
            
            glUniform4f(g_colorLocation,
                array_data[j][i].c.r * (1.0f / 255.0f),
                array_data[j][i].c.g * (1.0f / 255.0f),
                array_data[j][i].c.b * (1.0f / 255.0f),
                0.0f);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
    
    glDisableVertexAttribArray(g_vertexLocation);
    
    glUseProgram(0);
    
    return GLUS_TRUE;
}


GLUSvoid gles2_harness_terminate(GLUSvoid)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &g_verticesVBO);
    
    glUseProgram(0);
    
    glusDestroyProgram(&g_program);
}


void gles2_harness_main(void)
{
    EGLint eglAttributes[] = {
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE, 5,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    
    glusInitFunc(gles2_harness_init);
    
    glusReshapeFunc(gles2_harness_reshape);
    
    glusUpdateFunc(gles2_harness_update);
    
    glusTerminateFunc(gles2_harness_terminate);
    
    if (!glusCreateWindow("Artifactuary GLES2 Harness", 1280, 720, eglAttributes, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return;
    }
    
    glusRun();
}

