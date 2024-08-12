# RGB-Images

## Project

In this project we used C++ to represent and process RGB color images with 8-bits per RGB channel.

## Features 

- Implementation of Color
- Implementation of Image
- Implementation of Script commands for initialization and I/O
- Implementation of Script commands for simple image manipulations
- Implementation of Script commands for dimension-changing operations
- Implementation of Script commands for functionality operations

## Commands for initialization and I/O

- blank w h r g b - Defines a new image with dimensions w × h and all pixels set to color (r, g, b).
- open filename - Reads a new image in PNG format from filename.
- save filename - Saves current image in PNG format to filename.
  
## Commands for simple image manipulations

- invert - Transforms each individual pixel (r, g, b) to (255-r,255-g,255-b).
- to_gray_scale - Transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3.
- replace r1 g1 b1 r2 g2 b2 - Replaces all (r1,  g1, b1) pixels by (r2,  g2, b2).
- fill x y w h r g b  - Assign (r, g, b) to all pixels contained in rectangle defined by top-left corner (x, y), width w, and height h, i.e., all pixels (x', y'). 
- h_mirror - Mirror image horizontally. 
- v_mirror - Mirror image vertically. 
- add filename r g b x v - Copy all pixels from image stored in PNG file filename, except pixels in that image with “neutral” color (r, g, b), to the rectangle of the current image with top-left corner (x, y) of the current image.

## Commands for dimension-changing operations

- crop x y w h - Crop the image, reducing it to all pixels contained in the rectangle defined by top-left corner (x, y), width w, and height h.
- rotate_left - Rotate image left by 90 degrees.
- rotate_right - Rotate image right by 90 degrees.

## Commands for functionality operations

- median_filter ws - Apply a median filter with window size ws >= 3 to the current image.
- xpm2_open filename - Read image stored in the XPM2 file format.
- xpm2_save filename - Save image to the XPM2 file format. 