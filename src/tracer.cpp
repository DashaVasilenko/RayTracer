#include <cstdlib>
#include "color.h"
#include "vector.h"
#include "tracer.h"

//------------------------------------------------------------------------------
//
//
Scene::Scene()
 : color(1.0), fogcolor(0.0)
{
  refract = 1.0;
  depth = 3;
  fog = NULL;
}

Scene::~Scene()
{
  for (int i = 0; i < (int)shape.size(); i++)
    delete shape[i];
  for (int i = 0; i < (int)light.size(); i++)
    delete light[i];
}

Shape* Scene::intersect(const Ray& r, double& t) const
{
  Shape* s = NULL;
  t = DBL_INF;
  for (int i = 0; i < (int)shape.size(); i++)
  {
    double d = shape[i]->intersect(r);
    if (d > DBL_EPS && d < t)
    {
      s = shape[i];
      t = d;
    }
  }
  return s;
}

Color Scene::background(const Ray& r) const
{
  double t = (cos(7.0*DBL_PI*r.direct.x)*cos(3.0*DBL_PI*r.direct.y) + 1.0)*0.5;
  return Color::mix(Color(1.0), color, t);
  //return color;
}

Color Scene::trace(const Ray& r, int level, double weight) const
{
  double t;
  Shape* s = intersect(r, t);
  if (s == NULL)
    return background(r);

  Ray ray(r.origin + r.direct*t, Vec3(0.0));
  Vec3 normal(0.0);
  Material m;
  s->surface(ray.origin, normal, m);

  double cosi = Vec3::dot(r.direct, normal); // угол падения
  bool outside = cosi < 0.0;
  if (outside)
    cosi = -cosi;
  else
    normal = -normal;

  Color cs, c = m.emissive;
  for (int i = 0; i < (int)light.size(); i++)
  {
    ray.direct = normal;
    cs = light[i]->shadow(*this, ray); // свет, который попадает на поверхность (с учетом интенсивности)
    if (cs.sum() < DBL_EPS) // ???
      continue;
    if (ray.direct.x == 0.0 && ray.direct.y == 0.0 && ray.direct.z == 0.0) // фоновое освещение ???
    {
      c += cs*m.ambient;
      continue;
    }

    double cosl = Vec3::dot(ray.direct, normal);
    if (cosl <= 0.0)
      continue;
    if (m.diffuse.sum() > 0.0) // диффузное освещение
      c += cs*m.diffuse*cosl;
    if (m.specular.sum() > 0.0) // зеркальное освещение (блики)
    {
      cosl = Vec3::dot(ray.direct - normal*(2.0*cosl), r.direct);
      if (cosl > 0.0)
        c += cs*m.specular*pow(cosl, m.shininess);
    }
  }
  if (level > 0)
  {
    // преломление
    double w = m.transparent.sum()*weight;
    if (w > DBL_EPS)
    {
      double n = outside ? refract/m.refract : m.refract/refract;
      double d = 1.0 - n*n*(1.0 - cosi*cosi);
      if (d < 0.0) // полное внутреннее отражения
        m.reflect += m.transparent;
      else
      {
        Ray refract_ray(ray.origin - normal*DBL_EPS, r.direct*n + normal*(n*cosi - sqrt(d)));
        c += trace(refract_ray, level - 1, w)*m.transparent;
      }
    }
    // отражение
    w = m.reflect.sum()*weight;
    if (w > DBL_EPS)
    {
      Ray reflect_ray(ray.origin + normal*DBL_EPS, r.direct + normal*(2.0*cosi));
      c += trace(reflect_ray, level - 1, w)*m.reflect;
    }
  }
  c.normalize(); // нормализация итогового цвета
  if (fog != NULL && outside) // туман ??? учесть outside
    c = Color::mix(c, fogcolor, fog(t));
  return c;
}
