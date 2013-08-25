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

int32_t gles2_harness_light_index = 0;
int64_t gles2_harness_total_nsec = 0;


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
    
    GLfloat pixel_spacing = 0.1f;
    GLfloat array_spacing = 0.5f;
    GLfloat arrays_total_width;
    GLfloat arrays_total_height;
    GLfloat arrays_cur_x = 0.0f;
    
    int64_t frame_nsec = (int64_t)round(time * 1.0e9);
    
    
    artifactuary_process(gles2_harness_total_nsec, frame_nsec);
    
    gles2_harness_total_nsec += frame_nsec;
    
    artifactuary_array_data[artifactuary_array_data_mapping[gles2_harness_light_index]].c.r = 0;
    artifactuary_array_data[artifactuary_array_data_mapping[gles2_harness_light_index]].c.g = 0;
    artifactuary_array_data[artifactuary_array_data_mapping[gles2_harness_light_index]].c.b = 255;
    gles2_harness_light_index = (gles2_harness_light_index + 1) % ARTIFACTUARY_NUM_PIXELS;
    
    
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
    
    
    // compute size of the non-cityscape panels -- this will be the top row
    arrays_total_width = -array_spacing;
    arrays_total_height = 0.0f;
    
    //assert(ARTIFACTUARY_BACKDROP == 0);
    //for(int32_t k = 1; k < ARTIFACTUARY_NUM_ARRAYS; ++k) {
    for(int32_t k = 0; k < ARTIFACTUARY_NUM_ARRAYS; ++k) {
        GLfloat array_width = artifactuary_arrays[k].width * pixel_spacing;
        GLfloat array_height = artifactuary_arrays[k].height * pixel_spacing;
        
        arrays_total_width += array_width + array_spacing;
        if(array_height > arrays_total_height) {
            arrays_total_height = array_height;
        }
    }
    
    // add the height of the cityscape panel
    //arrays_total_height += array_spacing + ARTIFACTUARY_BACKDROP_HEIGHT * pixel_spacing;
    
    for(int32_t k = 0; k < ARTIFACTUARY_NUM_ARRAYS; ++k) {
        GLfloat base_x = arrays_total_width * -0.5f;
        GLfloat base_y = arrays_total_height * -0.5f;
        array_t* array = &artifactuary_arrays[k];
        
        /*
        if(k == ARTIFACTUARY_BACKDROP) {
            base_x = ARTIFACTUARY_BACKDROP_WIDTH * pixel_spacing * -0.5f;
            base_y += arrays_total_height - ARTIFACTUARY_BACKDROP_HEIGHT * pixel_spacing;
        }
        else {
        */
            base_x += arrays_cur_x;
        /*
        }
        */
        
        for(int32_t j = 0, data_pos = 0; j < array->height; ++j) {
            for(int32_t i = 0; i < array->width; ++i, ++data_pos) {
                glusMatrix4x4Identityf(modelMatrix);
                glusMatrix4x4Translatef(modelMatrix, base_x + i * pixel_spacing, base_y + (array->height - 1 - j) * pixel_spacing, 0.0f);
                glusMatrix4x4Scalef(modelMatrix, pixel_spacing * 0.5f, pixel_spacing * 0.5f, 1.0f);
                glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
                
                glUniform4f(g_colorLocation,
                    array->data[data_pos].c.r * (1.0f / 255.0f),
                    array->data[data_pos].c.g * (1.0f / 255.0f),
                    array->data[data_pos].c.b * (1.0f / 255.0f),
                    0.0f);
                
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }
        /*
        if(k != ARTIFACTUARY_BACKDROP) {
        */
            arrays_cur_x += array->width * pixel_spacing + array_spacing;
        /*
        }
        */
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

