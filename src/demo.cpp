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
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(50);
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
      // particle.pos = agl::randomUnitCube();
      float randVal = agl::random(-1.5,1.5);
      particle.pos = vec3(randVal, -1.6, 0);
      // print start pos 
      // cout << particle.pos << endl;
      // make gravity 0 to see if moving correctly 
      // particle.vel = agl::randomUnitCube();
      particle.vel = vec3(0.1, 0.5, 0);
      mParticles.push_back(particle);
    }
  }

  void createTrail(){

    trailingParticles.clear();

    for (int i = 0; i < sceneParticles.size(); i++){
      Particle p1;
      p1.color = sceneParticles[i].color;
      p1.size = sceneParticles[i].size;
      p1.pos = sceneParticles[i].pos; // need to change slightly
      p1.vel = sceneParticles[i].vel;

      trailingParticles.push_back(p1); // need to make more
    }
  }

  // testing initial firework -> no trail; just logic 
  void updateConfetti()
  {   
    if (time % 7 == 0){
      if (mParticles.size() != 0){
        sceneParticles.push_back(mParticles[0]);
        mParticles.erase(mParticles.begin());
        sceneParticles[sceneParticles.size()-1].color = vec4(agl::randomUnitCube(), 1);

      }
      
      // initialized to be same start pos as the rotating particle 
      // how to make them start under screen
      // sceneParticles[sceneParticles.size()-1].pos = position; 
    }
 
    if (sceneParticles.size() != 0){
      for (int i = 0; i < sceneParticles.size(); i++){

        Particle* particle = &sceneParticles[i];


        // vec3 newAcceleration = vec3(acceleration.x, 0.5*acceleration.y*pow(elapsedTime(),2), acceleration.z);
        particle->pos = particle->pos + dt() * particle->vel;
        particle->vel = particle->vel + acceleration*dt();

        // cout << particle->pos << endl;
        // particle->color.w = fmaxf(0, particle->color.w-0.001);


        if (pow(particle->vel.x,2)+ pow(particle->vel.y,2) + pow(particle->vel.z,2) < 0.05){
          //particle->color.w = 0;
          deadParticles.push_back(i);
        }

        // if (particle->color.w <= 0){
        //   deadParticles.push_back(i);
        // } 
      }
    }

    if (deadParticles.size() != 0){
      for (int i: deadParticles){
        mParticles.push_back(sceneParticles[i]);
        sceneParticles.erase(sceneParticles.begin() + i);
      }

      deadParticles.clear();
    }

    time+=1; 

  }

  void drawFireworks()
  {
    renderer.texture("image", "particle");

    if (sceneParticles.size() != 0){
      for (Particle particle : sceneParticles){
        renderer.sprite(particle.pos, particle.color, particle.size);
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
    // position = vec3(cos(theta), sin(theta), position.z);
    // theta = theta+0.01; 
    // renderer.sprite(position, vec4(1.0f), 0.25f);
    updateConfetti();
    drawFireworks();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);

  float theta = 0.1; // for changing theta of circle  
  int time = 0; 

  vec3 acceleration = vec3(0,-0.1,0); // gravity 

  // partilcles not in the scene 
  std::vector<Particle> mParticles;

  // particles in the scene 
  std::vector<Particle> sceneParticles; 
  // indicies of particles in scene that are dead - to be moved to mParticles
  // in update 

  std::vector<Particle> trailingParticles; 

  vector<int> deadParticles; 
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
