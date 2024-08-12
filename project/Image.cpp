#include "Image.hpp"

using namespace std;

namespace prog
{
  // constructor that creats the Image object
  Image::Image(int w, int h, const Color &fill)
  {
    this->w = w;
    this->h = h; 
    this->imagevector = vector<vector<Color>>(h, vector<Color>(w, fill));
  }

  // destructor for the Image class. 
  Image::~Image()
  {
  }

  // Getter that return the width or height of the image as an integer
  int Image::width() const
  {
    return w;
  }
  int Image::height() const
  {
    return h;
  }

  // Getter that returns a constant reference to the Color object at specified coordinates
  const Color& Image::at(int x, int y) const
  {
    return imagevector[y][x];
  }

  // Setter returns a reference to the Color object at the specified coordinates in the image
  Color& Image::at(int x, int y)
  {
    return imagevector[y][x];
  }
}
