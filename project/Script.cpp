#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <vector>
#include <algorithm>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    // constructor initializes the input stream with the contents of the file
    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }

    // helper function that deletes the current image if one exists. 
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }

    // destructor responsible for cleaning up any allocated resources
    Script::~Script() {
        clear_image_if_any();
    }

    // main entry point for executing the commands in the script file
    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            
            if (command == "invert") {
                invert();
                continue;
            }

            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }

            if (command == "replace") {
                replace();
                continue;
            }

            if(command == "fill") {
                fill();
                continue;
            }

            if(command == "h_mirror") {
                h_mirror();
                continue;
            }

            if(command == "v_mirror") {
                v_mirror();
                continue;
            }

            if(command == "add") {
                add();
                continue;
            }

            if(command == "crop") {
                crop();
                continue;
            }

            if(command == "rotate_left") {
                rotate_left();
                continue;
            }

            if(command == "rotate_right") {
                rotate_right();
                continue;
            }

            if(command == "median_filter") {
                median_filter();
                continue;
            }

            if(command == "xpm2_open") {
                xpm2_open();
                continue;
            }

            if(command == "xpm2_save") {
                xpm2_save();
                continue;
            }
        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    // Invert the colors of the image
    void Script::invert() {
        // for each pixel get a reference to the pixels color
        for(int j=0; j < image->height(); j++){
            for(int i=0; i < image->width(); i++){
                Color& c = image->at(i,j);

                // subtracts 255 from each of the pixels color components
                c.red() = 255 - c.red();
                c.green() = 255 - c.green();
                c.blue() = 255 - c.blue();
            }
        }
    }

    // Converts the image to grayscale
    void Script::to_gray_scale() {
        // for each pixel get a reference to the pixels color
        for(int i=0; i < image->height(); i++){
            for(int j=0; j < image->width(); j++){
                Color& c = image->at(j,i);

                // calculate the average of the pixels components
                rgb_value v = (c.red() + c.green() + c.blue()) / 3;

                // set the color components to the average value
                c.red() = v;
                c.green() = v;
                c.blue() = v;
            }
        }
    }   

    // Replaces all occurences of one color with another color
    void Script::replace() {
        // get the parameters
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;

        // for each pixel get a reference to the pixels color
        for(int y=0; y < image->height(); y++) {
            for (int x=0; x < image->width(); x++) {
                Color& c = image->at(x, y);

                // replace occurrences of a color with another color
                if (c.red() == r1 && c.green() == g1 && c.blue() == b1) {
                    c.red() = r2;
                    c.green() = g2;
                    c.blue() = b2;
                }
            }
        }     
    }

    // Fills an area of the image with a specified color 
    void Script::fill() {
        // get the parameters
        int x, y, w, h, r, g, b;
        input >> x >> y >> w >> h >> r >> g >> b;
        
        // for each pixel get a reference to the pixels color and fiils the area
        for (int i=x; i < x + w; i++) {
            for (int j=y; j < y + h; j++) {
                image->at(i, j) = Color(r, g, b);
            }
        }
    }

    // Mirrors the image horrizontally
    void Script::h_mirror() {
        int w = image->width();
        
        // for each pixel get a reference to the pixels color
        for (int y=0; y < image->height(); y++) {
            for (int x=0; x < w / 2; x++) {
                Color temp = image->at(x, y);

                // swaps the pixel's color with the color of the pixel at the same y-coordinate but the opposite x-coordinate.
                image->at(x, y) = image->at(w - 1 - x, y);
                image->at(w - 1 - x, y) = temp;
            }
        }
    }

    // Mirros the image vertically
    void Script::v_mirror() {
        int h = image->height();
        // for each pixel get a reference to the pixels color
        for (int y=0; y < h / 2; y++) {
            for (int x=0; x < image->width(); x++) {
                Color temp = image->at(x, y);

                // swaps the pixel's color with the color of the pixel at the same x-coordinate but the opposite y-coordinate.
                image->at(x, y) = image->at(x, h - 1 - y);
                image->at(x, h - 1 - y) = temp;
            }
        }
    }

    // Adds two images together
    void Script::add(){
        // get the parameters
        int x, y, r, g, b;
        string filename;
        input >> filename >> r >> g >> b >> x >> y;

        // load the image to be added
        Image* load = loadFromPNG(filename);

        // loop through each pixel in the image
        for(int i=0; i < load->width(); i++){
            for(int j=0; j < load->height(); j++){
                // check if the pixel is within the bounds of the destination image
                if(i+x < image->width() && j+y < image->height()){
                    Color temp = load->at(i,j);

                    // check if the color matches the specified color
                    if(temp.red() != r || temp.green() != g || temp.blue() != b){
                        image->at(x+i,y+j) = temp;
                    }
                }
            }
        }
        // delete the source image 
        delete load;
    }

    // Crops an image to a specified size
    void Script::crop(){
        // get the parameters
        int x, y, w, h;
        input >> x >> y >> w >> h;

        // create image with specified size
        Image* temp = new Image(w, h);

        // copy the pixels from the source image to the new image.
        for (int i=0; i < h; i++) {
            for (int j=0; j < w; j++) {
                temp->at(j, i) = image->at(x + j, y + i);
            }
        }
        // delete the source image 
        delete image;
        // set the new image as the current image.
        image = temp;            
    }

    // Rotates the image 90 degrees to the left
    void Script::rotate_left(){
        // create a new image with the same height and width as the current image.
        Image* left = new Image(image->height(), image->width());
        // loop through each pixel in the new image
        for(int i=0; i < left->width(); i++){
            for(int j=0; j < left->height(); j++){
                // set the pixel in the new image to the pixel in the current image but rotated 90 degrees to the left.
                left->at(i,j) = image->at(left->height()-1-j, i);
            }
        }
        // delete the source image 
        delete image;
        // set the new image as the current image.
        image = left;
    }            

    // Rotates the image 90 degrees to the right
    void Script::rotate_right() {
        // create a new image with the same height and width as the current image.
        Image* right = new Image(image->height(), image->width());
        // loop through each pixel in the new image
        for(int i = 0; i < right->width(); i++) {
            for(int j = 0; j < right->height(); j++) {
                // set the pixel in the new image to the pixel in the current image but rotated 90 degrees to the right.
                right->at(i, j) = image->at(j,right->width()-1-i);
            }
        }
        // delete the source image 
        delete image;
        // set the new image as the current image.
        image = right;
    }

    // Applies a median filter to the image. 
    void Script::median_filter(){
        int ws, mr, mg, mb;
        input >> ws;
        Image* median = new Image(image->width(),image->height());
        //Fill median with the values of image
        for(int i=0; i < image->width(); i++){
            for(int j=0; j < image->height(); j++){
                median->at(i,j) = image->at(i,j);
                vector<int> r, b, g;
                //Fill the rgb vectors with the rgb of each image pixel
                for(int nx=max(0, i-ws/2); nx <= min(image->width()-1, i+ws/2); nx++){
                    for(int ny=max(0, j-ws/2); ny <= min(image->height()-1, j+ws/2); ny++){
                        Color temp = image->at(nx,ny);
                        r.push_back(temp.red());
                        g.push_back(temp.green());
                        b.push_back(temp.blue());
                    }
                }
                
                sort(r.begin(), r.end());
                sort(g.begin(), g.end());
                sort(b.begin(), b.end());
                Color& c = median->at(i,j);
                //Check if the size of rgb vectors are odd or even
                if(r.size()%2 == 0){
                    mr = (r[r.size()/2-1] + r[r.size()/2])/2;
                    mg = (g[g.size()/2-1] + g[g.size()/2])/2;
                    mb = (b[b.size()/2-1] + b[b.size()/2])/2;
                }else{
                    mr = r[r.size()/2];
                    mg = g[g.size()/2];
                    mb = b[b.size()/2];
                }
                
                c.red() = mr;
                c.green() = mg;
                c.blue() = mb;
            }
        }
        
        delete image;
        image = median;
    }

    // Read image stored in the XPM2 file format
    void Script::xpm2_open() {
        clear_image_if_any();
        string filename;
        input >> filename;
        image = prog::loadFromXPM2(filename);
    }

    // Save image to the XPM2 file format
    void Script::xpm2_save() {
        // Print error if image object is null
        if (image == nullptr) {
            cout << "No image loaded. Cannot save." << endl;
            return;
        }
        string filename;
        input >> filename;
        prog::saveToXPM2(filename, image);
    }

}
