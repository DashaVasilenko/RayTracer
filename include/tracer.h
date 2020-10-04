#ifndef __TRACER_H__
#define __TRACER_H__

#include <vector>

#define DBL_EPS      1e-6   //
#define DBL_INF      1e10   //
#define DBL_PI       3.14159265358979323846 // pi

//------------------------------------------------------------------------------
//
//
class Ray 
{ 
public:
  Vec3 origin; // begin of the vector
  Vec3 direct; // must be normalized 

  Ray() : origin(0.0), direct(0.0) { }
  Ray(const Vec3& org, const Vec3& dir) : origin(org), direct(dir) { }
  Ray(const Ray& ray) : origin(ray.origin), direct(ray.direct) { }

//  Vec3 point(double t) const { return origin + direct*t; }
};

//------------------------------------------------------------------------------
//
//
class Scene;

struct Material
{
  Color emissive;    // собственное излучение
  Color ambient;     // фоновое освещение
  Color diffuse;     // рассеянное освещение
  Color specular;    // зеркальное освещение
  double shininess;  // степень блика
  Color reflect;     // коэффициент отражения
  Color transparent; // коэффициент прозрачности(пропускания)
  double refract;    // показатель преломления ???
};

//------------------------------------------------------------------------------
//
//
class Texture
{
public:
  virtual void material(const Vec3& p, Material& m) const = 0;
};

//------------------------------------------------------------------------------
//
//
class Shape
{
public:
  Shape() { texture = NULL; }
  virtual ~Shape() { }

  void setTexture(const Texture* t) { texture = t; }
  virtual double intersect(const Ray& r) const = 0;
  virtual void surface(const Vec3& p, Vec3& n, Material& m) const
  {
    if (texture != NULL)
      texture->material(p, m);
  }

protected:
  const Texture* texture;
};

//------------------------------------------------------------------------------
//
//
class Light
{
public:
  Light(const Color& c) : color(c) { }
  virtual ~Light() { }

  void setColor(const Color& c) { color = c; }
  virtual Color shadow(const Scene& scene, Ray& r) const
  {
    r.direct = 0.0; // фоновый источник
    return color;
  }

protected:
  Color color;
//double intensity;
};

//------------------------------------------------------------------------------
//
//
class Scene
{
public:
  Scene();
  ~Scene();

  void setDepth(int d) { assert(d >= 0); depth = d; }
  void setColor(const Color& c) { color = c; }
  void setRefract(double r) { assert(r > 0); refract = r; }
//  void setFog(double s1, double e1, Color f) { assert(s1 >= 0.0 && e1 >= 0.0); s = s1; e = e1; fogcolor = f; }
  void setFog(const Color& c, double (*f)(double)) { fogcolor = c; fog = f; }
  void add(Shape* s) { shape.push_back(s); }
  void add(Light* l) { light.push_back(l); }
  
  Shape* intersect(const Ray& r, double& t) const; // поиск точки пересечения с ближайщим объектом

  Color background(const Ray& r) const;

  Color trace(const Ray& r) const { return trace(r, depth, 1.0); }

protected:
  int    depth; // допустимая глубина рекурсии
  Color  color; // цвет фона
  double refract; // показатель преломления среды
  double (*fog)(double);
  Color fogcolor; // цвет тумана
  std::vector<Shape*> shape;
  std::vector<Light*> light;

  Color trace(const Ray& r, int level, double weight) const; // (луч, глубина рекурсии, вес луча света)
};

#endif
