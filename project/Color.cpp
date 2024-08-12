#include "Color.hpp"

namespace prog {
    // default constructor that initializaes all of the member variables to 0.
    Color::Color() {
        red_ = 0;
        green_ = 0;
        blue_ = 0;
    }
    // constructor that copies the values of the member variables from another object
    Color::Color(const Color& other) {
        red_ = other.red_;
        green_ = other.green_;
        blue_ = other.blue_;

    }
    // constructor that initializes the member variables to the specified values
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        red_ = red;
        green_ = green;
        blue_ = blue;
    }

    // Getters that return the color compoenente of the Color
    rgb_value Color::red() const {
        return red_;
    }
    rgb_value Color::green() const {
        return green_;
    }
    rgb_value Color::blue() const {
        return blue_;
    }

    // Setters that sets the color componenet of the Color 
    rgb_value& Color::red()  {
        return red_;
    }
    rgb_value& Color::green()  {
      return green_;
    }
    rgb_value& Color::blue()  {
      return blue_;
    }
}
