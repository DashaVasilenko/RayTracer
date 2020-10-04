#ifndef __COLOR_H__
#define __COLOR_H__

#include <cassert>
#include <cmath>

#ifndef _STDINT
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
#endif

//------------------------------------------------------------------------------
//
//
#pragma pack(push, 1)
struct RGBA
{
  uint8_t r; // red
  uint8_t g; // green
  uint8_t b; // blue
  uint8_t a; // alpha
};
#pragma pack(pop)

//------------------------------------------------------------------------------
//
//
class Color
{
public:
  double r, g, b;

  Color() { }
  Color(double d) { init(d); }
  Color(double dr, double dg, double db) { init(dr, dg, db); }
  Color(const RGBA& p) { init(p); }
  Color(const Color& c) { r = c.r; g = c.g; b = c.b; }

  void init(double d) { r = g = b = d; }
  void init(double dr, double dg, double db) { r = dr; g = dg; b = db; }
  void init(const RGBA& p) { double t = 1.0/255; r = p.r*t; g = p.g*t; b = p.b*t; }

  bool operator==(const Color& c) const { return r == c.r && g == c.g && b == c.b; }
  bool operator!=(const Color& c) const { return r != c.r || g != c.g || b != c.b; }

  double operator[](int i) const { assert(i >= 0 && i < 3); return (&r)[i]; }
  double& operator[](int i) { assert(i >= 0 && i < 3); return (&r)[i]; }

  Color& operator=(double d) { r = g = b = d; return *this; }
  Color& operator+=(double d) { r += d; g += d; b += d; return *this; }
  Color& operator-=(double d) { r -= d; g -= d; b -= d; return *this; }
  Color& operator*=(double d) { r *= d; g *= d; b *= d; return *this; }
  Color& operator/=(double d) { r /= d; g /= d; b /= d; return *this; }
  Color& operator=(const Color& c) { r = c.r; g = c.g; b = c.b; return *this; }
  Color& operator+=(const Color& c) { r += c.r; g += c.g; b += c.b; return *this; }
  Color& operator-=(const Color& c) { r -= c.r; g -= c.g; b -= c.b; return *this; }
  Color& operator*=(const Color& c) { r *= c.r; g *= c.g; b *= c.b; return *this; }
  Color& operator/=(const Color& c) { r /= c.r; g /= c.g; b /= c.b; return *this; }

  Color operator+(double d) const { return Color(r + d, g + d, b + d); }
  Color operator-(double d) const { return Color(r - d, g - d, b - d); }
  Color operator*(double d) const { return Color(r * d, g * d, b * d); }
  Color operator/(double d) const { return Color(r / d, g / d, b / d); }
  Color operator+(const Color& c) const { return Color(r + c.r, g + c.g, b + c.b); }
  Color operator-(const Color& c) const { return Color(r - c.r, g - c.g, b - c.b); }
  Color operator*(const Color& c) const { return Color(r * c.r, g * c.g, b * c.b); }
  Color operator/(const Color& c) const { return Color(r / c.r, g / c.g, b / c.b); }

  double sum() const { return r + g + b; }
  double length() const { return sqrt(r*r + g*g + b*b); }
  double luminance() const { return 0.299*r + 0.587*g + 0.114*b; } //яркость 
  Color& normalize();
  void toRGB(RGBA& p) const;

  static double mul(const Color& c1, const Color& c2) { return c1.r*c2.r + c1.g*c2.g + c1.b*c2.b; }
  static Color mix(const Color& c1, const Color& c2, double t) { return Color(c1.r + (c2.r - c1.r)*t, c1.g + (c2.g - c1.g)*t, c1.b + (c2.b - c1.b)*t); }
  static Color rand();
};

//------------------------------------------------------------------------------
//
//
class Image
{
public:
  Image() { data = NULL; w = 0; h = 0; }
  Image(int iw, int ih) { data = NULL; init(iw, ih); }
  Image(const Image& img) { data = NULL; init(img); }
  ~Image() { if (data != NULL) delete[] data; }

  Image& operator=(const Image& img) { init(img); return *this; }

  int width() const { return w; }
  int height() const { return h; }
  void init(int iw, int ih);
  void init(const Image& img);
  void clear(const RGBA& p);
  void clear(const Color& c)
  {
    RGBA p;
    c.toRGB(p);
    p.a = 0;
    clear(p);
  }
  void savePPM(const char* path, bool binary) const;
  void saveBMP(const char* path) const;

  void getPixel(int x, int y, RGBA& p) const
  {
    assert(data != NULL);
    assert(x >= 0 && x < w);
    assert(y >= 0 && y < h);
    p = data[y*w + x];
  }
  void getPixel(int x, int y, Color& c) const
  {
    assert(data != NULL);
    assert(x >= 0 && x < w);
    assert(y >= 0 && y < h);
    c.init(data[y*w + x]);
  }
  void setPixel(int x, int y, const RGBA& p)
  {
    assert(data != NULL);
    assert(x >= 0 && x < w);
    assert(y >= 0 && y < h);
    data[y*w + x] = p;
  }
  void setPixel(int x, int y, const Color& c)
  {
    assert(data != NULL);
    assert(x >= 0 && x < w);
    assert(y >= 0 && y < h);
    c.toRGB(data[y*w + x]);
  }

protected:
  RGBA* data;
  int w, h;
};

#endif
