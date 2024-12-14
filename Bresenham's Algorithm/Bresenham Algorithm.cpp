#include <freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
    float x, y;
};

vector<vector<Point>> lines;
vector<Point> currentLine;
vector<string> displayedCoordinates;
float m;


void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void drawGrid() {
    glColor3f(0.8, 0.8, 0.8);
    glLineWidth(1.0);

    glBegin(GL_LINES);

    // Draw vertical grid lines
    for (float x = -100.0; x <= 100.0; x += 10.0) {
        glVertex2f(x, -100.0);
        glVertex2f(x, 100.0);
    }

    // Draw horizontal grid lines
    for (float y = -100.0; y <= 100.0; y += 10.0) {
        glVertex2f(-100.0, y);
        glVertex2f(100.0, y);
    }

    glEnd();
}


void drawLine(const Point& p1, const Point& p2) {

    float x, y, p, dx, dy, temp;
    float x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;

    if ((x2 - x1) == 0) {
        m = (y2 - y1);
    }
    else {
        m = (y2 - y1) / (x2 - x1);
    }

    

    if (fabs(m) < 1) {
        

        if (x1 > x2) {

            drawLine(p2, p1);

            
        }
        


        

        /*cout << m << endl;*/

        dx = fabs(x2 - x1);
        dy = fabs(y2 - y1);

        p = 2 * dy - dx;

        x = x1;
        y = y1;

        glBegin(GL_POINTS);
        for (x = x1; x <= x2; x += 0.1f) {
            glVertex2f(x, y);

            
            if (p > 0) {

                if (m < 0) {
                    y = y - 0.1f;
                }
                else {
                    y = y + 0.1f;
                }
                
                p = p + 2 * dy - 2 * dx;
            }
            else {
                y = y;
                p = p + 2 * dy;
            }

        }
    }



    if (fabs(m) >= 1) {


        if (y1 > y2) {

            drawLine(p2, p1);
           
        }

        

        dx = fabs(x2 - x1);
        dy = fabs(y2 - y1);

        p = 2 * dx - dy;

        x = x1;
        y = y1;

        glBegin(GL_POINTS);
        for (y = y1; y <= y2; y += 0.1f) {
            glVertex2f(x, y);
            
            if (p > 0) {

                if (m >= 1) {
                    x = x + 0.1f;
                }
                else {
                    x = x - 0.1f;
                }

                p = p + 2 * dx - 2 * dy;
            }
            else {
                y = y;
                p = p + 2 * dx;
            }

        }
    }
    glEnd();

    
}

// Function to display content
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer


    drawGrid();
    

    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(2.5f);

    for (const auto& line : lines) {
        if (line.size() == 2) {
            drawLine(line[0], line[1]);
        }
    }

    for (size_t i = 0; i < displayedCoordinates.size(); i++) {
        if (i < lines.size() * 2) {
            Point point;
            if (i % 2 == 0) {

                point = lines[i / 2][0];
            }
            else {

                point = lines[i / 2][1];
            }
            renderBitmapString(point.x, point.y, GLUT_BITMAP_HELVETICA_18, displayedCoordinates[i].c_str());
        }
    }

    char scoreText[50];
    sprintf_s(scoreText, "Slope: %.2f", m);
    renderBitmapString(-90.0f, -90.0f, GLUT_BITMAP_HELVETICA_18, scoreText);
    renderBitmapString(40.0f, -90.0f, GLUT_BITMAP_HELVETICA_18, "Love Dewangan 500109339");
    glFlush(); // Render now
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);

        
        float x_pos = (x / (float)width) * 200.0f - 100.0f;
        float y_pos = ((height - y) / (float)height) * 200.0f - 100.0f;

        cout << "x: " << x_pos << " y: " << y_pos << endl;
        
        char coordinateText[50];
        sprintf_s(coordinateText, "(%.2f , %.2f)", x_pos, y_pos);


        displayedCoordinates.push_back(coordinateText);

        currentLine.push_back({ x_pos, y_pos });

        if (currentLine.size() == 2) {
            
            lines.push_back(currentLine);
            currentLine.clear();
        }

        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lines.clear();
        currentLine.clear();
        glutPostRedisplay();
    }
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0); 
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    // Set the display mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // Set the window size
    glutInitWindowSize(1000, 1000);
    // Set the window position
    glutInitWindowPosition(100, 100);
    // Create the window with a title
    glutCreateWindow("Bresenham's Algorithm");

    init();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);

    glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}
