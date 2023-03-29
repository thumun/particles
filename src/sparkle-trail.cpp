// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(500);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti(int size)
  {
    renderer.loadTexture("particle", "../textures/star4.png", 0);
    for (int i = 0; i < size; i++)
    {
      Particle particle;
      particle.color = vec4(agl::randomUnitCube(), 1);
      particle.size = 0.25;
      particle.rot = 0.0;
      particle.pos = agl::randomUnitCube();
      particle.vel = agl::randomUnitCube();
      mParticles.push_back(particle);
    }
  }

  void updateConfetti()
  {
    // mParticles[0].pos = position;
    mParticles[0].pos = vec3(cos(test-dt()), sin(test-dt()), mParticles[0].pos.z);
    cout << mParticles[0].pos << endl; 

    // for (int i = 0; i < mParticles.size(); i++){
    //   Particle particle = mParticles[i];

    //   if (i == 0){
    //     particle.pos = position; 
    //   } else { 
    //     particle.pos = mParticles[i-1].pos; 
    //   }
    //   // particle.pos = particle.pos+dt()*particle.vel; 
    // }

  }

  void drawConfetti()
  {
    renderer.texture("image", "particle");
    renderer.sprite(mParticles[0].pos, mParticles[0].color, mParticles[0].size, mParticles[0].rot);

    // for (int i = 0; i < mParticles.size(); i++)
    // {
    //   Particle particle = mParticles[i];
    //   renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    // }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    position = vec3(cos(test), sin(test), position.z);
    test = test+0.01; 
    renderer.sprite(position, vec4(1.0f), 0.25f);
    updateConfetti();
    drawConfetti();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);

  float test = 0.1; // for changing theta of circle  

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
