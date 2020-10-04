#include <cstdlib>
#include <iostream>
#include "color.h"
#include "vector.h"
#include "tracer.h"
#include "camera.h"

//------------------------------------------------------------------------------
//
//
void Camera::render(const Scene& scene, Image& img) const
{
  double hx = 2.0/img.width();
  double hy = 2.0/img.height();
  int dot = 0;
  for (int i = 0; i < img.height(); i++)
  {
    double y = 1.0 - (i + 0.5)*hy;
    for (int j = 0; j < img.width(); j++)
    {
      double x = -1.0 + (j + 0.5)*hx;
      img.setPixel(j, i, trace(scene, x, y));
    }
    if (i*80/img.height() > dot)
    {
      dot++;
      std::cout << '.';
      std::cout.flush();
    }
  }
  std::cout << std::endl;
}

void Camera::renderAA(const Scene& scene, int cnt, Image& img) const
{
  double hx = 2.0/img.width();
  double hy = 2.0/img.height();
  double h = 1.0/RAND_MAX;
  double d = 1.0/cnt;
  int dot = 0;
  for (int i = 0; i < img.height(); i++)
  {
    for (int j = 0; j < img.width(); j++)
    {
      double y = 1.0 - (i + 0.5)*hy;
      double x = -1.0 + (j + 0.5)*hx;
      Color c = trace(scene, x, y);
      for (int n = 1; n < cnt; n++)
      {
        y = 1.0 - (i + rand()*h)*hy;
        x = -1.0 + (j + rand()*h)*hx;
        c += trace(scene, x, y);
      }
      c *= d;
      img.setPixel(j, i, c);
    }
    if (i*80/img.height() > dot)
    {
      dot++;
      std::cout << '.';
      std::cout.flush();
    }
  }
  std::cout << std::endl;
}

void Camera::renderAAA(const Scene& scene, int min, int max, Image& img) const
{
  double hx = 2.0/img.width();
  double hy = 2.0/img.height();
  double h = 1.0/RAND_MAX;
  int dot = 0;
  for (int i = 0; i < img.height(); i++)
  {
    for (int j = 0; j < img.width(); j++)
    {
      double y = 1.0 - (i + 0.5)*hy;
      double x = -1.0 + (j + 0.5)*hx;
      Color c, sum = trace(scene, x, y);
      Color sum2 = sum*sum;
      int n = 1; // количество выпущенных лучей на пиксель
      while (n < max)
      {
        y = 1.0 - (i + rand()*h)*hy;
        x = -1.0 + (j + rand()*h)*hx;
        c = trace(scene, x, y);
        sum += c;
        sum2 += c*c;
        if (++n < min)
          continue;
        c = sum2 - sum*sum/n;
        if (c.sum() <= DBL_EPS*(n - 1))
          break;
      }
      sum *= 1.0/n;
      img.setPixel(j, i, sum);
    }
    if (i*80/img.height() > dot)
    {
      dot++;
      std::cout << '.';
      std::cout.flush();
    }
  }
  std::cout << std::endl;
}

Color Camera::trace(const Scene& scene, double x, double y) const
{
  Ray r(origin + horz*(x*px) + vert*(y*py), direct);
  return scene.trace(r);
}

Color PerspectiveCamera::trace(const Scene& scene, double x, double y) const
{
  Ray r(origin, (direct + horz*(x*px) + vert*(y*py)).normalize());
  return scene.trace(r);
}

Color CylindricCamera::trace(const Scene& scene, double x, double y) const
{
  x *= px;

  Ray r(origin, (direct*cos(x) + horz*sin(x) + vert*(y*py)).normalize());
  return scene.trace(r);
}

Color SphericCamera::trace(const Scene& scene, double x, double y) const
{
  x *= px;
  y *= py;

  Ray r(origin, (direct*cos(sqrt(x*x + y*y)) + horz*sin(x) + vert*sin(y)).normalize());
  return scene.trace(r);
}
