#include <cstdlib>
#include "vector.h"

//------------------------------------------------------------------------------
//
//
double Vec3::min() const
{
  double t = x;
  if (y < t)
    t = y;
  if (z < t)
    t = z;
  return t;
}

double Vec3::max() const
{
  double t = x;
  if (y > t)
    t = y;
  if (z > t)
    t = z;
  return t;
}

Vec3& Vec3::normalize() 
{
  double t = length();
  if (t > 0.0)
    *this *= 1.0/t;
  return *this;
}

Vec3 Vec3::rand()
{
  Vec3 v(::rand(), ::rand(), ::rand());
  v -= 0.5*RAND_MAX;
  return v.normalize();
}

//------------------------------------------------------------------------------
//
//
const int NOISE_SIZE = 15;
static double tableNoise[NOISE_SIZE][NOISE_SIZE][NOISE_SIZE];

static void initNoise()
{
  static bool init = false;
  if (init)
    return;

  double t = 1.0/RAND_MAX;
  for (int i = 0; i < NOISE_SIZE; i++)
    for (int j = 0; j < NOISE_SIZE; j++)
      for (int k = 0; k < NOISE_SIZE; k++)
        tableNoise[i][j][k] = (double)::rand()*t;
  init = true;
}

double Vec3::noise(const Vec3& v)
{
  double x, y, z;
  int ix, iy, iz, jx, jy, jz;
  for (int k = 0; k < 3; k++)
  {
    double t = fmod(v[k], NOISE_SIZE);
    if (t < 0.0)
      t += NOISE_SIZE;

    int i = (int)t;
    int j = i + 1;
    if (j >= NOISE_SIZE)
      j = 0;
    t -= i;
//    t = t*t*(3.0 - 2.0*t); // кубическая интерполяция
    t = t*t*t*(t*(6.0*t - 15.0) + 10.0); // интерполяция полиномом 5ой степени
    switch (k)
    {
    case 0:
      x = t;
      ix = i;
      jx = j;
      break;
    case 1:
      y = t;
      iy = i;
      jy = j;
      break;
    case 2:
      z = t;
      iz = i;
      jz = j;
      break;
    default:
      break;
    }
  }
  initNoise();
  return (1.0 - x)*(1.0 - y)*(1.0 - z)*tableNoise[ix][iy][iz] +
         (1.0 - x)*(1.0 - y)*     z   *tableNoise[ix][iy][jz] +
         (1.0 - x)*     y   *(1.0 - z)*tableNoise[ix][jy][iz] +
         (1.0 - x)*     y   *     z   *tableNoise[ix][jy][jz] +
              x   *(1.0 - y)*(1.0 - z)*tableNoise[jx][iy][iz] +
              x   *(1.0 - y)*     z   *tableNoise[jx][iy][jz] +
              x   *     y   *(1.0 - z)*tableNoise[jx][jy][iz] +
              x   *     y   *     z   *tableNoise[jx][jy][jz];
}

double Vec3::turbulence(const Vec3& v, double persistence, int cnt)
{
  double sum = 0.0;
  double max = 0.0;
  double t = 1.0;
  Vec3 p = v;
  while (cnt-- > 0)
  {
    sum += noise(p)*t;
    max += t;
    t *= persistence;
    p *= 2.0;
  }
  return sum/max;
}
