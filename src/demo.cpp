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
  float blasttime;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void initParticleList(){
      mParticles.clear();
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
          particle.blasttime = particle.delayby+3;
          mParticles.push_back(particle);
      }

  }

  void createConfetti()
  {
    renderer.loadTexture("particle", "../textures/circle.png", 0);
    initParticleList();
   
  }

  void blastParticle(Particle parent){
      for (int i = 0; i < 50; i++){
          Particle particle;
          particle.color = vec4(agl::randomUnitCube() + vec3(0.5), 1);
          particle.size = 0.05;
          particle.pos = parent.pos;
          particle.vel = vec3(random(-0.3, 0.3), random(-0.3, 0.3), 0);
          particle.isEnabled = true;
          particle.blasted = true;
          particle.delayby = 0;
          particle.blasttime = 0;
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
    time += dt();

      bool allsizezero = true;
      for (Particle& particle : mParticles) {
          if (particle.isEnabled)
          {
              if (particle.blasted == false && time > particle.blasttime)
              {
                  particle.size = 0.05;
                  particle.blasted = true;
                  blastParticle(particle);
                  return;
              }

              if (particle.blasted)
              {
                  particle.size -= 0.0005;
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

      if (allsizezero)
      {
          initParticleList();
          time = 0;
      }

      /*
    for (Particle &particle:mParticles){
      if (particle.isEnabled){
        particle.pos = particle.pos + dt() * particle.vel;
        particle.vel = particle.vel + acceleration*dt();
        cout << particle.vel << endl;

        if (particle.vel.y < 0.05){
          particle.isEnabled = false; 
          cout << "particle false: " << particle.isEnabled << endl;
        }
      }
    }

    if (time % 7 == 0){
      bool isDone = false;
      for (Particle &particle: mParticles){
        if(isDone){
          break;
        }
        else if (!particle.isEnabled){
          float randVal = agl::random(-1.5,1.5);
          particle.pos = vec3(randVal, -1.6, 0);
          particle.vel = vec3(0.1, agl::random(0.5,1.2), 0);
          // i.color = vec4(agl::random(155, 255)/255, agl::random(155, 255)/255, agl::random(155, 255)/255, 1);
          particle.color = vec4(agl::randomUnitCube(), 1);
          particle.color.x = particle.color.x+0.5; 
          particle.color.y = particle.color.y+0.5; 
          particle.color.z = particle.color.z+0.5; 
          particle.isEnabled = true;
          cout << "particle true: " <<  particle.isEnabled << endl;
          isDone = true; 
        }
      }
    }

    time += 1; 
    */

  }

  void drawFireworks()
  {
    renderer.texture("image", "particle");

    if (mParticles.size() != 0){
      for (const Particle &particle : mParticles){
        if (particle.isEnabled == true && particle.size > 0){
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

  float time = 0; 

  int size = 5;

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

