#ifndef __CAMERA_H__
#define __CAMERA_H__

//------------------------------------------------------------------------------
//
//
class Camera
{
public:
  Camera(const Vec3& org) : origin(org), direct(0.0, 0.0, -1.0), horz(1.0, 0.0, 0.0), vert(0.0, 1.0, 0.0) { px = py = 1.0; }

  void translate(const Vec3& org) { origin = org; }
  void rotate(const Vec3& dir, const Vec3& up)
  {
    (direct = dir).normalize();
    horz = Vec3::cross(dir, up).normalize();
    vert = Vec3::cross(horz, direct).normalize();
  }
  void orient(const Vec3& org, const Vec3& dir, const Vec3& up) { translate(org); rotate(dir, up); }

  void render(const Scene& scene, Image& img) const;
  void renderAA(const Scene& scene, int cnt, Image& img) const; // cnt - количество лучей на пиксель
  void renderAAA(const Scene& scene, int min, int max, Image& img) const; // max - максимальное количество лучей на пиксель

protected:
  Vec3 origin;   // положение в пространстве
  Vec3 direct;   // направление взгляда норм
  Vec3 horz;     // направление вправо норм
  Vec3 vert;     // направление вверх норм
  double px, py; // масштабирующие коэффициенты

  virtual Color trace(const Scene& scene, double x, double y) const; // нормализованные координаты (-1 <= x,y <= 1)
};

class OrthoCamera : public Camera
{
public:
  OrthoCamera(const Vec3& org, double scale, double aspect) : Camera(org) { init(scale, aspect); }

  void init(double scale, double aspect) { assert(scale > 0.0 && aspect > 0.0); py = scale; px = py*aspect; }
};

class PerspectiveCamera : public Camera
{
public:
  PerspectiveCamera(const Vec3& org, double fov, double aspect) : Camera(org) { init(fov, aspect); }//как соотносятся гориз и верт углы

  void init(double fov, double aspect) { assert(fov > 0.0 && fov < 180.0 && aspect > 0.0); py = tan(0.5*DBL_PI/180.0*fov); px = py*aspect; }

protected:
  virtual Color trace(const Scene& scene, double x, double y) const;
};

class CylindricCamera : public Camera
{
public:
  CylindricCamera(const Vec3& org, double fov, double aspect) : Camera(org) { init(fov, aspect); }

  void init(double fov, double aspect) { assert(fov > 0.0 && fov < 180.0 && aspect > 0.0); py = tan(0.5*DBL_PI/180.0*fov); px = py*aspect; }

protected:
  virtual Color trace(const Scene& scene, double x, double y) const;
};

class SphericCamera : public Camera
{
public:
  SphericCamera(const Vec3& org, double fov, double aspect) : Camera(org) { init(fov, aspect); }

  void init(double fov, double aspect) { assert(fov > 0.0 && aspect > 0.0); py = 0.5*DBL_PI/180.0*fov; px = py*aspect; }

protected:
  virtual Color trace(const Scene& scene, double x, double y) const;
};

#endif

