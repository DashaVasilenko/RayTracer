#ifndef __LIGHT_H__
#define __LIGHT_H__

//---------------------------------------------------------------------------------------
//
//
class DirectLight : public Light
{
public:
  DirectLight(const Color& c, const Vec3& dir) : Light(c), direct(dir) { direct.normalize(); }
// надо переписать
  virtual Color shadow(const Scene& scene, Ray& r) const;

protected:
  Vec3 direct;
};

class PointLight : public Light
{
public:
  PointLight(const Color& c, const Vec3& org) : Light(c), origin(org) { att1 = att2 = 0.0; }

  void setAttenuation(double k1, double k2) { assert(k1 >= 0 && k2 >= 0); att1 = k1; att2 = k2; }
  virtual Color shadow(const Scene& scene, Ray& r) const;

protected:
  Vec3 origin;
  double att1, att2;
};

class SphericLight : public Light
{
public:
  SphericLight(const Color& c, const Vec3& org, double r, int cnt = 16) : Light(c), origin(org) { assert(cnt > 0); radius = r; cntmin = 16; cntmax = cnt; att1 = att2 = 0.0; }

  void setAttenuation(double k1, double k2) { assert(k1 >= 0 && k2 >= 0); att1 = k1; att2 = k2; }
  virtual Color shadow(const Scene& scene, Ray& r) const;

protected:
  Vec3 origin;
  double radius;
  int cntmin;
  int cntmax;
  double att1, att2;
};

#endif
