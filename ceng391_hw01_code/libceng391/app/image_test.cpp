// CAN ÇİFTÇİ -- 270201080
#include <cstdlib>
#include <iostream>

#include "ceng391/image.hpp"

using namespace std;
using ceng391::Image;

int main(int /*argc*/, char** /*argv*/)
{
        /************************ EXERCISE-1 *************************/

        cout<<"-----------------------------EXERCISE-1------------------------------\n";

        Image testImage(640,360,1);  // adding n_channels support to the constructure
        cout << "Created image of size (" << testImage.w()<< "x" << testImage.h() << ") with " << testImage.n_channels() << " channels " << endl;
        Image testImage1(640,360,3);
        cout << "Created image of size (" << testImage1.w()<< "x" << testImage1.h() << ") with " << testImage1.n_channels() << " channels " << endl;
        Image testImage2(640,360,4);
        cout << "Created image of size (" << testImage2.w()<< "x" << testImage2.h() << ") with " << testImage2.n_channels() << " channels " << endl;

        //Image testImage3(640,360,2);    // ------->>>>>>>   when this line is execute, program will terminate immidiately for wrong parameter for n_channels


        testImage.set(255);
        testImage.set_rect(20,20,40,40,38);
        testImage.xsave_pnm("Test_image_grayscale");

        testImage1.set(150);   //support for set rect function for rgb images
        testImage1.set_rect_RGB(40,40,90,60,56,120,40);
        testImage1.xsave_pnm("Test_image_rgb");

        testImage2.set_RGBA(255,65,150,255);
        testImage2.set_rect_RGBA(0,0,200,100,65,89,16,140);
        testImage2.xsave_pnm("Test_image_rgba");

        /************************ EXERCISE-2 *************************/

        cout<<"-----------------------------EXERCISE-2------------------------------\n";

        Image *img1 = nullptr;
        img1 = img1->read_pnm("Test_image_grayscale");
        cout << "The first image that read size (" << img1->w() << "x" << img1->h() << ") with " << img1->n_channels() << " channels " << endl;

        Image *img2 = nullptr;
        img2 = img2->read_pnm("Test_image_rgb");
        cout << "The second image that read size (" << img2->w()<< "x" << img2->h() << ") with " << img2->n_channels() << " channels " << endl;

        Image *img3 = nullptr;
        img3 = img3->read_pnm("Test_image_rgba");
        cout << "The third image that read size (" << img3->w()<< "x" << img3->h() << ") with " << img3->n_channels() << " channels " << endl;

        /************************ EXERCISE-3 *************************/

        cout<<"-----------------------------EXERCISE-3------------------------------\n";

        // In this exercise, objects are comes from exercise 2. That mean read_pnm succesfully read the files

        cout<< "img1 now has "<<img1->n_channels()<< " channels\n";

        img1 = img1->to_grayscale();
        img1->xsave_pnm("grayscale-to-grayscale");
        cout<< "img1 has "<<img1->n_channels()<<" channels after executing to_grayscale function\n";

        img1 = img1->to_rgb();
        img1->xsave_pnm("grayscale-to-rgb");
        cout<< "img1 has "<<img1->n_channels()<<" channels after executing to_rgb function\n";

        img1 = img1->to_grayscale();       // returning back to grayscale

        img1 = img1->to_rgba();
        img1->xsave_pnm("grayscale-to-rgba");
        cout<< "img1 has "<<img1->n_channels()<<" channels after executing to_rgba function\n";


        cout<<"- - - - - - - - - - - - - - - - - - - - - - - - - - \n";

        cout<< "img2 now has "<<img2->n_channels()<< " channels\n";

        img2 = img2->to_rgba();
        img2->xsave_pnm("rgba-to-rgba");
        cout<< "img2 has "<<img2->n_channels()<<" channels after executing to_rgba function\n";

        img2 = img2->to_rgb();
        img2->xsave_pnm("rgba-to-rgb");
        cout<< "img2 has "<<img2->n_channels()<<" channels after executing to_rgb function\n";

        img2 = img2->to_rgba();     // returning back to rgba

        img2 = img2->to_grayscale();
        img2->xsave_pnm("rgba-to-grayscale");
        cout<< "img2 has "<<img2->n_channels()<<" channels after executing to_grayscale function\n";

        cout<<"- - - - - - - - - - - - - - - - - - - - - - - - - - \n";

        img3 = img3->to_rgb();                          // since read_pnm is converting rgb files to rgba by default I changed img3 from rgba to rgb
        cout<< "img3 now has "<<img3->n_channels()<< " channels\n";

        img3 = img3->to_rgb();
        img3->xsave_pnm("rgb-to-rgb");
        cout<< "img3 has "<<img3->n_channels()<<" channels after executing to_rgb function\n";

        img3 = img3->to_rgba();
        img3->xsave_pnm("rgb-to-rgba");
        cout<< "img3 has "<<img3->n_channels()<<" channels after executing to_rgba function\n";

        img3 = img3->to_rgb();                          //returning back to rgb

        img3 = img3->to_grayscale();
        img3->xsave_pnm("rgb-to-grayscale");
        cout<< "img3 has "<<img3->n_channels()<<" channels after executing to_grayscale function\n";

        return EXIT_SUCCESS;
}

