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
  bool blasted;
  float delayby;
  std::vector<Particle> children;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(5);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti(int size)
  {
    renderer.loadTexture("particle", "../textures/circle.png", 0);
     for (int i = 0; i < size; i++){
          Particle particle;
          particle.color = vec4(agl::randomUnitCube() + vec3(0.5), 1);
          particle.size = 0.05;
          float randVal = agl::random(-1.7, 1.7);
          particle.pos = vec3(randVal, -1.7, 0);
          if (particle.pos.x < 0)
              particle.vel = vec3(0.3, 0.6, 0);
          else
              particle.vel = vec3(-0.3, 0.6, 0);

          particle.isEnabled = false;
          particle.blasted = false;
          particle.delayby = i;
          for (int j = 0; j < 50; j++){
            Particle child;
            child.color = vec4(agl::randomUnitCube() + vec3(0.5), 1);
            child.size = 0.05;
            child.pos = vec3(0);
            child.vel = vec3(random(-0.3, 0.3), random(-0.3, 0.3), 0);
            child.isEnabled = false;
            child.blasted = true;
            child.delayby = 0;
            particle.children.push_back(child);
          }
          mParticles.push_back(particle);
      }
  }

  void resetParticles(){
    for (Particle& particle : mParticles){
      particle.size = 0.05;
      float randVal = agl::random(-1.7, 1.7);
      particle.pos = vec3(randVal, -1.7, 0);
      if (particle.pos.x < 0)
          particle.vel = vec3(0.3, 0.6, 0);
      else
          particle.vel = vec3(-0.3, 0.6, 0);

      particle.isEnabled = false;
      particle.blasted = false;
      for (Particle &child: particle.children){
        child.size = 0.05;
        child.vel = vec3(random(-0.3, 0.3), random(-0.3, 0.3), 0);
        child.isEnabled = false;
        child.blasted = true;
      }
    }
  }

  void blastParticle(Particle parent){
      for (Particle &child: parent.children){
          child.pos = parent.pos;
          child.isEnabled = true;
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
    time += dt();

      bool allsizezero = true;
      for (Particle& particle : mParticles) {
          if (particle.isEnabled)
          {
              if (particle.blasted == false && particle.vel.y <= 0.1)
              {
                  particle.size = 0.05;
                  particle.blasted = true;
                  blastParticle(particle);
                  return;
              }

              if (particle.blasted)
              {
                  particle.size -= 0.0005;
                  for (Particle& child: particle.children){
                    child.pos += child.vel * dt();
                    child.vel += acceleration * dt();
                    child.size -= 0.0005;

                    if (allsizezero && child.size > 0)
                    {
                        allsizezero = false;
                    }
                  }
              }
              particle.pos += particle.vel * dt();
              particle.vel += acceleration * dt();
          }
          else
          {
              if (time > particle.delayby)
              {
                  particle.isEnabled = true;
              }
          }
          if (allsizezero && particle.size > 0)
          {
              allsizezero = false;
          }
      }

      if (allsizezero){
          resetParticles();
          time = 0;
      }

  }

  void drawFireworks()
  {
    renderer.texture("image", "particle");

    if (mParticles.size() != 0){
      for (Particle &particle : mParticles){
        if (particle.isEnabled == true && particle.size > 0){
          renderer.sprite(particle.pos, particle.color, particle.size);
          for (Particle &child: particle.children){
            if (child.isEnabled == true && child.size > 0){
              renderer.sprite(child.pos, child.color, child.size);
            }
          }
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

  float time = 0; 

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

