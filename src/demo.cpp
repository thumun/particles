//--------------------------------------------------
// Author: Neha Thumu
// Date: 
// Description: 
//--------------------------------------------------

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
  float size;
  bool isEnabled; 
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(100);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti(int size)
  {
    renderer.loadTexture("particle", "../textures/circle.png", 0);
    for (int i = 0; i < size; i++)
    {
      Particle particle;
      particle.color = vec4(agl::randomUnitCube(), 1);
      particle.size = 0.25;
      // particle.pos = agl::randomUnitCube();
      float randVal = agl::random(-1.5,1.5);
      particle.pos = vec3(randVal, -1.6, 0);
      // print start pos 
      // cout << particle.pos << endl;
      // make gravity 0 to see if moving correctly 
      // particle.vel = agl::randomUnitCube();
      particle.vel = vec3(0.1, agl::random(0.3,0.9), 0);
      particle.isEnabled = false;
      mParticles.push_back(particle);
    }
  }

  // void createTrail(){

  //   trailingParticles.clear();

  //   for (int i = 0; i < sceneParticles.size(); i++){
  //     Particle p1;
  //     p1.color = sceneParticles[i].color;
  //     p1.size = sceneParticles[i].size;
  //     p1.pos = sceneParticles[i].pos; // need to change slightly
  //     p1.vel = sceneParticles[i].vel;

  //     trailingParticles.push_back(p1); // need to make more
  //   }
  // }

  // testing initial firework -> no trail; just logic 
  void updateConfetti()
  {   
    for (Particle &particle:mParticles){
      if (particle.isEnabled){
        particle.pos = particle.pos + dt() * particle.vel;
        particle.vel = particle.vel + acceleration*dt();

        if (pow(particle.vel.x,2)+ pow(particle.vel.y,2) + pow(particle.vel.z,2) < 0.05){
          particle.isEnabled = false; 
          cout << "particle false: " << particle.isEnabled << endl;
        }
      }
    }

    if (time % 7 == 0){
      bool isDone = false;
      for (Particle &i: mParticles){
        if(isDone){
          break;
        }
        else if (!i.isEnabled){
          float randVal = agl::random(-1.5,1.5);
          i.pos = vec3(randVal, -1.6, 0);
          i.vel = vec3(0.1, agl::random(0.3,0.9), 0);
          i.color = vec4(agl::randomUnitCube(), 1);
          i.isEnabled = true;
          cout << "particle true: " <<  i.isEnabled << endl;
          isDone = true; 
        }
      }
    }

    time += 1; 

  }

  void drawFireworks()
  {
    renderer.texture("image", "particle");

    if (mParticles.size() != 0){
      for (Particle particle : mParticles){
        if (particle.isEnabled){
          renderer.sprite(particle.pos, particle.color, particle.size);
        }
      }
    }
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

    updateConfetti();
    drawFireworks();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);

  int time = 0; 

  vec3 acceleration = vec3(0,-0.1,0); // gravity 

  std::vector<Particle> mParticles;

  std::vector<Particle> trailingParticles; 

};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
