
// CAN ÇİFTÇİ - 270201080
// CENG 391 - HW2

#include <cstdlib>
#include <iostream>

#include "ceng391/image.hpp"

using namespace std;
using ceng391::Image;

int main(int argc, char** argv)
{
        if (argc < 2) {
                cerr << "Usage: " << argv[0] << " <filename> [<filter-size>]" << endl;
                return EXIT_FAILURE;
        } 

        Image *img =  Image::from_png(argv[1], false);

         int filter_size = 5;
        if (argc > 2)
                filter_size = atoi(argv[2]);
        
        img->box_filter(filter_size);
        img->xsave_png("box.png");


        delete img;
        return EXIT_SUCCESS;
}

/// Local Variables:
/// mode: c++
/// compile-command: "make -C ../build image-box-filter"
/// End:
