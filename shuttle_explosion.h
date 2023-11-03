#ifndef SHUTTLE_EXPLOSION_H
#define SHUTTLE_EXPLOSION_H

#include <GL/glut.h>
// #include <iostream>
#include <cmath>
#include <vector>

class Particle {
public:
    float x;
    float y;
    float speedX;
    float speedY;

    Particle(float px, float py, float sx, float sy) : x(px), y(py), speedX(sx), speedY(sy) {}
};

class Explosion {
public:
    std::vector<Particle> particles;

    void createExplosion(float x, float y) {
        particles.clear();
        for (int i = 0; i < numParticles; i++) {
            float angle = 2 * M_PI * (rand() % 360) / 360.0;
            float speedX = particleSpeed * cos(angle) * (rand() % 100 / 100.0);
            float speedY = particleSpeed * sin(angle) * (rand() % 100 / 100.0);
            particles.push_back(Particle(x, y, speedX, speedY));
        }
    }

    void applyGravity() {
        for (auto& particle : particles) {
            particle.speedY += gravity;
            gravity += -0.000003f;
        }
    }

    void draw() {
        glPointSize(5);
        glBegin(GL_POINTS);

        applyGravity();
        for (auto& particle : particles) {
            particle.x += particle.speedX;
            particle.y += particle.speedY;
            glColor3f(1.0, 1.0, 0.0);
            glVertex2f(particle.x, particle.y);
        }

        glEnd();
    }

    bool allParticlesBelowWindow() {
        for (const auto& particle : particles) {
            if (particle.y > -1.0f) {
                return false;
            }
        }
        return true;
    }

private:
    float particleSpeed = 0.05f;
    float gravity = -0.00005f;
    int numParticles = 40;
};

std::vector<Explosion*> explosions;

void createExplosionInstance(float x, float y) {
    Explosion* newExplosion = new Explosion;
    newExplosion->createExplosion(x, y);
    explosions.push_back(newExplosion);
}

// void keyPressed(unsigned char key, int x, int y) {
//     if (key == ' ') {
//         createExplosionInstance(5.0f, 3.0f);
//     }
// }

void drawExplosion() {
    // glClear(GL_COLOR_BUFFER_BIT);

    for (size_t i = 0; i < explosions.size(); i++) {
        explosions[i]->draw();
        if (explosions[i]->allParticlesBelowWindow()) {
            delete explosions[i];
            explosions.erase(explosions.begin() + i);
            i--;
        }
    }

    // glFlush();
}

// void update(int value) {
//     glutTimerFunc(16, update, 0);
//     glutPostRedisplay();
// }

// int main(int argc, char** argv) {
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//     glutInitWindowSize(800, 600);
//     glutInitWindowPosition(100, 100);
//     glutCreateWindow("2D Explosion Animation");
//     glClearColor(0.0, 0.0, 0.0, 1.0);
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     glutInitWindowSize(800, 600);
//     glOrtho(0.0, 10.0, 0.0, 6.0, -1.0, 1.0);
//     glutDisplayFunc(drawExplosion);
//     glutKeyboardFunc(keyPressed);
//     glutTimerFunc(0, update, 0);

//     glutMainLoop();
//     return 0;
// }

#endif