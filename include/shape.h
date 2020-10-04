#ifndef _SHAPE_H_
#define _SHAPE_H_

//------------------------------------------------------------------------------
//
//
class Sphere : public Shape
{
public:   
  Sphere(const Vec3& org, double r) : origin(org) { assert(r > 0.0); radius = r; }

  virtual double intersect(const Ray& r) const;
  virtual void surface(const Vec3& p, Vec3& n, Material& m) const;

protected: 
  Vec3 origin;
  double radius;
};

//------------------------------------------------------------------------------
//
//
class Plane : public Shape
{
public:
  Plane(const Vec3& n, double d) : normal(n) { normal.normalize(); dist = d; }
  
  virtual double intersect(const Ray& r) const;
  virtual void surface(const Vec3& p, Vec3& n, Material& m) const;

protected:
  Vec3 normal;
  double dist;
};

//------------------------------------------------------------------------------
//
//
class Box : public Shape
{
public:   
  Box(const Vec3& org, const Vec3& v1, const Vec3& v2, const Vec3& v3) : origin(org) { init(v1, v2, v3); }
  Box(const Vec3& org, const Vec3& s) : origin(org) { init(Vec3(s.x, 0.0, 0.0), Vec3(0.0, s.y, 0.0), Vec3(0.0, 0.0, s.z)); }

  virtual double intersect(const Ray& r) const;
  virtual void surface(const Vec3& p, Vec3& n, Material& m) const;

protected: 
  Vec3 origin;
  Vec3 normal[3];
  double d1[3];
  double d2[3];

  void init(const Vec3& v1, const Vec3& v2, const Vec3& v3);
};

//-------------------------------------------------------------------------------
//
//
class ShapeRM : public Shape
{
public:   
  ShapeRM() {}

  virtual double intersect(const Ray& r) const;
  virtual void surface(const Vec3& p, Vec3& n, Material& m) const;
  virtual double distance(const Vec3& p) const = 0;
};

//------------------------------------------------------------------------------
//
//
class SphereRM : public ShapeRM
{
public:   
  SphereRM(const Vec3& org, double r) : origin(org) { assert(r > 0.0); radius = r; }

  virtual double distance(const Vec3& p) const;

protected: 
  Vec3 origin;
  double radius;
};

//------------------------------------------------------------------------------
//
//
class BoxRM : public ShapeRM
{
public:   
  BoxRM(const Vec3& org, const Vec3& s) : origin(org), size(s) { size *= 0.5; }

  virtual double distance(const Vec3& p) const;

protected: 
  Vec3 origin;
  Vec3 size;
};

//--------------------------------------------------------------------------------------------------
//
//
class GridRM : public SphereRM
{
public:   
  GridRM (const Vec3& org, const Vec3& s, double r) : SphereRM(org, r), box(org, s) { }
  
  virtual double distance(const Vec3& p) const ;

protected: 
  BoxRM box;
};

//--------------------------------------------------------------------------------------------------
//
//
class SphereGridRM : public SphereRM
{
public:   
  SphereGridRM (const Vec3& org, const Vec3& s, double r1, double r2) : SphereRM(org, r1), box(org, s), sphere(org, r2) { }
  
  virtual double distance(const Vec3& p) const;

protected: 
  BoxRM box;
  SphereRM sphere;
};

//------------------------------------------------------------------------------
//
//
class SolidTexture : public Texture //одинаковая равномерная текстура(типо золото, серебро, не мрамор)
{
public:
  SolidTexture(const Material& m) : mt(m) { }

  virtual void material(const Vec3& p, Material& m) const;

protected:
  Material mt;
};

//-----------------------------------------------------------------------------------
//
//
class CheckerTexture : public Texture
{
public:
  CheckerTexture(Texture* t1, Texture* t2, const Vec3& s) : size(1.0) { texture[0] = t1; texture[1] = t2; size /= s; }

  virtual void material(const Vec3& p, Material& m) const;

protected:
  const Texture* texture[2];
  Vec3 size; //размер клетки 
};

//-----------------------------------------------------------------------------
//
//
class MapTexture : public SolidTexture
{
public:
  MapTexture(const Material& m) : SolidTexture(m) { num = 0; item = NULL; }
  ~MapTexture() { if (item != NULL) delete[] item; }

  void setNum(int n);
  void setItem(int i, double t, const Color& amb, const Color& diff, const Color& spec)
  {
    assert(i >= 0 && i < num);
    item[i].t = t;
    item[i].ambient = amb;
    item[i].diffuse = diff;
    item[i].specular = spec;
  }

  virtual void material(const Vec3& p, Material& m) const;

protected:
  struct Item
  {
    double t;
    Color ambient;
    Color diffuse;
    Color specular;
  };
  int num;
  Item* item;

  virtual double value(const Vec3& p) const;
};

//----------------------------------------------------------------------------------
//
//
class GraniteTexture : public MapTexture
{
public:
  GraniteTexture(const Material& m, const Vec3& s) : MapTexture(m), size(s) { }

protected:
  Vec3 size;

  virtual double value(const Vec3& p) const;
};

//----------------------------------------------------------------------------------
//
//
class MarbleTexture : public MapTexture
{
public:
  MarbleTexture(const Material& m, double f) : MapTexture(m) { freq = f; }

protected:
  double freq;

  virtual double value(const Vec3& p) const;
};

//----------------------------------------------------------------------------------
//
//
class MalachiteTexture : public MapTexture
{
public:
  MalachiteTexture(const Material& m, const Vec3& s, double f) : MapTexture(m), size(DBL_PI) { size /= s; freq = f; }

protected:
  Vec3 size;
  double freq;

  virtual double value(const Vec3& p) const;
};

#endif
