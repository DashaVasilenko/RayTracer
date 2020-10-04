#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cassert>
#include <cmath>

//------------------------------------------------------------------------------
//
//
class Vec3
{
public:
  double x, y, z;

  Vec3() { }
  Vec3(double d) { init(d); }
  Vec3(double dx, double dy, double dz) { init(dx, dy, dz); }
  Vec3(const Vec3& v) { x = v.x; y = v.y; z = v.z; }

  void init(double d) { x = y = z = d; }
  void init(double dx, double dy, double dz) { x = dx; y = dy; z = dz; }

  bool operator==(const Vec3& v) const { return x == v.x && y == v.y && z == v.z; }
  bool operator!=(const Vec3& v) const { return x != v.x || y != v.y || z != v.z; }

  double operator[](int i) const { assert(i >= 0 && i < 3); return (&x)[i]; }
  double& operator[](int i) { assert(i >= 0 && i < 3); return (&x)[i]; }

  Vec3& operator=(double d) { x = y = z = d; return *this; }
  Vec3& operator+=(double d) { x += d; y += d; z += d; return *this; }
  Vec3& operator-=(double d) { x -= d; y -= d; z -= d; return *this; }
  Vec3& operator*=(double d) { x *= d; y *= d; z *= d; return *this; }
  Vec3& operator/=(double d) { x /= d; y /= d; z /= d; return *this; }
  Vec3& operator=(const Vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }
  Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
  Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
  Vec3& operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
  Vec3& operator/=(const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

  Vec3 operator+() const { return Vec3(x, y, z); }
  Vec3 operator-() const { return Vec3(-x, -y, -z); }
  Vec3 operator+(double d) const { return Vec3(x + d, y + d, z + d); }
  Vec3 operator-(double d) const { return Vec3(x + d, y + d, z + d); }
  Vec3 operator*(double d) const { return Vec3(x * d, y * d, z * d); }
  Vec3 operator/(double d) const { return Vec3(x / d, y / d, z / d); }
  Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
  Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
  Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
  Vec3 operator/(const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z); }

  double min() const;
  double max() const;
  double sum() const { return x + y + z; }
  double length() const { return sqrt(x*x + y*y + z*z); }
  Vec3& normalize();
  
  static double distance(const Vec3& v1, const Vec3& v2) { return (v1 - v2).length(); }
  static double dot(const Vec3& v1, const Vec3& v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
  static Vec3 cross(const Vec3& v1, const Vec3& v2) { return Vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x); }
  static Vec3 abs(const Vec3& v) { return Vec3(fabs(v.x), fabs(v.y), fabs(v.z)); }
  static Vec3 pow(const Vec3& v, double t) { return Vec3(::pow(v.x, t), ::pow(v.y, t), ::pow(v.z, t)); }
  static Vec3 mix(const Vec3& v1, const Vec3& v2, double t) { return Vec3(v1.x + (v2.x - v1.x)*t, v1.y + (v2.y - v1.y)*t, v1.z + (v2.z - v1.z)*t); }
  static Vec3 rand();
  static double noise(const Vec3& v);
  static double turbulence(const Vec3& v, double persistence, int cnt);//точка, в которой вычисляем функцию, стойкость, количество октав  
};

#endif
