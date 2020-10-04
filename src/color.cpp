#include <cstdlib>
#include <cstring>
#include <fstream>
#include "color.h"

//------------------------------------------------------------------------------
//
//
Color& Color::normalize()
{ 
  if (r < 0.0)
    r = 0.0;
  else if (r > 1.0)
    r = 1.0;
  if (g < 0.0)
    g = 0.0;
  else if (g > 1.0)
    g = 1.0;
  if (b < 0.0)
    b = 0.0;
  else if (b > 1.0)
    b = 1.0;
  return *this;
}

void Color::toRGB(RGBA& p) const
{
  p.r = (int)(r*255 + 0.5);
  p.g = (int)(g*255 + 0.5);
  p.b = (int)(b*255 + 0.5);
}

Color Color::rand()
{
  Color c(::rand(), ::rand(), ::rand());
  c *= 1.0/RAND_MAX;
  return c;
}

//------------------------------------------------------------------------------
//
//
void Image::init(int iw, int ih)
{
  if (data != NULL)
  {
    delete[] data;
    data = NULL;
  }
  w = iw;
  h = ih;
  if (w > 0 && h > 0)
  {
    data = new RGBA[w*h];
    memset(data, 0, w*h*sizeof(RGBA));
  }
}

void Image::init(const Image& img)
{
  if (this == &img)
    return;
  if (data != NULL)
  {
    delete[] data;
    data = NULL;
  }
  w = img.w;
  h = img.h;
  if (w > 0 && h > 0)
  {
    data = new RGBA[w*h];
    memcpy(data, img.data, w*h*sizeof(RGBA));
  }
}

void Image::clear(const RGBA& p)
{
  assert(data != NULL);
  int n = w*h;
  for (int i = 0; i < n; i++)
    data[i] = p;
}

void Image::savePPM(const char* path, bool binary) const
{
  assert(data != NULL);
  std::ofstream file(path);
  file << 'P' << (binary ? '6' : '3') << std::endl;
  file << w << ' ' << h << std::endl;
  file << 255 << std::endl;

  RGBA* p = data;
  if (binary)
  {
    file.close();
    file.open(path, std::ios_base::binary | std::ios_base::app);
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++, p++)
        file << p->r << p->g << p->b;
  }
  else
  {
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++, p++)
        file << (int)p->r << ' ' << (int)p->g << ' ' << (int)p->b << std::endl;
  }
  file.close();
}

void Image::saveBMP(const char* path) const
{
  assert(data != NULL);
  std::ofstream file(path, std::ios::binary);
  int rest = w*3%4;
  if (rest > 0)
    rest = 4 - rest;
#pragma pack(push, 1)
  struct
  {
    uint16_t type;         // type of file, must be 'BM'
    uint32_t size;         // size of file in bytes
    uint32_t reserved;     //
    uint32_t offset;       // offset from this header to actual data
  } header;
  struct
  {
    uint32_t size;         // size of struct
    uint32_t width;        // width of image in pixels
    uint32_t height;       // height of image in pixels
    uint16_t planes;       // number of planes
    uint16_t bits;         // bits per pixel
    uint32_t compression;  // type of compression, 0 - no compression
    uint32_t sizeImage;    // size of image in bytes
    uint32_t xResolution;  // horizontal resolution of the target device
    uint32_t yResolution;  // vertical resolution of the target device
    uint32_t clrUsed;      // number of colors used
    uint32_t clrImportant; // number of important colors
  } info;
#pragma pack(pop)
  memset(&header, 0, sizeof(header));
  header.type = 0x4D42;
  header.size = sizeof(header) + sizeof(info) + (w*3 + rest)*h;
  header.offset = sizeof(header) + sizeof(info);
  file.write((char*)&header, sizeof(header));

  memset(&info, 0, sizeof(info));
  info.size = sizeof(info);
  info.width = w;
  info.height = h;
  info.planes = 1;
  info.bits = 24;
  file.write((char*)&info, sizeof(info));
  for (int y = h - 1; y >= 0; y--)
  {
    RGBA* p = data + y*w;
    for (int x = 0; x < w; x++, p++)
      file << p->b << p->g << p->r;
    for (int i = 0; i < rest; i++)
      file << '\0';
  }
  file.close();
}
