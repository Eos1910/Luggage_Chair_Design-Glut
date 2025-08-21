#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

static float lightPos[4] = {15.0,15.0,-6.0,1.0};

const int texWidth = 256;
const int texHeight = 256;
GLuint luggageTexture;
GLuint woodTexture;
GLuint tireTexture;
GLuint carbonFiberTexture;
GLuint seatTexture;

float angle = 90; // the cylinder on the ceiling rod
float thickness1 = 0.1;
// Angles for that 3D rotation effect
float angleX = 20.0f; // Tilt angle along the X-axis
float angleY = 0.0f; // Rotation angle along the Y-axis
float zoom = -5.0f; // Initial zoom level

// Luggage proportions
float luggageWidth = 1.0f;
float luggageHeight = 1.5f;
float luggageDepth = 0.65f;

float luggageColor[3] = {0.53f, 0.81f, 0.92f};

// Wheel proportions
float wheelRadius = 0.025f;
float wheelThickness = 0.03f;

// Strap proportions
float strapWidth = 0.05f;
float strapHeight = 0.2f;
float strapDepth = 0.05f;

// Zipper proportions
float zipperWidth = 0.005f;
float zipperHeight = 0.01f;
float zipperLength = luggageHeight;
float tagWidth = 0.05f;
float tagHeight = 0.02f;
float tagLength = 0.1f;

float edgeRadius = 0.06f;
float halfWidth = luggageWidth / 2.0;
float halfHeight = luggageHeight / 2.0f;
float halfDepth = luggageDepth / 2.0f;

const float minZoom = -5.5f;
const float maxZoom = -3.5f;

bool isOpening = false;
bool isRedActive = false;
bool isGreenActive = false;
bool isYellowActive = false;

bool isOpen = false;
bool lidOpening = false;
bool lidIsOpen = false;
bool chairOpen = false;
float openAngle = 0.0f;
float chairAngle = 0.0f;
float lidAngle = 75.0f;
static float shaftLength1 = 0.01f;
static float shaftLength2 = 0.01f;

const float MAX_LENGTH = 1.0f;
const float MIN_LENGTH = 0.01f;
const float INITIAL_LENGTH = 2.0f; // Initial length of the grey rods
const float MAX_CHAIR_SUPPORT = 0.8f; // Chair Support Extension
const float MIN_CHAIR_SUPPORT = 0.0f;
const float MAX_WEIGHT_SUPPORT = 1.2f;
const float MIN_WEIGHT_SUPPORT = 0.2f;

float rodYchairOffset = 0.8f;
static float chairSupport1 = 0.0f;
static float chairSupport2 = 0.0f;
static float weightSupport1 = 0.0f;
static float weightSupport2 = 0.0f;

float rodXoffset = 0.7f;
float rodYoffset = 0.8f;
float extensionOffset = 0.6f;
static float zoomLevel = -15.0f;
static float rotationAngle = 0.0f;

float tagX = -luggageWidth / 2.2f;
float tagY = luggageHeight / 2.5f;
int animationStep = 0;

void myString(const char* text, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z); // Move to the specified position
    glScalef(0.070f, 0.070f, 0.070f);       // Scale the text to fit the scene
    // Calculate a pulsating brightness value
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Get time in seconds
    float brightness = (sin(time * 3.0f) + 1.0f) / 2.0f; // Range [0, 1] oscillation
    // Set color with pulsating brightness
    glColor3f(brightness, brightness, 1.0f); // Shining white-blue color
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c); // Render each character
    }
    glPopMatrix();
}

GLuint loadTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("C:\\Users\\Texture.jpg", &width, &height, &channels, 3);
    if (image) {
        glGenTextures(1, &luggageTexture);
        glBindTexture(GL_TEXTURE_2D, luggageTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);

    }
    else {
        printf("Failed to load texture.\n");
    }
}

GLuint loadWoodTexture() {
    int width, height, channels;
    unsigned char *image2 = stbi_load("C:\\Users\\wooden_floor.jpg", &width, &height, &channels, 3);
    if (image2) {
        glGenTextures(1, &woodTexture);
        glBindTexture(GL_TEXTURE_2D, woodTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
        stbi_image_free(image2);

    }
    else {
        printf("Failed to load texture.\n");
    }
}

void generateDetailedTireTexture() {
    GLubyte textureData[texWidth][texHeight][3];

    float colorPalette[4][3] = {
        {0.9f, 0.9f, 0.9f},
        {0.7f, 0.7f, 0.7f},
        {1.0f, 1.0f, 1.0f},
        {0.6f, 0.7f, 0.8f}
    };

    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            float wave = fabs(sin(x * 0.1f) * 0.5f + cos(y * 0.05f) * 0.5f);

            int colorIndex;
            if (wave > 0.75f) {
                colorIndex = 0;
            } else if (wave > 0.5f) {
                colorIndex = 1;
            } else if (wave > 0.25f) {
                colorIndex = 2;
            } else {
                colorIndex = 3;
            }

            textureData[y][x][0] = (GLubyte)(colorPalette[colorIndex][0] * 255);
            textureData[y][x][1] = (GLubyte)(colorPalette[colorIndex][1] * 255);
            textureData[y][x][2] = (GLubyte)(colorPalette[colorIndex][2] * 255);
    }
}

    glGenTextures(1, &tireTexture);
    glBindTexture(GL_TEXTURE_2D, tireTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void generateCarbonFiberTexture(){
    GLubyte textureData[texWidth][texHeight][3];

    float darkGray[3] = {0.7f, 0.7f, 0.7f};
    float lightGray[3] = {0.5f, 0.5f, 0.5f};

    for (int y = 0; y < texHeight; y++){
        for (int x = 0; x < texWidth; x++){
            int checker = (x / 8 + y / 8) % 2;
            float *color = checker ? lightGray : darkGray;

            textureData[y][x][0] = (GLubyte)(color[0] * 255);
            textureData[y][x][1] = (GLubyte)(color[1] * 255);
            textureData[y][x][2] = (GLubyte)(color[2] * 255);
        }
    }

    glGenTextures(1, &carbonFiberTexture);
    glBindTexture(GL_TEXTURE_2D, carbonFiberTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void generateSeatTexture(){
    GLubyte textureData[texWidth][texHeight][3];

    float colorPalette[3][3] = {
        {0.9f, 0.9f, 0.9f},
        {0.7f, 0.7f, 0.7f},
        {0.5f, 0.5f, 0.5f}
    };

    for (int y = 0; y < texHeight; y++){
        for (int x = 0; x <texWidth; x++){
            float baseColor = 0.8f + (std::rand() % 20 - 10) / 100.0f;
            bool isLine = ((std::rand() % 100) < 5) && (x % 10 == 0 || y % 10 == 0);
            int colorIndex = isLine ? (std::rand() % 3) : 1;

            textureData[y][x][0] = (GLubyte)(colorPalette[colorIndex][0] * 255 * baseColor);
            textureData[y][x][1] = (GLubyte)(colorPalette[colorIndex][1] * 255 * baseColor);
            textureData[y][x][2] = (GLubyte)(colorPalette[colorIndex][2] * 255 * baseColor);
        }
    }

    glGenTextures(1, &seatTexture);
    glBindTexture(GL_TEXTURE_2D, seatTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void drawLogo(float x1,float y1,float x2,float y2,float z1,float z2,float cx,float cy,float cz) {
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1; t += 0.001) {
        float x = (1 - t) * (1 - t) * x1 + 2 * (1 - t) * t * cx + t * t * x2;
        float y = (1 - t) * (1 - t) * y1 + 2 * (1 - t) * t * cy + t * t * y2;
        float z = (1 - t) * (1 - t) * z1 + 2 * (1 - t) * t * cz + t * t * z2;
        glVertex3f(x, y,z);}glEnd();
}

void shadowMatrix(GLfloat shadowMat[4][4], GLfloat lightPos[4], GLfloat groundPlane[4]) {
    GLfloat dot = (groundPlane[0] * lightPos[0] +
                  groundPlane[1] * lightPos[1] +
                  groundPlane[2] * lightPos[2] +
                  groundPlane[3] * lightPos[3]);

    shadowMat[0][0] = dot - lightPos[0] * groundPlane[0];
    shadowMat[1][0] = 0.0f - lightPos[0] * groundPlane[1];
    shadowMat[2][0] = 0.0f - lightPos[0] * groundPlane[2];
    shadowMat[3][0] = 0.0f - lightPos[0] * groundPlane[3];

    shadowMat[0][1] = 0.0f - lightPos[1] * groundPlane[0];
    shadowMat[1][1] = dot - lightPos[1] * groundPlane[1];
    shadowMat[2][1] = 0.0f - lightPos[1] * groundPlane[2];
    shadowMat[3][1] = 0.0f - lightPos[1] * groundPlane[3];

    shadowMat[0][2] = 0.0f - lightPos[2] * groundPlane[0];
    shadowMat[1][2] = 0.0f - lightPos[2] * groundPlane[1];
    shadowMat[2][2] = dot - lightPos[2] * groundPlane[2];
    shadowMat[3][2] = 0.0f - lightPos[2] * groundPlane[3];

    shadowMat[0][3] = 0.0f - lightPos[3] * groundPlane[0];
    shadowMat[1][3] = 0.0f - lightPos[3] * groundPlane[1];
    shadowMat[2][3] = 0.0f - lightPos[3] * groundPlane[2];
    shadowMat[3][3] = dot - lightPos[3] * groundPlane[3];
}

void myInit(void){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color to black and opaque
    glEnable(GL_DEPTH_TEST); // Stores fragments their z-values in the depth buffer
    glEnable(GL_LIGHTING); // Enable lighting
    glEnable(GL_LIGHT0); // Enable default lighting
    glEnable(GL_NORMALIZE); // Ensure consistent lighting
    glEnable(GL_COLOR_MATERIAL); // Allows materials to use colors
    glShadeModel(GL_SMOOTH); // Enable smooth Gourand shading
    glShadeModel(GL_FLAT); // Set shading model to flat
    glDepthFunc(GL_LEQUAL); // Set the type of depth-test
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Highest-quality perspective corrections
    glEnable(GL_TEXTURE_2D); // Enable texturing mapping
    loadTexture();
    loadWoodTexture();
    generateDetailedTireTexture();
    generateCarbonFiberTexture();
    generateSeatTexture();
}

void myGround(){
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glColor3f(1.5f, 1.5f, 1.5f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0,0.0); glVertex3f(-8.0f, -0.93f, -8.0f);
    glTexCoord2f(1.0,0.0); glVertex3f(8.0f, -0.93f, -8.0f);
    glTexCoord2f(0.0,1.0); glVertex3f(8.0f, -0.93f, 8.0f);
    glTexCoord2f(1.0,1.0); glVertex3f(-8.0f, -0.93f, 8.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void myWheel1(float x, float y, float z, float angle, float length, float radius, float height){
    GLUquadric* quadwheel = gluNewQuadric();
    gluQuadricTexture(quadwheel, GL_TRUE);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0.0f, 0.5f, 0.5f);
    glColor3f(0.4f, 0.4f, 0.4f);
    gluCylinder(quadwheel, radius / 2, radius / 2, length, 10,10);
    glPopMatrix();
    gluDeleteQuadric(quadwheel);

    glBindTexture(GL_TEXTURE_2D, tireTexture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(180, 1.0f, 0.0f, 0.0f); // Align the wheels to the ground
    glutSolidTorus(wheelRadius, wheelThickness, 30, 20); // Smooth torus
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void myWheel2(float x, float y, float z, float radius, float height){
    GLUquadric* quadpro = gluNewQuadric();
    glPushMatrix();
    glTranslatef(x, y + height / 2.5, z);
    glRotatef(-90, 1.0f, 0.0f, 0.0f); // Align with wheel
    glColor3f(0.3f, 0.3f, 0.3f); // Darker gray for protection
    gluCylinder(quadpro, radius, radius, height, 30, 30);
    glPopMatrix();
    gluDeleteQuadric(quadpro);

    // Add concave protective surface (curved inward) on top
    GLUquadric* concave = gluNewQuadric();
    for (float offset = 0.01f; offset <= thickness1; offset += 0.002f) { // thickness
        glPushMatrix();
        glTranslatef(x, y - 0.05 + height / 2.2, z + offset - 0.055);
        glRotatef(90, 0.0f, 0.0f, 1.0f);
        glColor3f(0.4f, 0.4f, 0.4f);
        glScalef(1.5f, 2.0f, 1.0f);
        gluPartialDisk(concave, radius / 1.5, radius * 0.75f, 50, 1, 0, 180);
        glPopMatrix();
    }
    gluDeleteQuadric(concave);
}

void myStrap(float x, float y, float z, float scaleX, float scaleY, float scaleZ){
    glPushMatrix();
    glTranslatef(x - scaleX * 0.4f, y, z);
    glScalef(scaleX * 0.2f, scaleY, scaleZ); // Scale the strap size
    glColor3f(0.4f, 0.3f, 0.2f); // Light brown color
    glutSolidCube(0.6f); // Simple cube
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x + scaleX * 0.4f, y, z);
    glScalef(scaleX * 0.2f, scaleY, scaleZ);
    glColor3f(0.4f, 0.3f, 0.2f);
    glutSolidCube(0.6f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    int segments = 45;
    float radius = scaleX * 0.4f;
    float cylinderHeight = scaleX * 0.1f;

    for (int i = 0; i < segments; ++i){
        float theta = (float)i / (segments - 1) * M_PI;
        float nextTheta = (float)(i + 1) / (segments - 1) * M_PI;

        float startX = radius * cos(theta);
        float startY = radius * sin(theta);
        float endX = radius * cos(nextTheta);
        float endY = radius * sin(nextTheta);

        glPushMatrix();
        glTranslatef((startX + endX) / 2, (startY + endY) / 4.5, 0.0f);

        float angle = atan2(endY - startY, endX - startX) * 180.0f / M_PI;
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        glScalef(cylinderHeight, sqrt((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY)), cylinderHeight);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPopMatrix();
}

void myZipper(float x, float y, float z, float length, float width, float height, float angleX, float angleY, float angleZ){
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
    glScalef(length, height, width);
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void myZipperTag(float x, float y, float z, float width, float height, float length, float angle){
    glPushMatrix();
    glTranslatef(x, y ,z);
    glRotatef(angle, -1.0f, -1.0f, 1.0f);
    glScalef(width, height, length);
    glColor3f(0.4f, 0.4f, 0.4f);
    glutSolidCube(0.5f);
    glPopMatrix();
}

void myEdge(float x, float y, float z, float angle, float length, float radius){
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(x / 1.3, y, z / 1.25);
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    glColor3fv(luggageColor);
    gluCylinder(quad, radius, radius, length, 30, 30);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void solidBox(GLdouble width, GLdouble height, GLdouble depth) {
    glPushMatrix();
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawVerticalRod(float xOffset, float rodYoffset, float shaftLength) {
    glPushMatrix();
    glTranslatef(xOffset, 0.0f + rodYoffset, 0.0f);
    glColor3f(0.6f, 0.6f, 0.6f);

    // Draw the main rod
    glPushMatrix();
    glTranslatef(0.0f, 0.0005f, 0.25f);
    solidBox(0.02f, 0.1f, 0.05f);
    glPopMatrix();

    // Draw the shaft
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, shaftLength / 8.5, 0.25f);
    solidBox(0.005f, shaftLength, 0.05f);
    glPopMatrix();

    glPopMatrix();
}

void texturedSolidBox(float width, float height, float depth) {
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float halfD = depth / 2.0f;

    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, -halfH, halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, -halfH, halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, halfH, halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, halfH, halfD);

    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, -halfH, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, -halfH, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, halfH, -halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, halfH, -halfD);

    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, halfH, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, halfH, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, halfH, halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, halfH, halfD);

    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, -halfH, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, -halfH, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, -halfH, halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, -halfH, halfD);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfW, -halfH, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, halfH, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, halfH, halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfW, -halfH, halfD);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, -halfH, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfW, halfH, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfW, halfH, halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, -halfH, halfD);
    glEnd();
}

void drawBackBox(float rodYoffset) {
    glBindTexture(GL_TEXTURE_2D, tireTexture);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0.0f, -0.05f + rodYoffset, 0.25f);
    glColor3f(3.0f, 3.0f, 3.0f);
    texturedSolidBox(0.5f, 0.01f, 0.1f);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawTopConnectingRod(float rodYoffset, float shaftLength1, float shaftLength2) {
    float topRodY = fmax(shaftLength1, shaftLength2) + 0.6f;
    float topRodLength = fabs(shaftLength1 - shaftLength2) + 0.8f;
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.4f);
    glTranslatef(0.0f, (topRodY + rodYoffset) / 1.7, 0.25f);
    solidBox(0.23f, 0.04f, 0.045f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, (topRodY + rodYoffset) / 1.685, 0.25f);
    glColor3f(0.5f, 0.5f, 0.4f);
    glutSolidSphere(0.02f, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glRotatef(angle,0.0,1.0,0.0);
    glTranslatef(-0.25f, (topRodY + rodYoffset) / 1.7, 0.100f);
    glColor3f(0.5f, 0.5f, 0.4f);
    glutSolidTorus(0.012, 0.021, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glRotatef(angle,0.0,1.0,0.0);
    glTranslatef(-0.25f, (topRodY + rodYoffset) / 1.7, -0.100f);
    glColor3f(0.5f, 0.5f, 0.4f);
    glutSolidTorus(0.012, 0.021, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glRotatef(angle,0.0,1.0,0.0);
    glTranslatef(-0.25f, (topRodY + rodYoffset) / 1.7, -0.11f);
    glColor3f(0.5f, 0.5f, 0.4f);
    glutSolidTorus(0.012, 0.021, 50, 50);
    glPopMatrix();
    glPushMatrix();
    glRotatef(angle,0.0,1.0,0.0);
    glTranslatef(-0.25f, (topRodY + rodYoffset) / 1.7, 0.11f);
    glColor3f(0.5f, 0.5f, 0.4f);
    glutSolidTorus(0.012, 0.021, 50, 50);
    glPopMatrix();
}

void myLuggage(void){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,luggageTexture);
    // Draw the luggage
    glPushMatrix();
    glBegin(GL_QUADS);
    // Front Face
    glColor3fv(luggageColor);
    glNormal3f(0.0f, 0.0f, 1.0f); // The normal vectors perpendicular to the vertex
    glTexCoord2d(1.0,0.0); glVertex3f(-luggageWidth / 2.5, -luggageHeight / 2, luggageDepth / 2.035);
    glTexCoord2d(0.0,0.0); glVertex3f(luggageWidth / 2.5, -luggageHeight / 2, luggageDepth / 2.035);
    glTexCoord2d(1.0,1.0); glVertex3f(luggageWidth / 2.5, luggageHeight / 2, luggageDepth / 2.035);
    glTexCoord2d(0.0,1.0); glVertex3f(-luggageWidth / 2.5, luggageHeight / 2, luggageDepth / 2.035);

    glColor3f(1.5f, 1.5f, 1.5f);
    glNormal3f(0.0f, 0.0f, 1.0f); // The normal vectors perpendicular to the vertex
    glVertex3f(-luggageWidth / 2.3, -luggageHeight / 2, luggageDepth / 5);
    glVertex3f(luggageWidth / 2.3, -luggageHeight / 2, luggageDepth / 5);
    glVertex3f(luggageWidth / 2.3, luggageHeight / 2, luggageDepth / 5);
    glVertex3f(-luggageWidth / 2.3, luggageHeight / 2, luggageDepth / 5);

    // Back Face
    glColor3fv(luggageColor);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2d(1.0,0.0); glVertex3f(-luggageWidth / 2.5 + (-openAngle / 90), -luggageHeight / 2, -luggageDepth / 2.035 + (openAngle * 0.002));
    glTexCoord2d(0.0,0.0); glVertex3f(luggageWidth / 2.5 + (-openAngle / 45), -luggageHeight / 2, -luggageDepth / 2.035 + (-openAngle / 33.5));
    glTexCoord2d(1.0,1.0); glVertex3f(luggageWidth / 2.5 + (-openAngle / 45), luggageHeight / 2, -luggageDepth / 2.035 + (-openAngle / 33.5));
    glTexCoord2d(0.0,1.0); glVertex3f(-luggageWidth / 2.5 + (-openAngle / 90), luggageHeight / 2, -luggageDepth / 2.035 + (openAngle * 0.002));

    glColor3f(1.5f, 1.5f, 1.5f);
    glNormal3f(0.0f, 0.0f, -1.0f); // The normal vectors perpendicular to the vertex
    glVertex3f(-luggageWidth / 3 + (-openAngle / 110), -luggageHeight / 2, luggageDepth / 8 + (-openAngle / 50));
    glVertex3f(luggageWidth / 2.8 + (-openAngle / 200), -luggageHeight / 2, luggageDepth / 8 + (-openAngle / 20));
    glVertex3f(luggageWidth / 2.8 + (-openAngle / 200), luggageHeight / 2, luggageDepth / 8 + (-openAngle / 20));
    glVertex3f(-luggageWidth / 3 + (-openAngle / 110), luggageHeight / 2, luggageDepth / 8 + (-openAngle / 50));
    glEnd();
    glPopMatrix();

    // Left Face
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(-luggageWidth / 2.25, -luggageHeight / 2, 0.0);
    glTexCoord2d(-1.0,0.0); glVertex3f(-luggageWidth / 2.25, -luggageHeight / 2, luggageDepth / 2.5);
    glTexCoord2d(0.0,-1.0); glVertex3f(-luggageWidth / 2.25, luggageHeight / 2, luggageDepth / 2.5);
    glTexCoord2d(-1.0,-1.0); glVertex3f(-luggageWidth / 2.25, luggageHeight / 2, 0.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-openAngle / 80, 0.0f , 0.0f);
    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(-luggageWidth / 2.25 + openAngle / 45, -luggageHeight / 2, 0.0 + openAngle / 40);
    glTexCoord2d(-1.0,0.0); glVertex3f(-luggageWidth / 2.25 + (openAngle * 0.0015), -luggageHeight / 2, -luggageDepth / 2.5 + -(openAngle / 500));
    glTexCoord2d(0.0,-1.0); glVertex3f(-luggageWidth / 2.25 + (openAngle * 0.0015), luggageHeight / 2, -luggageDepth / 2.5 + -(openAngle / 500));
    glTexCoord2d(-1.0,-1.0); glVertex3f(-luggageWidth / 2.25 + openAngle / 45, luggageHeight / 2, 0.0 + openAngle / 40);
    glEnd();
    glPopMatrix();

    // Right Face
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(luggageWidth / 2.25, -luggageHeight / 2, 0.0);
    glTexCoord2d(-1.0,0.0); glVertex3f(luggageWidth / 2.25, -luggageHeight / 2, luggageDepth / 2.5);
    glTexCoord2d(0.0,-1.0); glVertex3f(luggageWidth / 2.25, luggageHeight / 2, luggageDepth / 2.5);
    glTexCoord2d(-1.0,-1.0); glVertex3f(luggageWidth / 2.25, luggageHeight / 2, 0.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -openAngle / 50);
    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(luggageWidth / 2.25 + (-openAngle / 115), -luggageHeight / 2, 0.0 + (-openAngle / 75));
    glTexCoord2d(-1.0,0.0); glVertex3f(luggageWidth / 2.25 + (-openAngle / 42), -luggageHeight / 2, -luggageDepth / 2.5 + (-openAngle / 70));
    glTexCoord2d(0.0,-1.0); glVertex3f(luggageWidth / 2.25 + (-openAngle / 42), luggageHeight / 2, -luggageDepth / 2.5 + (-openAngle / 70));
    glTexCoord2d(-1.0,-1.0); glVertex3f(luggageWidth / 2.25 + (-openAngle / 115), luggageHeight / 2, 0.0 + (-openAngle / 75));
    glEnd();
    glPopMatrix();

    // Top Face
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(-luggageWidth / 2.255, luggageHeight / 2, 0.0);
    glTexCoord2d(1.0,0.0); glVertex3f(luggageWidth / 2.255, luggageHeight / 2, 0.0);
    glTexCoord2d(1.0,1.0); glVertex3f(luggageWidth / 2.255, luggageHeight / 2, luggageDepth / 2);
    glTexCoord2d(0.0,1.0); glVertex3f(-luggageWidth / 2.255, luggageHeight / 2, luggageDepth / 2);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f - openAngle / 360, 0.0f, 0.0f - openAngle / 32);
    for (int i = 0; i < 10; i += 5){
        glRotatef(i, 0.0f, 0.0f + openAngle, 0.0f);
    }

    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(-luggageWidth / 2.255, luggageHeight / 2, 0.0 + openAngle / 35);
    glTexCoord2d(1.0,0.0); glVertex3f(luggageWidth / 2.255 + (-openAngle / 180), luggageHeight / 2, 0.0 + openAngle / 550);
    glTexCoord2d(1.0,1.0); glVertex3f(luggageWidth / 2.255 + (-openAngle / 60), luggageHeight / 2, -luggageDepth / 2 + openAngle / 560);
    glTexCoord2d(0.0,1.0); glVertex3f(-luggageWidth / 2.255 + (-openAngle / 100), luggageHeight / 2, -luggageDepth / 2 + openAngle / 35);
    glEnd();
    glPopMatrix();

    // Bottom Face
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(-luggageWidth / 2.255, -luggageHeight / 2, 0.0);
    glTexCoord2d(-1.0,0.0); glVertex3f(luggageWidth / 2.255, -luggageHeight / 2, 0.0);
    glTexCoord2d(-1.0,-1.0); glVertex3f(luggageWidth / 2.255, -luggageHeight / 2, luggageDepth / 2);
    glTexCoord2d(0.0,-1.0); glVertex3f(-luggageWidth / 2.255, -luggageHeight / 2, luggageDepth / 2);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f - openAngle / 360, 0.0f, 0.0f - openAngle / 32);
    for (int i = 0; i < 10; i+=5){
        glRotatef(i, 0.0f, 0.0f + openAngle, 0.0f);
    }

    glBegin(GL_QUADS);
    glColor3fv(luggageColor);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2d(0.0,0.0); glVertex3f(-luggageWidth / 2.255, -luggageHeight / 2, 0.0 + openAngle / 35);
    glTexCoord2d(-1.0,0.0); glVertex3f(luggageWidth / 2.255 + (-openAngle / 180), -luggageHeight / 2, 0.0 + openAngle / 550);
    glTexCoord2d(-1.0,-1.0); glVertex3f(luggageWidth / 2.255 + (-openAngle / 60), -luggageHeight / 2, -luggageDepth / 2 + openAngle / 560);
    glTexCoord2d(0.0,-1.0); glVertex3f(-luggageWidth / 2.255 + (-openAngle / 100), -luggageHeight / 2, -luggageDepth / 2 + openAngle / 35);
    glEnd();
    glPopMatrix();

    //glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawBackRectangularBox() {
    glBindTexture(GL_TEXTURE_2D, carbonFiberTexture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0.0f, -0.65f, 0.425f);
    glColor3f(0.5f, 0.5f, 0.5f);
    texturedSolidBox(0.85f, 0.2f, 0.2f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.35f, -0.1f, 0.425f);
    glColor3f(0.5f, 0.5f, 0.5f);
    texturedSolidBox(0.15f, 0.9f, 0.2f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.35f, -0.1f, 0.425f);
    glColor3f(0.5f, 0.5f, 0.5f);
    texturedSolidBox(0.15f, 0.9f, 0.2f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.425f);
    glColor3f(0.5f, 0.5f, 0.5f);
    texturedSolidBox(0.85f, 0.5f, 0.2f);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void drawSeat() {
    glBindTexture(GL_TEXTURE_2D, seatTexture);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0.0f, -0.395f, 0.075f);
    glColor3f(0.8f, 0.7f, 0.6f);
    texturedSolidBox(0.5f, 0.625f, 0.05f);
    glPopMatrix();

    GLUquadric* quad = gluNewQuadric(); // Create a new quadric object

    glPushMatrix();
    glColor3f(0.8f, 0.7f, 0.6f); // Set the cylinder color
    glTranslatef(0.25f,-0.695f,0.075f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); // Align the cylinder along the Z-axis
    gluCylinder(quad, 0.025, 0.025, 0.5, 30, 30); // (quad, base radius, top radius, height, slices, stacks)
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.8f, 0.7f, 0.6f); // Set the cylinder color
    glTranslatef(0.25f,-0.195f,0.075f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); // Align the cylinder along the Z-axis
    gluCylinder(quad, 0.025, 0.025, 0.5, 30, 30); // (quad, base radius, top radius, height, slices, stacks)
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.8f, 0.7f, 0.6f); // Set the cylinder color
    glTranslatef(0.25f,-0.7125f,0.075f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Align the cylinder along the Z-axis
    gluCylinder(quad, 0.025, 0.025, 0.625, 30, 30); // (quad, base radius, top radius, height, slices, stacks)
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.8f, 0.7f, 0.6f); // Set the cylinder color
    glTranslatef(-0.25f,-0.7125f,0.075f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Align the cylinder along the Z-axis
    gluCylinder(quad, 0.025, 0.025, 0.625, 30, 30); // (quad, base radius, top radius, height, slices, stacks)
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void drawCharger() {
    glPushMatrix();
    glTranslatef(0.425f, 0.5f, 0.42f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glColor3f(0.4f, 0.4f, 0.4f);
    solidBox(0.2, 0.01, 0.15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.430f, 0.5f, 0.42f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    solidBox(0.1, 0.005, 0.07);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.432f, 0.55f, 0.42f);
    glRotatef(lidAngle, 0.0f, 0.0f, -1.0f);

    glColor3f(0.45, 0.45f, 0.45f);
    solidBox(0.22, 0.01, 0.15);
    glPopMatrix();
}

void drawChair(){
    GLUquadric* quad = gluNewQuadric(); // Create a new quadric object

    glPushMatrix();
    glColor3f(0.65f, 0.65f, 0.65f); // Set the cylinder color
    glTranslatef(0.3f,0.0f,0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); // Align the cylinder along the Z-axis
    gluCylinder(quad, 0.05, 0.05, 0.6, 30, 30); // (quad, base radius, top radius, height, slices, stacks)
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.295f, 0.0f);
    glColor3f(0.65f, 0.65f, 0.65f);
    solidBox(0.3f, 0.6f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.395f, 0.0f);
    glColor3f(0.65f, 0.65f, 0.65f);
    solidBox(0.5f, 0.6f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2f, -0.395f, -0.1f);
    glColor3f(0.15f, 0.15f, 0.15f);
    solidBox(0.1f, 0.6f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2f, -0.395f, -0.1f);
    glColor3f(0.15f, 0.15f, 0.15f);
    solidBox(0.1f, 0.6f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.395f, -0.1f);
    glColor3f(0.15f, 0.15f, 0.15f);
    solidBox(0.2f, 0.6f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.125f, -0.67f, -0.1f);
    glColor3f(0.15f, 0.15f, 0.15f);
    solidBox(0.05f, 0.05f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.125f, -0.67f, -0.1f);
    glColor3f(0.15f, 0.15f, 0.15f);
    solidBox(0.05f, 0.05f, 0.1f);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void chairRotate(){
    glPushMatrix();
    glTranslatef(0.0f,0.2f,0.4f);
    glRotatef(-180.0f, -1.0f, 0.0f, 0.0f);
    glRotatef(-180.0f, 0.0f, 0.0f, 0.0f);
    glRotatef(chairAngle, -1.0f, 0.0f, 0.0f);
    drawChair();
    drawSeat();
    glPopMatrix();
}

void weightSupport(float supportOffset, float extensionOffset){
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
    glColor3f(0.05f, 0.05f, 0.05f); // Set the cylinder color
    glTranslatef(supportOffset,-0.7f,0.3f);
    glRotatef(-90.0f, -0.1f, -0.1f, 1.0f); // Align the cylinder along the Z-axis
    gluCylinder(quad, 0.02, 0.005, 0.1 + extensionOffset, 30, 30); // (quad, base radius, top radius, height, slices, stacks)
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2f, -0.752f, 0.48f);
    glColor3f(0.0f, 0.0f, 0.0f);
    solidBox(0.1f, 0.01f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2f, -0.705f, 0.53f);
    glColor3f(0.0f, 0.0f, 0.0f);
    solidBox(0.1f, 0.1f, 0.01f);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.2f, -0.752f, 0.48f);
    glColor3f(0.0f, 0.0f, 0.0f);
    solidBox(0.1f, 0.01f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2f, -0.705f, 0.53f);
    glColor3f(0.0f, 0.0f, 0.0f);
    solidBox(0.1f, 0.1f, 0.01f);
    glPopMatrix();
    //Button
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.53f);
    glColor3f(0.9f, 0.9f, 0.9f);
    solidBox(0.05f, 0.05f, 0.01f);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawChairSupport(float xOffset, float rodYoffset,  float rodZoffset)
{
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f);
    glVertex3f(xOffset, -0.65f, 0.35f);
    glVertex3f(xOffset, -0.63f + rodYoffset, 0.2f + rodZoffset);
    glVertex3f(xOffset, -0.68f + rodYoffset, 0.25f + rodZoffset);
    glVertex3f(xOffset, -0.7f, 0.4f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f);
    glVertex3f(xOffset, -0.65f, 0.35f);
    glVertex3f(xOffset + 0.05f, -0.65f, 0.35f);
    glVertex3f(xOffset + 0.05f, -0.63f + rodYoffset, 0.25f + rodZoffset);
    glVertex3f(xOffset, -0.63f + rodYoffset, 0.25f + rodZoffset);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f);
    glVertex3f(xOffset + 0.05f, -0.65f, 0.35f);
    glVertex3f(xOffset + 0.05f, -0.63f + rodYoffset, 0.2f + rodZoffset);
    glVertex3f(xOffset + 0.05f, -0.68f + rodYoffset, 0.25f + rodZoffset);
    glVertex3f(xOffset + 0.05f, -0.7f, 0.4f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f);
    glVertex3f(xOffset, -0.7f, 0.35f);
    glVertex3f(xOffset + 0.05f, -0.68f, 0.35f);
    glVertex3f(xOffset + 0.05f, -0.66f + rodYoffset, 0.25f + rodZoffset);
    glVertex3f(xOffset, -0.68f + rodYoffset, 0.25f + rodZoffset);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f);
    glVertex3f(xOffset, -0.63f + rodYoffset, 0.2f + rodZoffset);
    glVertex3f(xOffset + 0.05f, -0.63f + rodYoffset , 0.2f + rodZoffset);
    glVertex3f(xOffset + 0.05f, -0.68f + rodYoffset, 0.25f + rodZoffset);
    glVertex3f(xOffset, -0.68f + rodYoffset, 0.25f + rodZoffset);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.125f, -0.552f, 0.42f);
    glColor3f(0.0f, 0.0f, 0.0f);
    solidBox(0.1f, 0.01f, 0.2f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.125f, -0.552f, 0.42f);
    glColor3f(0.0f, 0.0f, 0.0f);
    solidBox(0.1f, 0.01f, 0.2f);
    glPopMatrix();

    //Button
    glPushMatrix();
    glTranslatef(0.0f, -0.655f, 0.53f);
    glColor3f(0.9f, 0.9f, 0.9f);
    solidBox(0.05f, 0.05f, 0.01f);
    glPopMatrix();
}

void myShadow(){
    GLfloat groundPlane[4] = {0.0f, 1.0f, 0.0f, 0.92f};
    GLfloat shadowColor[] = {0.0f, 0.0f, 0.0f, 0.6f};
    GLfloat shadowMat[4][4];
    shadowMatrix(shadowMat, lightPos, groundPlane);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, shadowColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);// *

    glDepthMask(GL_FALSE);

    glPushMatrix();
    glMultMatrixf((GLfloat*)shadowMat);
    glTranslatef(0.6f, 0.1f, 0.2f);
    glRotatef(-90, 1.0, 0.0, 0.0);

    myLuggage();
    drawCharger();
    drawBackRectangularBox();
    chairRotate();
    drawChairSupport(0.1f, chairSupport1, chairSupport1 );
    drawChairSupport(-0.15f, chairSupport2, chairSupport2 );

    weightSupport(0.2f, weightSupport1);
    weightSupport(-0.2f, weightSupport2);
    drawCharger();

    drawVerticalRod(-0.1f, rodYoffset, shaftLength1);
    drawVerticalRod(0.1f, rodYoffset, shaftLength2);
    drawBackBox(rodYoffset);
    drawTopConnectingRod(rodYoffset, shaftLength1, shaftLength2);
    glFlush();

    myEdge(-halfWidth, halfHeight, halfDepth, 90, luggageHeight, edgeRadius);
    myEdge(halfWidth, halfHeight, halfDepth, 90, luggageHeight, edgeRadius);

    myEdge(-halfWidth + (-openAngle / 73), halfHeight, -halfDepth + (openAngle * 0.0001), 90, luggageHeight, edgeRadius);
    myEdge(halfWidth + (-openAngle / 39), halfHeight, -halfDepth + (-openAngle / 27), 90, luggageHeight, edgeRadius);

    float sideStrapX = 0.0f; // Positioned properly on the side
    float sideStrapY = 0.45f; // Centered Vertically
    float sideStrapZ = 0.12f; // Slightly position to the side
    glRotatef(90, 0.0f, 0.0f, -1.0f);
    myStrap(sideStrapX, sideStrapY, sideStrapZ, strapHeight, strapWidth, strapDepth); // Right side strap

    myZipperTag(-luggageWidth / 2.2f, luggageHeight / 2.5f, 0.005f, tagWidth, tagHeight, tagLength, 90);
    myZipperTag(tagX, tagY, 0.017f, tagWidth, tagHeight, tagLength, 90);

    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void myDisplay(void){


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

    glLoadIdentity(); //Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, zoom); // Move the object back to view
    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Tilt along X-axis
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);  // Rotate along Y-axis

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_DEPTH_TEST);

    myGround();
    myLuggage();

    drawCharger();
    drawBackRectangularBox();
    chairRotate();

    myShadow();

    glPushMatrix();
    glColor3f(1.0f, 0.843f, 0.0f);
    glLineWidth(1.5);

    glTranslatef(0.0,0.66,0.53);
    glBegin(GL_LINES);

        glVertex3f(-0.05f, 0.05f,0.0);
        glVertex3f(0.04f, 0.05f,0.0);

        glVertex3f(-0.04f, 0.04f,0.0);
        glVertex3f(-0.01f, 0.04f,0.0);

        glVertex3f(-0.01f, 0.04f,0.0);
        glVertex3f(-0.01f, 0.01f,0.0);

        glVertex3f(0.04f, 0.05f,0.0);
        glVertex3f(0.01f, 0.01f,0.0);

        glVertex3f(0.05f, 0.04f,0.0);
        glVertex3f(0.01f, 0.0f,0.0);

        glVertex3f(0.01f, 0.04f,0.0);
        glVertex3f(0.01f, 0.01f,0.0);
        glVertex3f(0.01f, 0.04f,0.0);
        glVertex3f(0.05f, 0.04f,0.0);
    glEnd();
    drawLogo(-0.04f, 0.04f, -0.01f, 0.01f,0.0,0.0, -0.027f, 0.01f,0.0f);
    drawLogo(-0.05,0.05,-0.01,0.0,0.0,0.0,-0.03f, 0.005f,0.0f);

    glBegin(GL_LINES);

        glVertex3f(-0.01f, -0.0f,0.0);
        glVertex3f(-0.05f, -0.04f,0.0);

        glVertex3f(-0.01f, -0.01f,0.0);
        glVertex3f(-0.05f, -0.05f,0.0);

        glVertex3f(-0.05f, -0.04f,0.0);
        glVertex3f(-0.01f, -0.04f,0.0);

        glVertex3f(-0.01f, -0.01f,0.0);
        glVertex3f(-0.01f, -0.04f,0.0);

        glVertex3f(-0.05f, -0.05f,0.0);
        glVertex3f(0.05f, -0.05f,0.0);

        glVertex3f(0.01f, -0.01f,0.0);
        glVertex3f(0.01f, -0.04f,0.0);
        glVertex3f(0.04f, -0.04f,0.0);
        glVertex3f(0.01f, -0.04f,0.0);
    glEnd();

    drawLogo(0.01f, -0.0f,0.05f, -0.05f,0.0,0.0, 0.034f, -0.01f,0.0);
    drawLogo(0.01f, -0.01f,0.04f, -0.04f,0.0,0.0,0.025,-0.01,0.0);
    glPopMatrix();

    drawChairSupport(0.1f, chairSupport1, chairSupport1 );
    drawChairSupport(-0.15f, chairSupport2, chairSupport2 );

    weightSupport(0.2f, weightSupport1);
    weightSupport(-0.2f, weightSupport2);
    drawCharger();

    drawVerticalRod(-0.1f, rodYoffset, shaftLength1);
    drawVerticalRod(0.1f, rodYoffset, shaftLength2);
    drawBackBox(rodYoffset);
    drawTopConnectingRod(rodYoffset, shaftLength1, shaftLength2);
    glFlush();

    myEdge(-halfWidth, halfHeight, halfDepth, 90, luggageHeight, edgeRadius);
    myEdge(halfWidth, halfHeight, halfDepth, 90, luggageHeight, edgeRadius);

    myEdge(-halfWidth + (-openAngle / 73), halfHeight, -halfDepth + (openAngle * 0.0001), 90, luggageHeight, edgeRadius);
    myEdge(halfWidth + (-openAngle / 39), halfHeight, -halfDepth + (-openAngle / 27), 90, luggageHeight, edgeRadius);

    glPushMatrix();
    float sideStrapX = 0.0f; // Positioned properly on the side
    float sideStrapY = 0.45f; // Centered Vertically
    float sideStrapZ = 0.12f; // Slightly position to the side
    glRotatef(90, 0.0f, 0.0f, -1.0f);
    myStrap(sideStrapX, sideStrapY, sideStrapZ, strapHeight, strapWidth, strapDepth); // Right side strap
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    myString("Left Arrow Key = Camera left", 600.0f, 280.0f, 0.0f);
    myString("Right Arrow Key = Camera right", 600.0f, 266.0f, 0.0f);
    myString("Up Arrow Key = Camera up", 600.0f, 252.0f, 0.0f);
    myString("Down Arrow Key = Camera down", 600.0f, 238.0f, 0.0f);
    myString("Left Click = Open/Close Luggage", 600.0f, 224.0f, 0.0f);
    myString("Right Click = Open/Close Chair", 600.0f, 210.0f, 0.0f);
    myString("R/G/Y keys = Change luggage's color", 600.0f, 196.0f, 0.0f);
    myString("P key = Open/Close Charger Lid", 600.0f, 182.0f, 0.0f);
    myString("F1 key = Zoom In", 600.0f, 168.0f, 0.0f);
    myString("F2 key = Zoom Out", 600.0f, 154.0f, 0.0f);
    myString("F3 key = Handle up", 600.0f, 140.0f, 0.0f);
    myString("F4 key = Handle down", 600.0f, 126.0f, 0.0f);
    myString("F5 key = Chair Supports up", 600.0f, 112.0f, 0.0f);
    myString("F6 key = Chair Supports down", 600.0f, 98.0f, 0.0f);
    myString("F7 key = Weight Supports up", 600.0f, 84.0f, 0.0f);
    myString("F8 key = Weight Supports down", 600.0f, 70.0f, 0.0f);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    float topStrapX = 0.0f; // Position horizontally
    float topStrapY = luggageHeight / 2.2 + strapDepth / 2 + strapHeight / 3.5; // Slightly raised up to the top of the luggage
    float topStrapZ = 0.12f; // Slightly position to the side
    myStrap(topStrapX, topStrapY, topStrapZ, strapHeight, strapWidth, strapDepth); // Top side strap

    glPushMatrix();
    for (float i = -luggageWidth / 1.35; i < -0.55; i += zipperHeight * 1.5){
        myZipper(-luggageWidth / 2.31 - 0.01f, i, 0.005f, zipperWidth, zipperHeight, 0.01f, 0.0f, 0.0f, 0.0f);
    }

    for (float i = -0.75; i < -0.55; i += zipperHeight * 1.5){
        myZipper(-luggageWidth / 2.31 - 0.01f, i, -0.005f, zipperWidth, zipperHeight, 0.01f, 0.0f, 0.0f, 0.0f);
    }

    for (float i = -luggageWidth / 1.35; i < -0.55; i += zipperHeight * 1.5){
        myZipper(-luggageWidth / 2.31 - 0.01f, -i, 0.005f, zipperWidth, zipperHeight, 0.01f, 0.0f, 0.0f, 0.0f);
    }

    for (float i = -0.75; i < -0.55; i += zipperHeight * 1.5){
        myZipper(-luggageWidth / 2.31 - 0.01f, -i, -0.005f, zipperWidth, zipperHeight, 0.01f, 0.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPushMatrix();
    for (float i = -luggageWidth / 2.25; i < luggageWidth / 2.2; i += zipperHeight * 1.5){
        myZipper(i, luggageHeight / 2.03 + 0.01f, 0.005f, zipperWidth, zipperHeight, 0.01f, 90.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f + (-openAngle / 300), 0.0f, -0.01f + (-openAngle / 65));
    glRotatef(180 - (-openAngle * 2.3), 0.0f, 1.0f, 0.0f);
    for (float i = -0.415; i < luggageWidth / 2.25; i += zipperHeight * 1.5){
        myZipper(i, luggageHeight / 2.03 + 0.01f, -0.005f, zipperWidth, zipperHeight, 0.01f, 90.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPushMatrix();
    for (float i = -luggageHeight / 2; i < luggageHeight / 2; i += zipperHeight * 1.5){
        myZipper(luggageWidth / 2.31 + 0.01f, i, 0.005f, zipperWidth, zipperHeight, 0.01f, 0.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.885f + (-openAngle / 120), 0.0f, -0.012f + (-openAngle / 31));
    glRotatef(180, 0.0f, 1.0f, 0.0f);
    for (float i = -0.74; i < luggageHeight / 2; i += zipperHeight * 1.5){
        myZipper(luggageWidth / 2.31 + 0.01f, i, -0.005f, zipperWidth, zipperHeight, 0.01f, 0.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPushMatrix();
    for (float i = -luggageWidth / 2.25; i < luggageWidth / 2.2; i += zipperHeight * 1.5){
        myZipper(i, -luggageHeight / 2.03 - 0.01f, 0.005f, zipperWidth, zipperHeight, 0.01f, 90.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f + (-openAngle / 300), 0.0f, -0.01f + (-openAngle / 65));
    glRotatef(180 - (-openAngle * 2.3), 0.0f, 1.0f, 0.0f);
    for (float i = -0.415; i < luggageWidth / 2.2; i += zipperHeight * 1.5){
        myZipper(i, -luggageHeight / 2.03 - 0.01f, -0.005f, zipperWidth, zipperHeight, 0.01f, 90.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    myZipperTag(-luggageWidth / 2.2f, luggageHeight / 2.5f, 0.005f, tagWidth, tagHeight, tagLength, 90);
    myZipperTag(tagX, tagY, 0.017f, tagWidth, tagHeight, tagLength, 90);

    float wheelX = (luggageWidth / 2 + wheelThickness / 2) / 1.3;
    float wheelY = -luggageHeight / 1.75 - wheelRadius / 2;
    float wheelZ = luggageDepth / 2.1 - wheelRadius;

    myWheel1(-wheelX, wheelY, wheelZ - 0.03, 172, 0.8, 0.04, 0.2); // Front - left wheel
    myWheel1(-wheelX, wheelY, wheelZ, 172, 0.8, 0.04, 0.2);
    myWheel1(wheelX, wheelY, wheelZ - 0.03, -172, 0.8, 0.04, 0.2); // Front - right wheel
    myWheel1(wheelX, wheelY, wheelZ, -172, 0.8, 0.04, 0.2);
    myWheel1(-wheelX + (-openAngle / 95), wheelY, -wheelZ + (openAngle * 0.0005) + 0.03, 172, 0.8, 0.04, 0.2); // Back - left wheel
    myWheel1(-wheelX + (-openAngle / 95), wheelY, -wheelZ + (openAngle * 0.0005), 172, 0.8, 0.04, 0.2); // Back - left wheel
    myWheel1(wheelX + (-openAngle / 52), wheelY, -wheelZ + (-openAngle / 35) + 0.03, -172, 0.8, 0.04, 0.2); // Back - right wheel
    myWheel1(wheelX + (-openAngle / 52), wheelY, -wheelZ + (-openAngle / 35), -172, 0.8, 0.04, 0.2); // Back - right wheel

    myWheel2(-wheelX, wheelY, wheelZ - 0.03, 0.04, 0.2); // Front - left wheel
    myWheel2(-wheelX, wheelY, wheelZ, 0.04, 0.2);
    myWheel2(wheelX, wheelY, wheelZ - 0.03, 0.04, 0.2); // Front - right wheel
    myWheel2(wheelX, wheelY, wheelZ, 0.04, 0.2);
    myWheel2(-wheelX + (-openAngle / 95), wheelY, -wheelZ + (openAngle * 0.0005) + 0.03, 0.04, 0.2); // Back - left wheel
    myWheel2(-wheelX + (-openAngle / 95), wheelY, -wheelZ + (openAngle * 0.0005), 0.04, 0.2); // Back - left wheel
    myWheel2(wheelX + (-openAngle / 52), wheelY, -wheelZ + (-openAngle / 35) + 0.03, 0.04, 0.2); // Back - right wheel
    myWheel2(wheelX + (-openAngle / 52), wheelY, -wheelZ + (-openAngle / 35), 0.04, 0.2); // Back - right wheel

    glutSwapBuffers();
}

void myTimer(int value){
    angleY += 0.2f; // Increment rotation angle along the Y-axis
    if (angleY > 360.0f){
        angleY -= 360.0f;
    }

    glutPostRedisplay(); // Post re-paint request to activate myDisplay
    glutTimerFunc(16, myTimer, 0); // Next timer call in 16 milliseconds
}

void myArrow_keys(int a_keys, int x, int y){
    switch (a_keys){
        case GLUT_KEY_F1:
            if (zoom < maxZoom){
                zoom += 0.1f;
            }
            break;
        case GLUT_KEY_F2:
            if (zoom > minZoom){
                zoom -= 0.1f;
            }
            break;
        case GLUT_KEY_F3:
            if (shaftLength1 < MAX_LENGTH) shaftLength1 += 0.005f;
            if (shaftLength2 < MAX_LENGTH) shaftLength2 += 0.005f;
            break;
        case GLUT_KEY_F4:
            if (shaftLength1 > MIN_LENGTH) shaftLength1 -= 0.005f;
            if (shaftLength2 > MIN_LENGTH) shaftLength2 -= 0.005f;
            break;
        case GLUT_KEY_F5:
            if(chairOpen){
                if (chairSupport1 < MAX_CHAIR_SUPPORT) chairSupport1 += 0.005f;
                if (chairSupport2 < MAX_CHAIR_SUPPORT) chairSupport2 += 0.005f;
            }
            break;
        case GLUT_KEY_F6:
            if (chairSupport1 > MIN_CHAIR_SUPPORT) chairSupport1 -= 0.005f;
            if (chairSupport2 > MIN_CHAIR_SUPPORT) chairSupport2 -= 0.005f;
            break;
        case GLUT_KEY_F7:
            if (weightSupport1 < MAX_WEIGHT_SUPPORT) weightSupport1 += 0.005f;
            if (weightSupport2 < MAX_WEIGHT_SUPPORT) weightSupport2 += 0.005f;
            break;
        case GLUT_KEY_F8:
            if (weightSupport1 > MIN_WEIGHT_SUPPORT) weightSupport1 -= 0.005f;
            if (weightSupport2 > MIN_WEIGHT_SUPPORT) weightSupport2 -= 0.005f;
            break;
        case GLUT_KEY_UP:
            angleX += 2.0f;
            break;
        case GLUT_KEY_DOWN:
            angleX -= 2.0f;
            break;
        case GLUT_KEY_LEFT:
            angleY += 2.0f;
            break;
        case GLUT_KEY_RIGHT:
            angleY -= 3.0f;
            break;
    }
    glutPostRedisplay();
}

void myKeyboard(unsigned char key, int x, int y){
    switch(key){
        case 'R':
        case 'r':
            if (isRedActive){
                luggageColor[0] = 0.53f; luggageColor[1] = 0.81f; luggageColor[2] = 0.92f;
                isRedActive = false;
            }
            else {
                luggageColor[0] = 1.0f; luggageColor[1] = 0.25f; luggageColor[2] = 0.25f;
                isRedActive = true;
            }
            isGreenActive = false;
            isYellowActive = false;
            break;

        case 'G':
        case 'g':
            if (isGreenActive){
                luggageColor[0] = 0.53f; luggageColor[1] = 0.81f; luggageColor[2] = 0.92f;
                isGreenActive = false;
            }
            else {
                luggageColor[0] = 0.0f; luggageColor[1] = 1.0f; luggageColor[2] = 0.5f;
                isGreenActive = true;
            }
            isRedActive = false;
            isYellowActive = false;
            break;

        case 'Y':
        case 'y':
            if(isYellowActive){
                     luggageColor[0] = 0.53f; luggageColor[1] = 0.81f; luggageColor[2] = 0.92f;
                isYellowActive = false;
            }
            else{
                     luggageColor[0] = 0.9f; luggageColor[1] = 0.9f; luggageColor[2] = 0.1f;
                isYellowActive = true;
            }
            isRedActive = false;
            isGreenActive = false;
            break;

        case 'P':
        case 'p':
            if (!lidOpening){
                lidOpening = true;
            }
            break;
    }
    glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        animationStep++;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        if (chairSupport1 <= MIN_CHAIR_SUPPORT && chairSupport2 <= MIN_CHAIR_SUPPORT)
        {
            chairOpen = !chairOpen;
        }
    }
    glFlush();
}

void updateAnimation(int value) {

     if (lidOpening) {
        if (!lidIsOpen) {
            lidAngle += 2.0f;
            if (lidAngle >= 75.0f) {
                lidAngle = 75.0f;
                lidOpening = false;
                lidIsOpen = true;
            }
        } else {
            lidAngle -= 2.0f;
            if (lidAngle <= 0.0f) {
                lidAngle = 0.0f;
                lidOpening = false;
                lidIsOpen = false;
            }
        }
     }

    switch (animationStep){
        case 1:
            if (tagY < 0.75f){
                tagY += 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 2:
            if (tagX < 0.45f){
                tagX += 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 3:
            if (tagY > -0.75f){
                tagY -= 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 4:
            if (tagX > -0.45f){
                tagX -= 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 5:
            if (tagY < -0.6f){
                tagY += 0.03f;
            }
            else {
                isOpen = true;
            }
            break;

        case 6:
            if (isOpen){
                isOpen = false;
                animationStep++;
            }
            break;

        case 7:
            if (tagY > -0.75f){
                tagY -= 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 8:
            if (tagX < 0.45f){
                tagX += 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 9:
            if (tagY < 0.75f){
                tagY += 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 10:
            if (tagX > -0.45f){
                tagX -= 0.03f;
            }
            else {
                animationStep++;
            }
            break;

        case 11:
            if (tagY > 0.6f){
                tagY -= 0.03f;
            }
            else {
                animationStep = 0;
            }
            break;
    }

    if (isOpen && openAngle < 10) {
        openAngle += 2.0f;
    }

    else if (!isOpen && openAngle > 0) {
        openAngle -= 2.0f;
    }

    else if (chairOpen && chairAngle <= 90){
        chairAngle += 3;
    }

    else if (!chairOpen && chairAngle >= 0){
        chairAngle -= 3;
    }

    glutPostRedisplay();
    glutTimerFunc(30, updateAnimation, 0);
}

void myReshape(int w, int h){
    glViewport(0, 0, w, h); // Set the view port to cover the new window
    glMatrixMode(GL_PROJECTION); // Operate on the Projection Matrix
    glLoadIdentity(); // Reset
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50); // Position the window's initial top - left corner
    glutInitWindowSize(800, 600); // Set the window's initial width & height
    glutCreateWindow("VoyagePal"); // Create window with the given title
    myInit(); // OpenGL initialization
    glutDisplayFunc(myDisplay); // Register callback handler for window re-paint event
    glutReshapeFunc(myReshape); // Register callback handler for window re-size event
    glutSpecialFunc(myArrow_keys);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    glutTimerFunc(16, myTimer, 0); // First timer call immediately
    glutTimerFunc(0, updateAnimation, 0);
    glutMainLoop(); // Enter the infinite event-processing loop
    return 0;
}
