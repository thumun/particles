//--------------------------------------------------
// Author: Neha Thumu
// Date: 4/7/23
// Description:
// Creating fireworks particle effect!
// [Using the code from sparkle-trail as a base]
// Basically have 2 sets of particles: parents & childen
// the parents are the initial particles (firework going up)
// children are the particles that happen after the blast (and parent joins
// in falling)
// particles are recycled when there's no more particles that are visible
// not visible = size is <= 0
//--------------------------------------------------

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle
{
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float size;
  bool isEnabled; // if firework is visible essentially 
  bool blasted; // if the firework has exploded 
  float delayby; // time between fireworks (don't want all at same time)
  std::vector<Particle> children; // firework's blast particles 
};

class Viewer : public Window
{
public:
  Viewer() : Window()
  {
  }

  void setup()
  {
    setWindowSize(1000, 1000);
    createFirework(5);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  // initalizes the fireworks 
  // parent: 
  //    makes sure starting pos off screen but will still be visible 
  //    goes left or right based on x pos (x vel changes)
  //    delay is based on index 
  // child: 
  //    each parent has children vector 
  //    pos doesn't matter since gonna set when blast happens 
  //    velocity random and small to get firework blast effect 
  //    no need for a delay since want them all to go off together 
  void createFirework(int size)
  {
    renderer.loadTexture("particle", "../textures/circle.png", 0);
    for (int i = 0; i < size; i++)
    {
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
      for (int j = 0; j < 50; j++)
      {
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

  // recycling mechanic 
  // when particles have size 0 they're reset 
  // values changed during programs become default (same as in setup)
  void resetParticles()
  {
    for (Particle &particle : mParticles)
    {
      particle.size = 0.05;
      float randVal = agl::random(-1.7, 1.7);
      particle.pos = vec3(randVal, -1.7, 0);
      if (particle.pos.x < 0)
        particle.vel = vec3(0.3, 0.6, 0);
      else
        particle.vel = vec3(-0.3, 0.6, 0);

      particle.isEnabled = false;
      particle.blasted = false;
      for (Particle &child : particle.children)
      {
        child.size = 0.05;
        child.vel = vec3(random(-0.3, 0.3), random(-0.3, 0.3), 0);
        child.isEnabled = false;
        child.blasted = true;
      }
    }
  }

  // intiailizes the blast particles (children of parent firework)
  // pos becomes the parent's pos (so bright too :) b/c of add)
  void blastParticle(Particle &parent)
  {
    for (Particle &child : parent.children)
    {
      child.pos = parent.pos;
      child.isEnabled = true;
    }
  }

  void updateFirework()
  {
    time += dt(); // time allows new fireworks to start 

    bool allsizezero = true; // to see if size is 0 for every particle 
                            // including children!!  
    for (Particle &particle : mParticles)
    {
      if (particle.isEnabled)
      {
        // if firework is still whole but reached end of life 
        // (velocity in y = 0)
        // then time to blast ! (sets up children too)
        if (particle.blasted == false && particle.vel.y <= 0.1)
        {
          particle.size = 0.05;
          particle.blasted = true;
          blastParticle(particle);
          return;
        }

        // once in blast: particles become smaller 
        // and head off in their respective directions based on prev positions
        // parent acts like child --> becomes smaller and heads off in direction
        if (particle.blasted)
        {
          particle.size -= 0.0005;
          for (Particle &child : particle.children)
          {
            child.pos += child.vel * dt();
            child.vel += acceleration * dt();
            child.size -= 0.0005;

            if (allsizezero && child.size > 0)
            {
              allsizezero = false;
            }
          }
        }
        // (parent moved in either case)
        particle.pos += particle.vel * dt();
        particle.vel += acceleration * dt();
      }
      else
      {
        // setting up new firework 
        if (time > particle.delayby)
        {
          particle.isEnabled = true;
        }
      }
      // if a particle of size > 0 exists then bool is false 
      if (allsizezero && particle.size > 0)
      {
        allsizezero = false;
      }
    }

    // otherwise - time to reset 
    if (allsizezero)
    {
      resetParticles();
      time = 0;
    }
  }

  // renders the fireworks
  // & renders their blast particles if firework has exploded
  void drawFireworks()
  {
    renderer.texture("image", "particle");

    if (mParticles.size() != 0)
    {
      for (Particle &particle : mParticles)
      {
        if (particle.isEnabled == true && particle.size > 0)
        {
          renderer.sprite(particle.pos, particle.color, particle.size);
          for (Particle &child : particle.children)
          {
            if (child.isEnabled == true && child.size > 0)
            {
              renderer.sprite(child.pos, child.color, child.size);
            }
          }
        }
      }
    }
  }

  void mouseMotion(int x, int y, int dx, int dy)
  {
  }

  void mouseDown(int button, int mods)
  {
  }

  void mouseUp(int button, int mods)
  {
  }

  void scroll(float dx, float dy)
  {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods)
  {
  }

  void draw()
  {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);

    updateFirework();
    drawFireworks();
    renderer.endShader();
  }

protected:
  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);

  float time = 0;

  vec3 acceleration = vec3(0, -0.1, 0); // gravity

  std::vector<Particle> mParticles;
};

int main(int argc, char **argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
