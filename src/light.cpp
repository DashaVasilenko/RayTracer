#include <cstdlib>
#include "color.h"
#include "vector.h"
#include "tracer.h"
#include "light.h"

//------------------------------------------------------------------------------
//
//
Color DirectLight::shadow(const Scene& scene, Ray& r) const
{
  Ray ray(r.origin, -direct);
  Vec3 normal(0.0);
  Material m;
  Color c = color;
  while (c.sum() > DBL_EPS)
  {
    ray.origin += ray.direct*DBL_EPS;

    double t;
    Shape* s = scene.intersect(ray, t);
    if (s == NULL)
      break;
    ray.origin += ray.direct*t;
    s->surface(ray.origin, normal, m);
    c *= m.transparent; // ???
  }
  r.direct = ray.direct;
  return c;
}

Color PointLight::shadow(const Scene& scene, Ray& r) const
{
  Ray ray(r.origin, origin - r.origin);
  double d = ray.direct.length(); // расстояние от источника до поверхности
  if (d > 0.0)
    ray.direct *= 1.0/d;

  Vec3 normal(0.0);
  Material m;
  Color c = color;
  c *= 1.0/(1.0 + d*(att1 + d*att2)); // ослабление интенсивности света
  while (c.sum() > DBL_EPS)
  {
    ray.origin += ray.direct*DBL_EPS;
     double t;
    Shape* s = scene.intersect(ray, t);
    if (s == NULL || t >= d)
      break;
    ray.origin += ray.direct*t;
    s->surface(ray.origin, normal, m);
//      c = c*f.Kt + f.color*(1.0 - f.Kt); так не использовать
//      c = (c + f.color*(2.0 - 2.0*f.Kt))*f.Kt;
//      c = (c + f.color*(1.0 - f.Kt - f.Kr))*f.Kt;
//      c = (c + f.color*(1.0 - f.Kt))*f.Kt; // ??? учесть Kr, 2.0
    c *= m.transparent; // ???
    d -= t;
  }
  r.direct = ray.direct;
  return c;
}

Color SphericLight::shadow(const Scene& scene, Ray& r) const
{
  Color sum(0.0), sum2(0.0);
  int n = 0; // количество пробных лучей
  for (int i = 0; i < cntmax; i++)
  {
    Ray ray(r.origin, origin - r.origin + Vec3::rand()*radius);
    double d = ray.direct.length(); // расстояние от источника до поверхности
    if (d > 0.0)
      ray.direct *= 1.0/d;

    Vec3 normal(0.0);
    Material m;
    Color c = color;
    c *= 1.0/(1.0 + d*(att1 + d*att2)); // ослабление интенсивности света
    while (c.sum() > DBL_EPS)
    {
      ray.origin += ray.direct*DBL_EPS;

      double t;
      Shape* s = scene.intersect(ray, t);
      if (s == NULL || t >= d)
        break;
      ray.origin += ray.direct*t;
      s->surface(ray.origin, normal, m);
      c *= m.transparent; // ???
      d -= t;
    }
    sum += c;
    sum2 += c*c;
    if (++n < cntmin)
      continue;
    c = sum2 - sum*sum/n;
    if (c.sum() <= DBL_EPS*(n - 1)) // ???
      break;
  }
  sum *= 1.0/n;
  r.direct = (origin - r.origin).normalize();
  return sum;
}
