#include <cstdlib>
#include <ctime>
#include <iostream>
#include "color.h"
#include "vector.h"
#include "tracer.h"
#include "shape.h"
#include "camera.h"
#include "light.h"

const int width = 800;
const int height = 600;

//                    emissive    ambient                           diffuse                           specular                      shininess reflect     transparent           refract
Material mtLight   = {Color(1.0), Color(0.0),                       Color(0.0),                       Color(0.0),                       0.00, Color(0.0), Color(1.0),           1.0}; // белый источник света

Material mtBrass   = {Color(0.0), Color(0.32941, 0.22353, 0.02745), Color(0.78039, 0.56863, 0.11373), Color(0.99216, 0.94118, 0.80784), 0.22, Color(0.2), Color(0.0),           1.0}; // латунь
Material mtBronze  = {Color(0.0), Color(0.21250, 0.12750, 0.05400), Color(0.71400, 0.42840, 0.18144), Color(0.39355, 0.27191, 0.16672), 0.20, Color(0.1), Color(0.0),           1.0};
Material mtChrome  = {Color(0.0), Color(0.25000, 0.25000, 0.25000), Color(0.40000, 0.40000, 0.40000), Color(0.77460, 0.77460, 0.77460), 0.60, Color(0.4), Color(0.0),           1.0};
Material mtCopper  = {Color(0.0), Color(0.19125, 0.07350, 0.02250), Color(0.70380, 0.27048, 0.08280), Color(0.25678, 0.13762, 0.08601), 0.10, Color(0.1), Color(0.0),           1.0};
Material mtGold    = {Color(0.0), Color(0.24725, 0.19950, 0.07450), Color(0.75164, 0.60648, 0.22648), Color(0.62828, 0.55580, 0.36606), 0.40, Color(0.3), Color(0.0),           1.0};
Material mtSilver  = {Color(0.0), Color(0.19225, 0.19225, 0.19225), Color(0.50754, 0.50754, 0.50754), Color(0.50827, 0.50827, 0.50827), 0.40, Color(0.3), Color(0.0),           1.0};

Material mtGlass   = {Color(0.0), Color(0.2, 0.0, 0.1),             Color(0.3, 0.0, 0.2),             Color(0.5),                       0.60, Color(0.1), Color(0.8, 0.6, 0.7), 1.5};
Material mtEmerald = {Color(0.0), Color(0.02150, 0.17450, 0.02150), Color(0.07568, 0.61424, 0.07568), Color(0.63300, 0.72781, 0.63300), 0.60, Color(0.1), Color(0.2, 0.5, 0.4), 1.6}; // изумруд
Material mtJade    = {Color(0.0), Color(0.13500, 0.22250, 0.15750), Color(0.54000, 0.89000, 0.63000), Color(0.31623, 0.31623, 0.31623), 0.10, Color(0.0), Color(0.0),           1.6}; // нифрит
Material mtPearl   = {Color(0.0), Color(0.25000, 0.20725, 0.20725), Color(1.00000, 0.82900, 0.82900), Color(0.29665, 0.29665, 0.29665), 0.09, Color(0.1), Color(0.0),           1.6};
Material mtRuby    = {Color(0.0), Color(0.17450, 0.01175, 0.01175), Color(0.61424, 0.04136, 0.04136), Color(0.72781, 0.62696, 0.62696), 0.60, Color(0.0), Color(0.5, 0.2, 0.4), 1.7}; // рубин

Material mtBlack   = {Color(0.0), Color(0.0),                       Color(0.0),                       Color(0.4),                       0.25, Color(0.0), Color(0.0),           1.0};
Material mtWhite   = {Color(0.0), Color(0.4),                       Color(0.8),                       Color(1.0),                       0.25, Color(0.0), Color(0.0),           1.0};
Material mtRed     = {Color(0.0), Color(0.1, 0.0, 0.0),             Color(0.7, 0.0, 0.0),             Color(1.0, 0.4, 0.4),             0.25, Color(0.0), Color(0.0),           1.0};
Material mtGreen   = {Color(0.0), Color(0.0, 0.1, 0.0),             Color(0.0, 0.7, 0.0),             Color(0.4, 1.0, 0.4),             0.25, Color(0.0), Color(0.0),           1.0};
Material mtBlue    = {Color(0.0), Color(0.0, 0.0, 0.1),             Color(0.0, 0.0, 0.7),             Color(0.4, 0.4, 1.0),             0.25, Color(0.0), Color(0.0),           1.0};

Material mtMarble    = {Color(0.0), Color(0.4),                     Color(0.8),                       Color(0.5),                       1.00, Color(0.0), Color(0.0),           1.7};
Material mtGranite   = {Color(0.0), Color(0.2, 0.1, 0.1),           Color(0.85, 0.65, 0.55),          Color(0.5),                       1.00, Color(0.0), Color(0.0),           1.7};
Material mtMalachite = {Color(0.0), Color(0.0, 0.2, 0.15),          Color(0.05, 0.40, 0.30),          Color(0.1, 0.5, 0.4),             1.00, Color(0.0), Color(0.0),           1.8};

SolidTexture txLight(mtLight);
SolidTexture txBrass(mtBrass);
SolidTexture txBronze(mtBronze);
SolidTexture txChrome(mtChrome);
SolidTexture txCopper(mtCopper);
SolidTexture txGold(mtGold);
SolidTexture txSilver(mtSilver);

SolidTexture txGlass(mtGlass);
SolidTexture txEmerald(mtEmerald);
SolidTexture txJade(mtJade);
SolidTexture txPearl(mtPearl);
SolidTexture txRuby(mtRuby);

SolidTexture txBlack(mtBlack);
SolidTexture txWhite(mtWhite);
SolidTexture txRed(mtRed);
SolidTexture txGreen(mtGreen);
SolidTexture txBlue(mtBlue);

MarbleTexture txMarble(mtMarble, 7.0);
GraniteTexture txGranite(mtGranite, Vec3(5.0));
MalachiteTexture txMalachite(mtMalachite, Vec3(1.0), 11.0);
CheckerTexture txChess(&txBlack, &txWhite, Vec3(1.0));

const double Sfog = 15.0; // расстояние до тумана
const double Kfog = 2.0; // интенсивность тумана

double myFog(double t)
{
  if (t < Sfog)
    return 0.0;
  return 1.0 - pow(Sfog/t, Kfog); 
}

int main(int argc, char* argv[])
{
//  srand((int)time(NULL));

  txMarble.setNum(3);
  txMarble.setItem(0, 0.0, Color(0.0), Color(0.2), mtMarble.specular);
  txMarble.setItem(1, 0.2, Color(0.2), Color(0.5), mtMarble.specular);

  txGranite.setNum(6);
  txGranite.setItem(0, 0.0, Color(0.0), Color(0.83, 0.63, 0.57), mtGranite.specular);
  txGranite.setItem(1, 0.2, Color(0.0), Color(0.92, 0.83, 0.71), mtGranite.specular);
  txGranite.setItem(2, 0.3, Color(0.0), Color(0.87, 0.70, 0.65), mtGranite.specular);
  txGranite.setItem(3, 0.5, Color(0.0), Color(0.83, 0.63, 0.57), mtGranite.specular);
  txGranite.setItem(4, 0.7, Color(0.0), Color(0.94, 0.89, 0.82), mtGranite.specular);

  txMalachite.setNum(4);
  txMalachite.setItem(0, 0.0, Color(0.0, 0.06, 0.05), Color(0.00, 0.13, 0.10), mtMalachite.specular);
  txMalachite.setItem(1, 0.1, Color(0.0, 0.20, 0.15), Color(0.05, 0.40, 0.30), mtMalachite.specular);
  txMalachite.setItem(2, 0.8, Color(0.0, 0.30, 0.20), Color(0.09, 0.85, 0.60), mtMalachite.specular);
  txMalachite.setItem(3, 1.0, Color(0.0, 0.20, 0.15), Color(0.05, 0.40, 0.30), mtMalachite.specular);

  Scene scene;
  scene.setColor(Color(0.5, 0.8, 1.0));
  scene.setDepth(8);
//  scene.setFog(Color(0.75, 0.75, 0.75), myFog);
//  scene.setRefract(1.6);

  scene.add(new Light(Color(0.8)));

  PointLight* l1 = new PointLight(Color(0.8), Vec3(10.0, 10.0, 10.0));
  l1->setAttenuation(0.02, 0.001);
  scene.add(l1);

  PointLight* l2 = new PointLight(Color(0.4), Vec3(-10.0, 10.0, 10.0));
  l2->setAttenuation(0.02, 0.001);
  scene.add(l2);
/*
 SphericLight* l2 = new SphericLight(Color(0.5), Vec3(-10.0, 10.0, 10.0), 1.0, 256);
  l2->setAttenuation(0.01, 0.001);
  scene.add(l2);
*/
  Sphere* s1 = new Sphere(Vec3(0.0, 3.0, 0.0), 2.0);
  s1->setTexture(&txSilver);
  scene.add(s1);

  Sphere* s2 = new Sphere(Vec3(-3.0, 1.0, 0.0), 1.0);
  s2->setTexture(&txGold);
  scene.add(s2);

  Sphere* s3 = new Sphere(Vec3(3.0, 1.0, 2.0), 1.0);
  s3->setTexture(&txEmerald);
  scene.add(s3);

  Sphere* s4 = new Sphere(Vec3(0.0, 1.0, -3.0), 1.0);
  s4->setTexture(&txRuby);
  scene.add(s4);
/*
  GridRM* s8 = new GridRM(Vec3(1.0, 1.0, 4.0), Vec3(1.0), 0.6);
  s8->setTexture(&txMarble);
  scene.add(s8);
*/
/*
  SphereGridRM* s9 = new SphereGridRM(Vec3(1.0, 1.0, 4.3), Vec3(1.0), 0.6, 0.7); 
  s9->setTexture(&txMarble);
  scene.add(s9);
*/
/*
  Plane* p = new Plane(Vec3(0.0, 0.0, 1.0), 2.0);
  p->setTexture(&txMalachite);
  scene.add(p);
*/
  Plane* p = new Plane(Vec3(0.0, 1.0, 0.0), 0.0);
  p->setTexture(&txChess);
  scene.add(p);
/*
  Box* b1 = new Box(Vec3(-6.0, 0.0, -6.0), Vec3(12.0, -0.1, 12.0));
  b1->setTexture(&txChess);
  scene.add(b1);
*/
//  Box* b2 = new Box(Vec3(-3.0, 0.0, 3.0), Vec3(2.0, 1.0, 1.0));
  Box* b2 = new Box(Vec3(-3.0, 0.0, 3.0), Vec3(1.0, 1.0, 0.0), Vec3(0.0, 1.0, 1.0), Vec3(-1.0, 1.0, -1.0));
  b2->setTexture(&txGlass);
  scene.add(b2);
  
  Box* b3 = new Box(Vec3(2.0, 0.0, -3.0), Vec3(2.0, 2.0, 1.0));
  b3->setTexture(&txGranite);
  scene.add(b3);

//  OrthoCamera cam(Vec3(0.0, 2.0, 10.0), 5.0, (double)width/height);
//  PerspectiveCamera cam(Vec3(0.0, 2.0, 10.0), 45, (double)width/height);
//  CylindricCamera cam(Vec3(0.0, 0.0, 4.0), 120, (double)width/height);
//  SphericCamera cam(Vec3(0.0, 2.0, 4.0), 180, (double)width/height);
//  cam.rotate(Vec3(0.0, -2.0, -10.0), Vec3(0.0, 1.0, 0.0));

  PerspectiveCamera cam(Vec3(0.0, 2.0, 12.0), 45, (double)width/height);
  Image img(width, height);
  cam.render(scene, img);
//  cam.renderAA(scene, 64, img);
//  cam.renderAAA(scene, 4, 8, img);
  img.saveBMP("image.bmp");
//  img.savePPM("image.ppm", true);
/*
  int cntimg = 4;
  for (int i = 0; i <= cntimg; i++)
  {
    double angle = 2.0*DBL_PI/cntimg*i;
    double x = 12.0*sin(angle);
    double z = 12.0*cos(angle);
    cam.orient(Vec3(x, 3.0, z), Vec3(-x, -2.0, -z), Vec3(0.0, 1.0, 0.0));

    char path[16];
    sprintf(path, "%03d.bmp", i); // 3 символа и лидирующие нули 
    std::cout << path <<"\n";
    cam.render(scene, img);
//  cam.renderAA(scene, 64, img);
//  cam.renderAAA(scene, 4, 8, img);
    img.saveBMP(path);
  }
*/
  return 0;
}
