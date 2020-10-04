#include <cstdlib>
#include "color.h"
#include "vector.h"
#include "tracer.h"
#include "shape.h"

//------------------------------------------------------------------------------
//
//
double Sphere::intersect(const Ray& r) const
{
  Vec3 v = origin - r.origin;
  double b = Vec3::dot(v, r.direct);
  double c = Vec3::dot(v, v) - radius*radius;
  double d = b*b - c;
  if (d < 0.0)
    return 0.0;
  d = sqrt(d);
    
  double t = b - d;
  if (t <= 0.0)
    t = b + d;
  return t;
}

void Sphere::surface(const Vec3& p, Vec3& n, Material& m) const
{
  Vec3 v = p - origin;
  n = v*(1.0/radius);
  Shape::surface(v, n, m);
}

//------------------------------------------------------------------------------
//
//
double Plane::intersect(const Ray& r) const
{
  double d = Vec3::dot(r.direct, normal);
  if (fabs(d) < DBL_EPS)
    return 0.0;
  return (dist - Vec3::dot(r.origin, normal))/d;
}

void Plane::surface(const Vec3& p, Vec3& n, Material& m) const
{
  n = normal;
  Shape::surface(p, n, m);
}

//------------------------------------------------------------------------------
//
//
double Box::intersect(const Ray& r) const
{
  double tmin = -DBL_INF;
  double tmax = DBL_INF;
  for (int i = 0; i < 3; i++)
  {
    double p = Vec3::dot(r.direct, normal[i]);
    double d = Vec3::dot(r.origin, normal[i]);
    double t1, t2;
    if (p < -DBL_EPS)
    {
      if (d <= d1[i])
        return 0.0;
      t1 = (d2[i] - d)/p;
      t2 = (d1[i] - d)/p;
    }
    else if (p > DBL_EPS)
    {
      if (d >= d2[i])
        return 0.0;
      t1 = (d1[i] - d)/p;
      t2 = (d2[i] - d)/p;
    }
    else
    {
      if (d <= d1[i] || d >= d2[i])
        return 0.0;
      continue;
    }
    if (t1 > tmin)
      tmin = t1;
    if (t2 < tmax)
      tmax = t2;
    if (tmin > tmax)
      return 0.0;
  }
  return (tmin > 0.0) ? tmin : tmax;
}

void Box::surface(const Vec3& p, Vec3& n, Material& m) const
{
  double tmin = DBL_INF;
  for (int i = 0; i < 3; i++)
  {
    double d = Vec3::dot(p, normal[i]);
    double t = fabs(d1[i] - d);
    if (t < tmin)
    {
      tmin = t;
      n = -normal[i];
    }
    t = fabs(d2[i] - d);
    if (t < tmin)
    {
      tmin = t;
      n = normal[i];
    }
  }
  Shape::surface(p - origin, n, m);
}

void Box::init(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
  normal[0] = Vec3::cross(v1, v2).normalize();
  d1[0] = Vec3::dot(origin, normal[0]);
  d2[0] = d1[0] + Vec3::dot(v3, normal[0]);

  normal[1] = Vec3::cross(v2, v3).normalize();
  d1[1] = Vec3::dot(origin, normal[1]);
  d2[1] = d1[1] + Vec3::dot(v1, normal[1]);

  normal[2] = Vec3::cross(v3, v1).normalize();
  d1[2] = Vec3::dot(origin, normal[2]);
  d2[2] = d1[2] + Vec3::dot(v2, normal[2]);

  for (int i = 0; i < 3; i++)
    if (d1[i] > d2[i])
    {
      normal[i] = -normal[i];
      d1[i] = -d1[i];
      d2[i] = -d2[i];
    }
}

//-------------------------------------------------------------------------------
//
//
#define MARCH_MAX  256  //
#define MARCH_EPS  1e-4 //
#define MARCH_INF  1e3  //

inline double intersectCSG(double distA, double distB)
{
  return std::max(distA, distB);
}

inline double unionCSG(double distA, double distB)
{
  return std::min(distA, distB);
}

inline double differenceCSG(double distA, double distB)
{
  return std::max(distA, -distB);
}

double ShapeRM::intersect(const Ray& r) const
{
  double t = 0.0;
  for (int i = 0; i < MARCH_MAX; i++)
  {
    double d = distance(r.origin + r.direct*t);
    if (t > 0.0 && fabs(d) < DBL_EPS)
      return t;
    t += d;
    if (t > MARCH_INF)
      break;
  }
  return 0.0;
}

void ShapeRM::surface(const Vec3& p, Vec3& n, Material& m) const
{
  n = Vec3(distance(Vec3(p.x + MARCH_EPS, p.y, p.z)) - distance(Vec3(p.x - MARCH_EPS, p.y, p.z)),
           distance(Vec3(p.x, p.y + MARCH_EPS, p.z)) - distance(Vec3(p.x, p.y - MARCH_EPS, p.z)),
           distance(Vec3(p.x, p.y, p.z + MARCH_EPS)) - distance(Vec3(p.x, p.y, p.z - MARCH_EPS))).normalize();
  Shape::surface(p, n, m);
}

double SphereRM::distance(const Vec3& p) const
{
  return Vec3::distance(p, origin) - radius;
}

double BoxRM::distance(const Vec3& p) const 
{
  Vec3 v = Vec3::abs(p - origin) - size;
  return v.max();
}

double GridRM::distance(const Vec3& p) const 
{
  double sphereDist = SphereRM::distance(p);
  double boxDist = box.distance(p);
  //return intersectCSG(boxDist, sphereDist);
  //return unionCSG(boxDist, sphereDist);
  return differenceCSG(boxDist, sphereDist);
}

double SphereGridRM::distance(const Vec3& p) const 
{
  double sphereDist = SphereRM::distance(p);
  double boxDist = box.distance(p);
  double t = differenceCSG(boxDist, sphereDist);
  double outsphereDist = sphere.distance(p);
  //return intersectCSG(boxDist, sphereDist);
  //return unionCSG(boxDist, sphereDist);
  return intersectCSG(t, outsphereDist);
}

//------------------------------------------------------------------------------
//
//
void SolidTexture::material(const Vec3& p, Material& m) const
{
  m = mt;
}

void CheckerTexture::material(const Vec3& p, Material& m) const
{
  Vec3 v = p*size + DBL_EPS;
  if (v.x < 0.0)
    v.x = 1.0 - v.x;
  if (v.y < 0.0)
    v.y = 1.0 - v.y;
  if (v.z < 0.0)
    v.z = 1.0 - v.z;
  texture[((int)v.x + (int)v.y + (int)v.z) & 0x01]->material(p, m);
}

//-----------------------------------------------------------------------------
//
//
void MapTexture::setNum(int n)
{
  assert(n > 0);
  if (item != NULL)
  {
    delete[] item;
    item = NULL;
  }
  item = new Item[num = n];
  if (num == 1)
  {
    item[0].t = 1.0;
    item[0].ambient = mt.ambient;
    item[0].diffuse = mt.diffuse;
    item[0].specular = mt.specular;
  }
  else
  {
    for (int i = 0; i < num; i++)
    {
      double t = (double)i/(num - 1);
      item[i].t = t;
      item[i].ambient = mt.ambient*t;
      item[i].diffuse = mt.diffuse*t;
      item[i].specular = mt.specular*t;
    }
  }
}

void MapTexture::material(const Vec3& p, Material& m) const
{
  m = mt;

  double t = value(p);
  for (int i = 0; i < num; i++)
    if (t <= item[i].t)
    {
      if (i == 0)
      {
        m.ambient = item[0].ambient;
        m.diffuse = item[0].diffuse;
        m.specular = item[0].specular;
      }
      else
      {
        t = (t - item[i - 1].t)/(item[i].t - item[i - 1].t);
        t = t*t*(3.0 - 2.0*t); // кубическа€ интерпол€ци€
        m.ambient = Color::mix(item[i - 1].ambient, item[i].ambient, t);
        m.diffuse = Color::mix(item[i - 1].diffuse, item[i].diffuse, t);
        m.specular = Color::mix(item[i - 1].specular, item[i].specular, t);
      }
      break;
    }
}

double MapTexture::value(const Vec3& p) const
{
  return (sin(p.x) + 1.0)*0.5;
}

double GraniteTexture::value(const Vec3& p) const
{
  return Vec3::turbulence(p*size, 0.5, 5);
}

double MarbleTexture::value(const Vec3& p) const
{
  Vec3 v(p.x, (p.y + Vec3::turbulence(p, 0.7, 5))*2.0, (p.z + Vec3::turbulence(p, 0.8, 5))*2.0);
  return (sin((v.x + Vec3::turbulence(v, 0.5, 5))*freq) + 1.0)*0.5;
}

double MalachiteTexture::value(const Vec3& p) const
{
//  return (sin(p.x) + 1.0)*0.5;
//  return Vec3::noise(p*freq);
//  return (sin((p.x + 0.5*sin(p.y*p.z) + Vec3::turbulence(p, 0.5, 5))*freq) + 1.0)*0.5;
//  return (sin((sin(p.x)*sin(p.y)*sin(p.z) + Vec3::turbulence(p, 0.5, 5))*freq) + 1.0)*0.5;
  Vec3 v = p*size;
  return (sin((sin(v.x) + sin(v.y) + sin(v.z) + Vec3::turbulence(v, 0.5, 5))*freq) + 1.0)*0.5;
}
