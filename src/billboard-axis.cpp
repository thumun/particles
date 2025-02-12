// Bryn Mawr College, alinen, 2020
//

//--------------------------------------------------
// Authors: Aline Normoyle, Neha Thumu
// Date: 4/7/23
// Description:
// Creating a tree that always faces the camera 
// (billboard axis)
// [Using code from Aline as a base]
//--------------------------------------------------

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // getting imgRatio to make sure tree is scaled properly
    imgRatio =  float(img.height())/float(img.width());
    cout << imgRatio << endl;
    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }

  // orbital camera code taken from meshViewer 
  void computeCamPos(float r, float a, float e){
    camPos.x = r*cos(a)*cos(e);
    camPos.y = r*sin(e);
    camPos.z = r*sin(a)*cos(e);
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    if(isMousePress){
      if (abs(dx) > abs(dy)){
        if (dx < 0){
            azimuth -= 0.1f; 
        } else { 
            azimuth += 0.1f; 
        }
      } else if (abs(dy) > abs(dx)) { 
        if (dy < 0){
            elevation -= 0.05f; 
        } else { 
            elevation += 0.05f; 
        }
      }

      // clamp: 
      if (elevation > M_PI/2){
        elevation = -1*M_PI/2;

      } else if (elevation < -M_PI/2){
        elevation = M_PI/2;
      }
    }
  }

  void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT){
         isMousePress = true; 
      }
  }

  void mouseUp(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT){
         isMousePress = false; 
      }
  }

  void scroll(float dx, float dy) {
  }

  void keyUp(int key, int mods) {
    if (key == GLFW_KEY_UP){
         // moving camera forward - change r
         radius += 0.5;
    } else if (key == GLFW_KEY_DOWN){
        // moving camera backward - change r
        radius -= 0.5;
    }
  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    computeCamPos(radius, azimuth, elevation);

    renderer.lookAt(camPos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    // renderer.scale(vec3(2.0f));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
     // adding rotations and scales here so it faces the camera 
    vec3 n = normalize(camPos-lookPos);
    theta = atan2(n.x, n.z);
    renderer.rotate(theta, vec3(0, 1, 0));
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.scale(vec3(1, imgRatio, 1));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 camPos = eyePos;
  bool isMousePress = false;
  float azimuth = 0; 
  float elevation = 0; 
  float radius = 10.0f;  
  float theta = 0; 
  float imgRatio = 0; 
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
