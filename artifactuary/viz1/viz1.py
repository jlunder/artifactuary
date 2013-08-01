import sys

import pygame
from pygame.locals import *

import math, random


array_width = 110
array_height = 18

light_colors = []

class FeedbackFireEffect:
    def __init__(self):
        def bilinear_palette(start_color, end_color, steps):
            for i in range(steps):
                self.palette.append(
                    (float(start_color[0] * (steps - (i + 1))) / steps +
                     float(end_color[0] * (i + 1)) / steps,
                     
                     float(start_color[1] * (steps - (i + 1))) / steps +
                     float(end_color[1] * (i + 1)) / steps,
                     
                     float(start_color[2] * (steps - (i + 1))) / steps +
                     float(end_color[2] * (i + 1)) / steps,))

        self.palette = []
        bilinear_palette((  0,   0,   0,), (  0,   0,   0,), 64)
        bilinear_palette((  0,   0,   0,), (128,  25,   0,), 32)
        bilinear_palette((128,  25,   0,), (230, 100,  25,), 32)
        bilinear_palette((230, 100,  25,), (242, 200,  50,), 32)
        bilinear_palette((242, 200,  50,), (255, 255, 255,), 32)
        bilinear_palette((255, 255, 255,), (255, 255, 255,), 64)
#        bilinear_palette((0.0, 0.0, 0.0,), (1.0, 1.0, 1.0,), 256)
        
        self.total_ticks = 0
        self.feedback_array = [0.5] * (array_width * array_height)
    
    def generate(self, ticks, colors):
        self.total_ticks += ticks

        array_top_row = (array_height - 1) * array_width

        for i in range(array_width / 10):
            self.feedback_array[array_top_row + random.randrange(array_width)] = 1.0
            self.feedback_array[array_top_row + random.randrange(array_width)] = 0.0
        
        self.feedback_array[array_top_row] = (
            self.feedback_array[array_top_row] +
            self.feedback_array[array_top_row] +
            self.feedback_array[array_top_row + 1] +
            self.feedback_array[array_top_row + array_width - 1]) / 4.0
        self.feedback_array[array_top_row + array_width - 1] = (
            self.feedback_array[array_top_row + array_width - 1] +
            self.feedback_array[array_top_row + array_width - 1] +
            self.feedback_array[array_top_row + array_width - 2] +
            self.feedback_array[array_top_row]) / 4.0
        for x in range(array_width - 2):
            self.feedback_array[array_top_row + x + 1] = (
                self.feedback_array[array_top_row + x] +
                self.feedback_array[array_top_row + x + 1] +
                self.feedback_array[array_top_row + x + 1] +
                self.feedback_array[array_top_row + x + 2]) / 4.0
        
        self.feedback_array[array_top_row - array_width] = (
            self.feedback_array[array_top_row - array_width] +
            self.feedback_array[array_top_row] +
            self.feedback_array[array_top_row - array_width + 1] +
            self.feedback_array[array_top_row - array_width + array_width - 1]) / 4.0
        self.feedback_array[array_top_row + array_width - 1] = (
            self.feedback_array[array_top_row - array_width + array_width - 1] +
            self.feedback_array[array_top_row + array_width - 1] +
            self.feedback_array[array_top_row - array_width + array_width - 2] +
            self.feedback_array[array_top_row - array_width]) / 4.0
        for x in range(array_width - 2):
            self.feedback_array[array_top_row - array_width + x + 1] = (
                self.feedback_array[array_top_row - array_width + x] +
                self.feedback_array[array_top_row + x + 1] +
                self.feedback_array[array_top_row - array_width + x + 1] +
                self.feedback_array[array_top_row - array_width + x + 2]) / 4.0
        
        for y in range(array_height - 2):
            self.feedback_array[y * array_width] = (
                self.feedback_array[y * array_width + array_width - 1] +
                self.feedback_array[(y + 1) * array_width] +
                self.feedback_array[y * array_width + 1] +
                self.feedback_array[(y + 2) * array_width]) / 4.0
            self.feedback_array[y * array_width + array_width - 1] = (
                self.feedback_array[y * array_width] +
                self.feedback_array[(y + 1) * array_width + array_width - 1] +
                self.feedback_array[y * array_width + array_width - 2] +
                self.feedback_array[(y + 2) * array_width + array_width - 1]) / 4.0
            for x in range(array_width - 2):
                self.feedback_array[y * array_width + x + 1] = (
                    self.feedback_array[y * array_width + x] +
                    self.feedback_array[(y + 1) * array_width + x + 1] +
                    self.feedback_array[y * array_width + x + 2] +
                    self.feedback_array[(y + 2) * array_width + x + 1]) / 4.0
        
        for i in range(array_width * array_height):
            colors[i] = self.palette[int(round(self.feedback_array[i] * (len(self.palette) - 1)))]

class ParticleFireworksEffect:
    def __init__(self):
        self.total_ticks = 0
        self.feedback_array = [0.5] * (array_width * array_height)
    
    def generate(self, ticks, colors):
        pass

class TestEffect:
    def __init__(self):
        self.total_ticks = 0
        self.width = float(array_width)
        self.height = float(array_height)
    
    def generate(self, ticks, colors):
        self.total_ticks += ticks
        for y in range(array_height):
            for x in range(array_width):
                colors[y * array_width + x] = (
                    math.sin((math.pi * 2.0 * self.total_ticks / 30.0) + math.pi * x / self.width) / 2 + 0.5,
                    y / self.height,
                    0.0,
                    0.5,)

effect = FeedbackFireEffect()

pygame.init()

width = 800
height = 450

screen = pygame.display.set_mode((width, height,), HWSURFACE DOUBLEBUF)

glEnable(GL_DEPTH_TEST)

glShadeModel(GL_FLAT)
glClearColor(0.0, 0.0, 0.0, 0.0)

glEnable(GL_COLOR_MATERIAL)

glEnable(GL_LIGHTING)
glEnable(GL_LIGHT0)        
glLight(GL_LIGHT0, GL_POSITION,  (1, 1, 1, 0))

glEnable(GL_BLEND)
glBlendFunc(GL_SRC_ALPHA, GL_ONE)

glViewport(0, 0, width, height)
glMatrixMode(GL_PROJECTION)
glLoadIdentity()
gluPerspective(45.0, float(width) / height, .1, 1000.0)

glMatrixMode(GL_MODELVIEW)
glLoadIdentity()

glTranslate(0, 0, -8)

clock = pygame.time.Clock()    

light_point_vertices = []
light_glow_vertices = []

for j in range(array_height):
    for i in range(array_width):
        x = (i - array_width / 2 + 0.5) * 0.1
        y = (array_height / 2 - j) * 0.1

        lightScale = 0.02
        light_point_vertices.append(
            ((-lightScale + x, -lightScale + y, 0.0,),
             (-lightScale + x,  lightScale + y, 0.0,),
             ( lightScale + x,  lightScale + y, 0.0,),
             ( lightScale + x, -lightScale + y, 0.0,),
             ))
        
        lightScale = 0.04
        light_glow_vertices.append(
            ((-lightScale + x, -lightScale + y, 0.0,),
             (-lightScale + x,  lightScale + y, 0.0,),
             ( lightScale + x,  lightScale + y, 0.0,),
             ( lightScale + x, -lightScale + y, 0.0,),
             ))

for j in range(array_height):
    for i in range(array_width):
        light_colors.append((1.0, 1.0, 1.0,))

remainder_ms = 0

while True:
    for e in pygame.event.get():
        if e.type == pygame.QUIT or e.type == pygame.KEYDOWN:
            pygame.quit()
            sys.exit()
    
    remainder_ms += clock.tick()
    ticks = int(remainder_ms / 33)
    remainder_ms -= ticks * 33
    if ticks > 0:
        effect.generate(ticks, light_colors)
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMaterial(GL_FRONT, GL_AMBIENT, (0.1, 0.1, 0.1, 1.0))
    glMaterial(GL_FRONT, GL_DIFFUSE, (1.0, 1.0, 1.0, 1.0))
    
    # draw Artifactuary

    glMaterial(GL_FRONT, GL_AMBIENT, (1.0, 1.0, 1.0, 1.0))
    glMaterial(GL_FRONT, GL_DIFFUSE, (0.0, 0.0, 0.0, 1.0))

    # draw lights
    
    glDepthMask(False)
    
    glPushMatrix()
    glTranslate(0.0, 0.0, 0.0)

    glBegin(GL_QUADS)
    for i in range(array_width * array_height):
        c = light_colors[i]
        glColor(c)
        glVertex(light_point_vertices[i][0])
        glVertex(light_point_vertices[i][1])
        glVertex(light_point_vertices[i][2])
        glVertex(light_point_vertices[i][3])
        glVertex(light_glow_vertices[i][0])
        glVertex(light_glow_vertices[i][1])
        glVertex(light_glow_vertices[i][2])
        glVertex(light_glow_vertices[i][3])
    glEnd()
    
    glPopMatrix()
    
    glDepthMask(True)
    
    pygame.display.flip()

