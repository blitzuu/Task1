#include "glos.h"
#include "gl.h"
#include "glaux.h"
#include "glut.h"
#include <iostream>
#include <cmath>

#define M_PI 3.14159

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat lastX = 0.0f;
GLfloat lastY = 0.0f;
bool mousePressed = false;

// Coordonatele atomilor de carbon
GLfloat carbonPositions[6][3] = {
    {-0.9f, -0.3f, 0.0f},   // stanga jos
    {-0.9f, 0.6f, 0.0f},    // stanga sus
    {0.0f, 1.0f, 0.0f},     // cel mai sus atom
    {0.9f, 0.6f, 0.0f},     // dreapta sus
    {0.9f, -0.3f, 0.0f},    // dreapta jos
    {0.0f, -0.8f, 0.0f}     // cel mai de jos atom
};

// Coordonatele atomilor de hidrogen
GLfloat hydrogenPositions[6][3] = {
    {-1.7f, -0.8f, 0.0f},    // stanga jos
    {-1.7f, 1.1f, 0.0f},     // stanga sus
    {0.0f, 1.7f, 0.0f},      // cel mai sus atom
    {1.7f, 1.1f, 0.0f},      // dreapta sus
    {1.7f, -0.8f, 0.0f},     // dreapta jos
    {0.0f, -1.5f, 0.0f}      // cel mai de jos atom
};

// Function to set material properties for a shiny surface
void setMaterialProperties(GLfloat r, GLfloat g, GLfloat b) {
    GLfloat matAmbient[] = { r * 0.2f, g * 0.2f, b * 0.2f, 1.0f };
    GLfloat matDiffuse[] = { r, g, b, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

// Function to draw a carbon atom
void drawCarbonAtom(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    setMaterialProperties(0.0f, 0.5f, 0.6f); 
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
}

// Function to draw a hydrogen atom
void drawHydrogenAtom(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    setMaterialProperties(1.0f, 1.0f, 1.0f); // Cyan
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();
}

// Function to draw a bond between two atoms
// Funcție pentru desenarea unei legături între două atomi
void drawBond(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2) {
    GLUquadric* quadric = gluNewQuadric();
    glPushMatrix();

    // Calculăm vectorul direcției
    GLfloat dx = x2 - x1;
    GLfloat dy = y2 - y1;
    GLfloat dz = z2 - z1;
    GLfloat length = sqrt(dx * dx + dy * dy + dz * dz);

    // Calculăm unghiul de rotație
    GLfloat angle = acos(dz / length) * 180.0 / M_PI;
    GLfloat axisX = -dy;
    GLfloat axisY = dx;
    GLfloat axisZ = 0.0f;

    // Translatarea la poziția inițială
    glTranslatef(x1, y1, z1);

    // Rotația pentru a alinia cilindrul
    glRotatef(angle, axisX, axisY, axisZ);

    GLfloat matAmbient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat matDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // Desenăm cilindrul (legătura)
    gluCylinder(quadric, 0.03, 0.03, length, 20, 20);

    glPopMatrix();
    gluDeleteQuadric(quadric);
}



// Function to draw the benzene molecule
void drawBenzene() {
    // Desenăm legăturile carbon-carbon
    for (int i = 0; i < 6; ++i) {
        drawBond(carbonPositions[i][0], carbonPositions[i][1], carbonPositions[i][2],
            carbonPositions[(i + 1) % 6][0], carbonPositions[(i + 1) % 6][1], carbonPositions[(i + 1) % 6][2]);
    }

    // Desenăm legăturile carbon-hidrogen
    for (int i = 0; i < 6; ++i) {
        drawBond(carbonPositions[i][0], carbonPositions[i][1], carbonPositions[i][2],
            hydrogenPositions[i][0], hydrogenPositions[i][1], hydrogenPositions[i][2]);
    }

    // Desenăm atomii de carbon
    for (int i = 0; i < 6; ++i) {
        drawCarbonAtom(carbonPositions[i][0], carbonPositions[i][1], carbonPositions[i][2]);
    }

    // Desenăm atomii de hidrogen
    for (int i = 0; i < 6; ++i) {
        drawHydrogenAtom(hydrogenPositions[i][0], hydrogenPositions[i][1], hydrogenPositions[i][2]);
    }
}

// Funcție pentru gestionarea evenimentelor mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mousePressed = true;
        lastX = x;
        lastY = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mousePressed = false;
    }
}

// Funcție pentru gestionarea mișcării mouse-ului
void motion(int x, int y) {
    if (mousePressed) {
        rotationX += (y - lastY) * 0.1f;
        rotationY += (x - lastX) * 0.1f;
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

// Funcție pentru afișarea scenei
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // Activăm iluminarea și setăm proprietățile luminii
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Setăm proprietățile luminii
    GLfloat lightPos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Desenăm molecula de benzen
    drawBenzene();

    // Desenăm numele studentului
    glRasterPos2f(-1.5f, -2.5f);
    std::string studentName = "Mihai Zabava";
    for (char& c : studentName) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glFlush();
    glutSwapBuffers();
}

// Funcție pentru redimensionarea ferestrei
void myReshape(GLsizei w, GLsizei h) {
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Funcție pentru inițializarea OpenGL
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Setăm fundalul la alb
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); // Activăm iluminarea
    glEnable(GL_LIGHT0); // Activăm lumina 0

    // Setăm proprietățile luminii
    GLfloat lightPos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

// Funcție principală
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Utilizăm GLUT_DOUBLE pentru buffering-ul dublu
    glutInitWindowSize(800, 600);
    glutCreateWindow("Molecula de benzen");

    init(); // Apelăm funcția de inițializare

    glutDisplayFunc(display); // Funcția de afișare
    glutReshapeFunc(myReshape); // Funcția de redimensionare fereastră
    glutMouseFunc(mouse); // Funcția de gestionare a evenimentelor mouse
    glutMotionFunc(motion); // Funcția de gestionare a mișcării mouse-ului

    glutMainLoop();
    return 0;
}
