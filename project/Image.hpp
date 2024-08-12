#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__

#include "Color.hpp"
#include <vector>

namespace prog
{
  // define class that represents the weight, height and pixels of the image
  class Image
  {
  private:
    int w;
    int h;
    std::vector<std::vector<Color>> imagevector;
  public:
    // constructors 
    Image(int w, int h, const Color &fill = {255, 255, 255});
    // destructor
    ~Image();

    // Getters and Setters 
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;
  };
}
#endif