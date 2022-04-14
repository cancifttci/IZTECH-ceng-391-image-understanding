
// CAN ÇİFTÇİ - 270201080
// CENG 391 - HW2



// USAGE ./image-test watch.png



#include <cstdlib>
#include <iostream>

#include "ceng391/image.hpp"

using namespace std;
using ceng391::Image;

int main(int argc, char** argv)
{
        /* Image *img = nullptr;
        if (argc == 1) {
                img = Image::new_grayscale(320, 240);
                cout << "Created image of size (" << img->w()
                     << "x" << img->h() << ")!" << endl;
                img->set(255);
        } else {
                img = Image::from_png(argv[1], false);
        }

        //img->set_rect_rgba(100, -50, 1000, 150, 0, 255, 0, 128);
        //img->xsave_pnm("test_img");
        //img->xsave_png("test_img.png");

        delete img;
 */
        if (argc < 2) {
                cerr << "Usage: " << argv[0] << " <filename>" << endl;
                return EXIT_FAILURE;
        }

                // ----------------------- TEST CASES FOR EXERCISE 1 -------------------------//
        

        // IMAGE::SMOOTH_X FUNCTION FOR SIGMA=10.0

        Image *img =  Image::from_png(argv[1], false);

        img->smooth_x(10.0);
        img->xsave_png("SMOOTH_X.png");

        // IMAGE::SMOOTH_Y FUNCTION FOR SIGMA=10.0

        img =  Image::from_png(argv[1], false);

        img->smooth_y(10.0);
        img->xsave_png("SMOOTH_Y.png");

        //IMAGE::SMOOTH FUNCTION FOR SIGMA=10.0

        img =  Image::from_png(argv[1], false);
        
        img->smooth(10.0);
        img->xsave_png("SMOOTH_BOTH.png");
        
        
        // ----------------------- TEST CASES FOR EXERCISE 2 -------------------------//


        //IMAGE::DERIV_X FUNCTION

        img =  Image::from_png(argv[1], false);

        short *ptr;
        ptr = img->deriv_x();
        img->xsave_png("DERIV_X.png");

        //IMAGE::DERIV_Y FUNCTION

        img =  Image::from_png(argv[1], false);

        ptr = img->deriv_y();
        img->xsave_png("DERIV_Y.png");


        // ----------------------- TEST CASES FOR EXERCISE 3 -------------------------//

        // USAGE : rotate(angle,source image pointer, boolean for bilinear, bool for rotate around center)

        //IMAGE::ROTATE FUNCTION (the angle is in degree not radian) AROUND ORIGIN

        img =  Image::from_png(argv[1], false);
        Image *img1 =  Image::from_png(argv[1], false);

        img->rotate(30,img1,0,0);
        img1->xsave_png("ROTATED_AROUND_ORIGIN_NEARESTNEIGHBOUR1.png");

        img1 =  Image::from_png(argv[1], false);
        img->rotate(45,img1,0,0);
        img1->xsave_png("ROTATED_AROUND_ORIGIN_NEARESTNEIGHBOUR2.png");

        img1 =  Image::from_png(argv[1], false);
        img->rotate(60,img1,1,0);
        img1->xsave_png("ROTATED_AROUND_ORIGIN_BILINEAR.png");                          //BILINEAR

        //ROTATION AROUND CENTER
        img1 =  Image::from_png(argv[1], false);
        img->rotate(30,img1,0,1);
        img1->xsave_png("ROTATED_AROUND_CENTER_NEARESTNEIGHBOUR3.png");

        img1 =  Image::from_png(argv[1], false);
        img->rotate(270,img1,0,1);
        img1->xsave_png("ROTATED_AROUND_CENTER_NEARESTNEIGHBOUR4.png");

        img1 =  Image::from_png(argv[1], false);
        img->rotate(60,img1,1,1);
        img1->xsave_png("ROTATED_AROUND_CENTER_BILINEAR.png");                          //BILINEAR

        
        delete img;
        delete img1;

        return EXIT_SUCCESS;
}

/// Local Variables:
/// mode: c++
/// compile-command: "make -C ../build image-test"
/// End:
