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
    // I want this to happen every so often 
    if (time % 3 == 0){
      sceneParticles.push_back(mParticles[0]);
      mParticles.erase(mParticles.begin());
      // initialized to be same start pos as the rotating particle 
      sceneParticles[sceneParticles.size()-1].pos = position; 
      // resetting to orig values 
      sceneParticles[sceneParticles.size()-1].color = vec4(agl::randomUnitCube(), 1);
      sceneParticles[sceneParticles.size()-1].rot = 0.0;
    }
 
    if (sceneParticles.size() > 1){
      for (int i = 0; i < sceneParticles.size()-2; i++){
        Particle* particle = &sceneParticles[i];
        // hopefully it shoots off in a straight line (?)
        particle->pos = particle->pos + dt() * particle->vel;
        // lowers the transparency based on time 
        particle->color.w = fmaxf(0, particle->color.w-0.005);

        // change the rotation slightly 
        particle->rot = particle->rot + 0.01; 

        // if transparency is 0 then delete particle from scene 
        // put back in mParticles for reusing
        if (particle->color.w <= 0){
          deadParticles.push_back(i);
        } 
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

  void drawConfetti()
  {
    renderer.texture("image", "particle");

    if (sceneParticles.size() != 0){
      for (Particle particle : sceneParticles){
        renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
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
    position = vec3(cos(theta), sin(theta), position.z);
    theta = theta+0.01; 
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

  float theta = 0.1; // for changing theta of circle  
  int time = 0; 

  // partilcles not in the scene 
  std::vector<Particle> mParticles;

  // particles in the scene 
  std::vector<Particle> sceneParticles; 
  // indicies of particles in scene that are dead - to be moved to mParticles
  // in update 
  vector<int> deadParticles; 
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
