 // CAN ÇİFTÇİ -- 270201080

#include "ceng391/image.hpp"
#include <iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>

using namespace std;

namespace ceng391 {

Image::Image(int width, int height, int n_channels, int step)
{       
        //------------------NEW CODE---------------------//
        m_n_channels = n_channels;                                          // adding n_channels support to the constructer. if param is wrong terminate the program
        if (!(n_channels == 1 || n_channels == 3 || n_channels == 4))
        {
                std::cerr<<"ERROR : Wrong argument for n_channels. It can be only 1(grayscale), 3(RGB) or 4(RGBA)"
                <<std::endl;
                exit(EXIT_FAILURE);
        }
        
        //------------------NEW CODE---------------------//
        m_width = width;
        m_height = height;
        if (step < m_width*m_n_channels)
                step = m_width*m_n_channels;
        m_step = step;

        m_data = new uchar[m_step * m_height];
}
Image::~Image()
{
        delete [] m_data;
}

void Image::set_rect(int x_tl, int y_tl, int width, int height, uchar value)  //modified version for existing set_rect function for support rgb and rgba it checks m_n_channels and
{                                                                             // does the copying the values that desired format
        if (x_tl < 0) {
                width += x_tl;
                x_tl = 0;
        }

        if (y_tl < 0) {
                height += y_tl;
                y_tl = 0;
        }
        size_t length = min(width, m_width - x_tl);
        int y_max = min(y_tl + height, m_height);
        if (m_n_channels==1) {

            for (int y = y_tl; y < y_max; ++y) {
            uchar *row_y = data(y) + x_tl;
            memset(row_y, value, length);
            }
        }else if (m_n_channels==3){                                             //for RGB support
            this->set_rect_RGB(x_tl,y_tl,width,height,value,value,value);
        }else if (m_n_channels==4){                                             //for RGBA support
            this->set_rect_RGBA(x_tl,y_tl,width,height,value,value,value,255);
        }
}
//------------------NEW CODE---------------------//

void Image::set_rect_RGB(int x_tl, int y_tl, int width, int height, uchar red, uchar green, uchar blue)   // it creates rectangle with rgb support with desired size
{
        if (x_tl < 0) {
                width += x_tl;
                x_tl = 0;
        }

        if (y_tl < 0) {
                height += y_tl;
                y_tl = 0;
        }

        size_t length = min(width, m_width - x_tl);
        int y_max = min(y_tl + height, m_height);
        for (int y = y_tl; y < y_max; ++y) 
        {
                uchar *row_y = data(y) + x_tl;

                for (int x = x_tl; x < x_tl+width; ++x)                     // adding rgb values for every single pixel
                {
                        *(row_y + x*m_n_channels) = blue;
                        *(row_y + x*m_n_channels + 1) = red;
                        *(row_y + x*m_n_channels + 2) = green;
                }

        }
}
//------------------NEW CODE---------------------//

void Image::set_rect_RGBA(int x_tl, int y_tl, int width, int height, uchar red, uchar green, uchar blue, uchar alpha)   // RGBA version for set-rect function with alpha value
{
        if (x_tl < 0) {
                width += x_tl;
                x_tl = 0;
        }

        if (y_tl < 0) {
                height += y_tl;
                y_tl = 0;
        }

        size_t length = min(width, m_width - x_tl);
        int y_max = min(y_tl + height, m_height);
        for (int y = y_tl; y < y_max; ++y)
        {
                uchar *row_y = data(y) + x_tl;

                for (int x = x_tl; x < x_tl+width; ++x)                 // adding rgba values for every single pixel
                {
                        *(row_y + x*m_n_channels) = red;
                        *(row_y + x*m_n_channels + 1) = green;
                        *(row_y + x*m_n_channels + 2) = blue;
                        *(row_y + x*m_n_channels + 3) = alpha;
                }
                
        }
}
// -------------------------- MODIFIED VERSION -------------------------- //
void Image::xsave_pnm(const  std::string& filename) const                           // Modified version for xsave function. It supports rgb and rgba now
{
        string magic_head;
        if (m_n_channels==1){                                                       // checking channel info for magic head
            magic_head = "P5";
        }else if (m_n_channels == 3 || m_n_channels == 4){
            magic_head = "P6";
        }else{
            cerr<< "Object can not suitable for write\n";
            return;
        }
        ofstream fout;
        string extended_name = filename + ".pnm";
        fout.open(extended_name.c_str(), ios::out | ios::binary);
        if (fout.fail()){                                                           // If ofstream fails it returns error message and terminate immediately
            cerr<<"ERROR : Can't write the object to the target\n";
            exit(EXIT_FAILURE);
        }
        fout << magic_head << "\n";
        fout << m_width << " " << m_height << " 255\n";
        if (m_n_channels == 1)                                                         // checking format type and doing necessary file operations
        {
                for (int y = 0; y < m_height; ++y) 
                {
                        const uchar *row_data = this->data(y);
                        fout.write(reinterpret_cast<const char*>(row_data),
                        m_width*sizeof(uchar));
                }
        }
        else if (m_n_channels == 3)
        {
                for (int y = 0; y < m_height; ++y)
                {
                const uchar *row_data = this->data(y);
                fout.write(reinterpret_cast<const char*>(row_data),
                           m_width*sizeof(uchar)*m_n_channels);
                }
        }
        else if (m_n_channels == 4)
        {
                for (int y = 0; y < m_height; ++y)
                {
                const uchar *row_data = this->data(y);

                        for (int i = 0; i < m_width; i++)
                        {
                        fout.write(reinterpret_cast<const char*>(row_data),
                                        sizeof(uchar)*(m_n_channels-1));            // Discarding alpha values for writing file
                                row_data += m_n_channels;
                        }
                
                }
        }
        fout.close();
}
Image *Image::read_pnm(const std::string& filename)                     // new function for reading pnm files return image pointer
{
        int height,width,maxcolor;
        string magic_head;
        string extended_name = filename + ".pnm";
        ifstream fin;
        fin.open(extended_name.c_str(),ios::in | ios::binary);

        if (fin.fail())                                                 // fail check for reading file. If fail return an error message and terminate immediately
        {
                cerr << "ERROR : Could not open file " << filename << "\n";
                exit(EXIT_FAILURE);
        }
        
        fin >> magic_head >> width >> height >> maxcolor ;
        Image *img = nullptr;                                           // assigning nullptr to the Image pointer.

        if (magic_head == "P5")                                         // Reading pgm file. Creates a Image object that grayscale type and return object address
        {       
                img = new Image(width,height,1);
                uchar *ptr = img->m_data;
                for (int i = 0; i < width*height; i++)
                {
                        fin>>*ptr;
                        ptr += 1;
                }
                
        }
        else if (magic_head == "P6")                                    // Reading ppm file. Creates a Image object that rgba type with alpha value '255' and return object address
        {
                img = new Image(width,height,4);
                uchar *ptr = img->m_data;

                for (int i = 0; i < width*height*3; i++)
                {       
                        
                        fin>>*ptr;
                        if (i%3 == 2)
                        {
                                ptr += 1;
                                *ptr = 255;
                        }
                        ptr +=1;
                }
                
        }
        else                                                //printing error message for unknown format type. returns null pointer.
        {
                cerr << "ERROR : Invalid format type \n";

        }
        fin.close();
        return img;
}
Image *Image::to_rgb()                                      // Conversion function for rgb format. If already desired format immediately return
{       
        if (m_n_channels == 3)
        {
                return this;
        }
        Image *img = nullptr;
        uchar *original_data = m_data;
        img = new Image(m_width,m_height,3);       // creating new RGB object
        uchar *new_data = img->m_data;
        if (m_n_channels == 1)                              // for Grayscale
        {
                for (int i = 0; i < m_step*m_height; i++)
                {
                        *(new_data) = *(original_data + i);
                        *(new_data + 1) = *(original_data + i);
                        *(new_data + 2) = *(original_data + i);
                        new_data += 3;
                }
                return img;
        }
        else if (m_n_channels == 4)                         //for RGBA
        {
                for (int i = 0; i < m_step*m_height; i++)
                {
                        *(new_data + i) = *(original_data + i);
                        if (i % 3 == 2)
                        {
                                original_data += 1;
                        }

                }
                return img;
        }      
}
Image *Image::to_grayscale()                               // Conversion function for Grayscale format. If already desired format immediately return
{
        if (m_n_channels == 1)
        {
                return this;
        }
        Image *img = nullptr;
        uchar *original_data = m_data;
        img = new Image(m_width,m_height,1);    // creating grayscale object
        uchar *new_data = img->m_data;
        if (m_n_channels == 3)                            // for RGB. w.r.t.  I Gray (x, y) = I Red (x, y) ∗ 0.3 + I Green (x, y) ∗ 0.59 + I Blue (x, y) ∗ 0.11 formula
        {       

                for (int i = 0; i < m_step*m_height; i++)
                {
                        int value = ((*(original_data + i*3))*30 + (*(original_data + i*3 + 1))*59
                                                                   + (*(original_data + i*3 + 2))*11)/100;
                        if (value>255)
                        {
                            value=255;
                        }                                          // checking overflow and underflow before writing back
                        else if (value<0)
                        {
                            value = 0;
                        }

                        *(new_data+i) = value;
                }
                return img;
        }
        else if (m_n_channels == 4)                     // for RGBA. w.r.t.  I Gray (x, y) = I Red (x, y) ∗ 0.3 + I Green (x, y) ∗ 0.59 + I Blue (x, y) ∗ 0.11 formula with
        {                                               // discarding alpha values
                for (int i = 0; i < m_step*m_height; i++)
                {
                    int value = ((*(original_data + i*4))*30 + (*(original_data + i*4 + 1))*59
                                                                   + (*(original_data + i*4 + 2))*11)/100;
                    if (value>255)
                    {
                        value=255;
                    }                                               // checking overflow and underflow before writing back
                    else if (value<0)
                    {
                        value=0;
                    }
                    *(new_data + i) = value;

                }
                return img;
        }
}
Image *Image::to_rgba()                                 // Conversion function for RGBA format. If already desired format immediately return
{
        if (m_n_channels == 4)
        {
                return this;
        }
        Image *img = nullptr;
        uchar *original_data = m_data;
        img = new Image(m_width,m_height,4);        // creating RGBA object
        uchar *new_data = img->m_data;
        if (m_n_channels == 1)                  //for Grayscale. with default alpha value '255'
        {       

                for (int i = 0; i < m_step*m_height; i++)
                {
                        *(new_data) = *(original_data + i);
                        *(new_data + 1) = *(original_data + i);
                        *(new_data + 2) = *(original_data + i);
                        *(new_data + 3) = 255;
                        new_data += 4;
                }
                return img;
        }
        else if (m_n_channels == 3)              // for RGB. with default alpha value '255'
        {
                for (int i = 0; i < m_step*m_height; i++)
                {       
                        *(new_data) = *(original_data + i);
                        if (i % 3 == 2)
                        {
                                new_data += 1;
                                *new_data = 255;
                        }
                        new_data+=1;
                }
                return img;
        }
}
}
