#include "XPM2.hpp"
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <iomanip>

namespace std {
    // Hash specialization for the 'Color' struct
    template <>
    struct hash<prog::Color> {
        std::size_t operator()(const prog::Color& color) const {
            // Combine the hash values of the individual color components
            std::size_t hashValue = std::hash<prog::rgb_value>()(color.red());
            hashValue ^= std::hash<prog::rgb_value>()(color.green());
            hashValue ^= std::hash<prog::rgb_value>()(color.blue());
            return hashValue;
        }
    };
}

namespace prog {

    //overload of operator< for comparing Color objects in unordered_map
    bool operator<(const prog::Color& color1, const prog::Color& color2){
        // Order colors based on their magnitude
        int mag1 = color1.red() + color1.green() + color1.blue();
        int mag2 = color2.red() + color2.green() + color2.blue();
        
        return mag1 < mag2;
    }

    // Operator overload for comparing equality of 'Color' objects
    bool operator==(const prog::Color& color1, const prog::Color& color2){
        return (color1.red() == color2.red()) &&
               (color1.green() == color2.green()) &&
               (color1.blue() == color2.blue());
    }

    //load an image from an XPM2 file format
    Image* loadFromXPM2(const std::string& file) {
        std::ifstream input(file);

        // Check if the file was successfully opened
        if (!input.is_open()) {
            std::cout << "Failed to open the file" << std::endl;
            return nullptr; 
        }

        std::string line;

        // Read the XPM2 header 
        if (!std::getline(input, line)) {
            input.close();
            std::cout << "error reading the first line" << std::endl;
            return nullptr; 
        }

        // read the values line  (w, h, n, c)
        if (!getline(input, line)) {
            input.close();
            std::cout << "Failed to read values line" << std::endl;
            return nullptr; 
        }

        //parse values
        std::istringstream valuesStream(line);
        int width, height, colors, charPerPixel;
        if (!(valuesStream >> width >> height >> colors >> charPerPixel)) {
            input.close();
            std::cout << "Invalid values" << std::endl;
            return nullptr;
        }
                
        // read the colors chars section
        std::unordered_map<char, Color> colorMap;

        //for each color line
        for (int i = 0; i < colors; i++){

            //read color entry
            if (!getline(input, line)) {
                input.close();
                std::cout << "Failed to read color entry" << std::endl;
                return nullptr; 
            }

            //each symbol is at the first position of the line
            char character = line[0];
            
            //colorCode starts at position 4 of the line
            std::string colorCode = line.substr(4);

            
            // Extract individual RGB values from colorCode
            std::string redHex = colorCode.substr(1, 2);
            std::string greenHex = colorCode.substr(3, 2);
            std::string blueHex = colorCode.substr(5, 2);

            // Convert hexadecimal values to decimal integers
            int red = std::stoi(redHex, nullptr, 16);
            int green = std::stoi(greenHex, nullptr, 16);
            int blue = std::stoi(blueHex, nullptr, 16);

            //sets each character to the corresponding Color in the map
            colorMap[character] = Color(red, green, blue);
        }
     
        //constructs an image from the symbols data
        Image* image = new Image(width, height);

        //iterates through all lines
        for (int y=0; y<height; y++){
            
            //checks if the pixel section has the correct size
            if (!std::getline(input, line) || static_cast<int>(line.length()) < width) {
                input.close();
                delete image;
                std::cout << "missing pixel data" << std::endl;
                return nullptr;
            }

            //iterates through all columns in the line
            for (int x = 0; x < width; x++) {
                char pixel = line[x];

                //sets the corresponding color of the image pixel
                if (colorMap.count(pixel)) {
                    image->at(x, y) = colorMap[pixel]; 
                } 

                // pixel symbol not defined in the color section
                else {
                    std::cout << "this char is not in the map" << std::endl;
                    input.close();
                    delete image;
                    return nullptr; 
                }
            }
        }
        input.close();
        return image; //returns the valid image pointer
    }

    //auxiliary function to return hexadecimal value string
    std::string toHex(int value){
        std::string hexString;
        const std::string hexChars = "0123456789ABCDEF";

        // algorithm to convert decimal value to hexadecimal
        while (value > 0){
            int digit = value %16;
            hexString = hexChars[digit] + hexString;
            value /= 16;
        }

        // Pad with leading zero if necessary
        while (hexString.length() < 2) {
            hexString = "0" + hexString; 
        }

        return hexString;
    }

    //save a image object to a file in the xpm2 format
    void saveToXPM2(const std::string& file, const Image* image) {
        std::ofstream output(file);

        //checks if successfully opened the output file
        if (!output.is_open()) {
            std::cerr << "Failed to open the file: " << file << std::endl;
            return;
        }

        //write header
        output << "! XPM2" << std::endl;

        //defines the values width and height values
        int width = image->width();
        int height = image->height();

        // Map colors to characters
        std::unordered_map<Color, char> colorCodeMap; 
        
        int colors = 0; 

        // iterates over each line
        for (int y = 0; y < height; y++) {

            // iterates over each column
            for (int x = 0; x < width; x++) {
                
                //add unique colors to the map and assign characters to them
                Color color = image->at(x, y);
                if (colorCodeMap.find(color) == colorCodeMap.end()) {
                    char character = 'A' + colors;
                    colorCodeMap[color] = character;
                    colors++;
                }
            }
        }

        // Characters per pixel
        int charPerPixel = 1;  

        //write the values section
        output << width << " " << height << " " << colors << " " << charPerPixel << std::endl;

        // iterates through all pairs in the colorCodeMap
        for (const auto& pair : colorCodeMap) {
            Color color = pair.first;
            char character = pair.second;

            //defines each string hexadecimal representation of rgb values
            std::string hexRed = toHex(color.red());
            std::string hexGreen = toHex(color.green());
            std::string hexBlue = toHex(color.blue());

            //write each color codification line
            output << character << " c #" << hexRed << hexGreen << hexBlue << std::endl;
        }

         //iterates through all lines
        for (int y = 0; y < height; y++) {

            //iterates through all columns
            for (int x = 0; x < width; x++) {
                Color color = image->at(x, y);

                //writes the corresponding char of the pixel color
                char character = colorCodeMap[color];
                output << character;
            }

            output << std::endl;
        }

        output.close(); //closes opened files for good programming practices
    }
}