
// CAN ÇİFTÇİ - 270201080
// CENG 391 - HW2


#include "ceng391/image.hpp"

#include <cstdlib>
#include <cstdio>
#include<fstream>
#include<iostream>
#include<cstring>
#include <stdexcept>

#include <png.h>

#include <cmath>

#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif

#ifndef png_infopp_NULL
#  define png_infopp_NULL (png_infopp)NULL
#endif

#ifndef int_p_NULL
# define int_p_NULL (int*)NULL
#endif

const double PI = 3.1415926535897;

using namespace std;

namespace ceng391 {

Image::Image(int width, int height, int n_channels, int step)
{
        m_width = width;
        m_height = height;
        m_n_channels = n_channels;
        int row_len = m_width * m_n_channels;
        if (step < row_len)
                step = row_len;
        m_step = step;

        m_data = new uchar[m_step * m_height];
}

Image::~Image()
{
        delete [] m_data;
}

void Image::reallocate(int width, int height, int n_channels)
{
        if (width  != this->m_width ||
            height != this->m_height ||
            n_channels != this->m_n_channels) {
                delete [] m_data;
                int step = width * n_channels;
                m_step = step;
                m_data = new uchar[height * m_step];
                m_width = width;
                m_height = height;
                m_n_channels = n_channels;
        }
}

void Image::set_rect_rgba(int x_tl, int y_tl, int width, int height,
                          uchar red, uchar green, uchar blue, uchar alpha)
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
        if (m_n_channels == 4) {
                for (int y = y_tl; y < y_max; ++y) {
                        uchar *row_y = data(y) + x_tl;
                        for (int x = x_tl; x < x_tl + length; ++x) {
                                row_y[4*x]   = red;
                                row_y[4*x+1] = green;
                                row_y[4*x+2] = blue;
                                row_y[4*x+3] = alpha;
                        }
                }
        } else if (m_n_channels == 3) {
                for (int y = y_tl; y < y_max; ++y) {
                        uchar *row_y = data(y) + x_tl;
                        for (int x = x_tl; x < x_tl + length; ++x) {
                                row_y[3*x]   = red;
                                row_y[3*x+1] = green;
                                row_y[3*x+2] = blue;
                        }
                }
        } else if (m_n_channels == 1) {
                int value = 0.3f * red + 0.59f * green + 0.11f * blue;
                if (value > 255)
                        value = 255;
                for (int y = y_tl; y < y_max; ++y) {
                        uchar *row_y = data(y) + x_tl;
                        memset(row_y, value, length);
                }
        } else {
                cerr << "Unexpected number of channels ("
                     << m_n_channels << ") in call to set_rect_xxx()" << endl;
                exit(EXIT_FAILURE);
        }
}

void Image::xsave_pnm(const  std::string& filename) const
{
        if (m_n_channels != 1) {
                cerr << "xsave_pnm() works only with grayscale images" << endl;
                exit(EXIT_FAILURE);
        }
        const string magic_head = "P5";
        ofstream fout;
        string extended_name = filename + ".pgm";
        fout.open(extended_name.c_str(), ios::out | ios::binary);
        fout << magic_head << "\n";
        fout << m_width << " " << m_height << " 255\n";
        for (int y = 0; y < m_height; ++y) {
                const uchar *row_data = this->data(y);
                fout.write(reinterpret_cast<const char*>(row_data),
                           m_width*sizeof(uchar));
        }
        fout.close();
}

void Image::xsave_png(const std::string &filename) const
{
        // We open the output file with C style IO since we are using libpng
        // C-API
        FILE *fout = fopen(filename.c_str(), "w+b");
        if (!fout) {
                cerr << "[ERROR][CENG391::Image] Failed open file for writing: " << filename << endl;
                exit(EXIT_FAILURE);
        }

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                      0, 0, 0);
        if (!png_ptr) {
                cerr << "[ERROR][CENG391::Image] Failed to create PNG write structure!" << endl;
                fclose(fout);
                exit(EXIT_FAILURE);
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
                cerr << "[ERROR][CENG391::Image] Failed to create PNG info structure!" << endl;
                png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
                fclose(fout);
                exit(EXIT_FAILURE);
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
                cerr << "[ERROR][CENG391::Image] Failed to create PNG jump buffer!" << endl;
                png_destroy_write_struct(&png_ptr, &info_ptr);
                fclose(fout);
                exit(EXIT_FAILURE);
        }

        int color_type;
        switch (this->m_n_channels) {
        case 1: color_type = PNG_COLOR_TYPE_GRAY; break;
        case 3: color_type = PNG_COLOR_TYPE_RGB; break;
        case 4: color_type = PNG_COLOR_TYPE_RGBA; break;
        default:
                cerr << "[ERROR][CENG391::Image] Unsupported image type for saving as PNG!" << endl;
                png_destroy_write_struct(&png_ptr, &info_ptr);
                fclose(fout);
                exit(EXIT_FAILURE);
        }

        png_init_io(png_ptr, fout);
        png_set_IHDR(png_ptr, info_ptr, this->m_width, this->m_height, 8,
                     color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_write_info(png_ptr, info_ptr);

        png_bytepp row_pointers = (png_bytepp)malloc(this->m_height * sizeof(png_bytep));
        if (!row_pointers) {
                cerr << "[ERROR][CENG391::Image]Error creating PNG row pointers" << endl;
                png_destroy_write_struct(&png_ptr, &info_ptr);
                fclose(fout);
                exit(EXIT_FAILURE);
        }

        for (png_int_32 k = 0; k < this->m_height; k++) {
                row_pointers[k] = (png_bytep)(this->data(k));
        }

        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, info_ptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);
        free(row_pointers);
        fclose(fout);
}


void Image::xload_png(const std::string &filename, bool load_as_grayscale)
{
        // We open the output file with C style IO since we are using libpng
        // C-API
        FILE *fin = fopen(filename.c_str(), "r+b");
        if (!fin) {
                cerr << "[ERROR][CENG391::Image] Failed to open file for reading: " << filename << endl;
                exit(EXIT_FAILURE);
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                     NULL, NULL, NULL);
        if (!png_ptr) {
                cerr << "[ERROR][CENG391::Image] Could not create PNG read structure" << endl;
                fclose(fin);
                exit(EXIT_FAILURE);
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
                cerr << "[ERROR][CENG391::Image] Could not create PNG info pointer" << endl;
                png_destroy_read_struct(&png_ptr, png_infopp_NULL,
                                        png_infopp_NULL);
                fclose(fin);
                exit(EXIT_FAILURE);
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
                cerr << "[ERROR][CENG391::Image] Could not set jump point for reading PNG file" << endl;
                png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
                fclose(fin);
                exit(EXIT_FAILURE);
        }

        png_init_io(png_ptr, fin);
        png_read_info(png_ptr, info_ptr);

        png_uint_32 stream_width, stream_height;
        int bit_depth, color_type, interlace_type;
        png_get_IHDR(png_ptr, info_ptr, &stream_width, &stream_height, &bit_depth, &color_type,
                     &interlace_type, int_p_NULL, int_p_NULL);

        png_set_strip_16(png_ptr);
        if (color_type == PNG_COLOR_TYPE_GA) {
                png_set_strip_alpha(png_ptr); /*(not recommended). */
        }

        png_set_packing(png_ptr);
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
                png_set_palette_to_rgb(png_ptr);
        }

        png_set_expand(png_ptr);

        // Depending on the type of image in the file and the load_as_grayscale
        // flag, we determine the desired number of channels of the output
        // image.
        int desired_n_channels = 4;
        if (load_as_grayscale) {
                desired_n_channels = 1;
                png_set_rgb_to_gray_fixed(png_ptr, 1, 30000, 59000);
                png_set_strip_alpha(png_ptr); /*(not recommended). */
        } else {
                if (color_type == PNG_COLOR_TYPE_GRAY ||
                    color_type == PNG_COLOR_TYPE_GA) {
                        desired_n_channels = 1;
                }

                if(color_type == PNG_COLOR_TYPE_RGB ||
                   color_type == PNG_COLOR_TYPE_PALETTE) {
                        png_set_add_alpha(png_ptr, 255, PNG_FILLER_AFTER);
                }
        }

        // If the current image dimensions do not match the image to be loaded,
        // then reallocate with the desired dimensions.
        reallocate(stream_width, stream_height, desired_n_channels);

        png_bytepp row_pointers = (png_bytepp)malloc(this->m_height * sizeof(png_bytep));
        if (!row_pointers) {
                cerr << "[ERROR][CENG391::Image]Error creating PNG row pointers" << endl;
                png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
                fclose(fin);
                exit(EXIT_FAILURE);
        }
        for (int k = 0; k < this->m_height; k++) {
                row_pointers[k] = (png_bytep)(this->data(k));
        }

        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, info_ptr);

        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

        free(row_pointers);

        fclose(fin);
}

void copy_row_to_buffer(uchar* buffer, const uchar* row,
                        int n, int border_size)
{
        memcpy(reinterpret_cast<void *>(buffer + border_size),
               reinterpret_cast<const void *>(row),
               n*sizeof(*buffer));

        for (int i = 0; i < border_size; ++i) {
                buffer[i] = buffer[border_size];
                buffer[n-i-1] = buffer[n+border_size-1];
        }
}

void copy_column_to_buffer(uchar* buffer, const uchar* column,
                           int n, int step, int border_size)
{
        for (int i = 0; i < n; ++i) {
                buffer[border_size + i] = column[i * step];
        }

        for (int i = 0; i < border_size; ++i) {
                buffer[i] = buffer[border_size];
                buffer[n-i-1] = buffer[n+border_size-1];
        }
}

void box_filter_buffer(int n, uchar* buffer, int filter_size)
{
        for(int i = 0; i < n; ++i) {
                int sum = 0;
                for (int j = 0; j < filter_size; ++j)
                        sum += buffer[i + j];
                sum /= filter_size;
                buffer[i] = sum;
        }
}

void Image::box_filter_x(int filter_size)
{
        if (m_n_channels != 1) {
                cerr << "Can only box filter grayscale images" << endl;
                exit(EXIT_FAILURE);
        }
        int border_size = filter_size / 2;
        filter_size = 2*border_size + 1;

        int lbuffer = 2*border_size + m_width;
        uchar *buffer = new uchar[lbuffer];

        for (int y = 0; y < m_height; ++y) {
                copy_row_to_buffer(buffer, data(y), m_width, border_size);
                box_filter_buffer(m_width, buffer, filter_size);
                memcpy(reinterpret_cast<void *>(data(y)),
                       reinterpret_cast<const void *>(buffer),
                       m_width*sizeof(*buffer));
        }

        delete [] buffer;
}

void Image::box_filter_y(int filter_size)
{
        if (m_n_channels != 1) {
                cerr << "Can only box filter grayscale images" << endl;
                exit(EXIT_FAILURE);
        }
        int border_size = filter_size / 2;
        filter_size = 2*border_size + 1;

        int lbuffer = 2*border_size + m_height;
        uchar *buffer = new uchar[lbuffer];

        for (int x = 0; x < m_width; ++x) {
                copy_column_to_buffer(buffer, m_data + x, m_height,
                                      m_step, border_size);
                box_filter_buffer(m_height, buffer, filter_size);
                for (int y = 0; y < m_height; ++y)
                        m_data[x + y*m_step] = buffer[y];
        }

        delete [] buffer;
}

void Image::box_filter(int filter_size)
{
        box_filter_x(filter_size);
        box_filter_y(filter_size);
}

void Image::smooth_x(float sigma){

        if (m_n_channels != 1) {
                cerr << "Can only gaussian filter [x] grayscale images" << endl;
                exit(EXIT_FAILURE);
        }

        int kernel_radius = round(sigma);
        int kernel_size = 2*(kernel_radius) + 1;                // assuming after 2*sigma is 0 according to HW2
        int border_size = kernel_size/2;
        int x = -(kernel_size/2);
        double kernel[kernel_size];

        uchar *buffer = new uchar[2*border_size + m_width];

        //creating filter 
        double sum = 0.0;
        int t = 0;

        for (int i = x; i <= -(x); i++)         //filling kernel coefficients
        {
                kernel[t] = exp(-(0.5)*(i*i)/(sigma*sigma));
                sum += kernel[t];
                t += 1;
        }

        for (int i = 0; i < kernel_size; i++)   //normalizing
        {
                kernel[i] /= sum;
        }
        
        for (int y = 0; y < m_height; y++)      //applying kernel to the image in x-direction
        {       
                copy_row_to_buffer(buffer, data(y), m_width, border_size);
                
                for (int x = 0; x < m_width; x++)
                {       
                        int temp_sum = 0;
                        for (int i = 0; i < kernel_size; i++)
                        {
                                temp_sum += buffer[x + i]*kernel[i];
                        }
                        m_data[y*m_step + x] = uchar(temp_sum);                         //assigning calculated value
                }
        }
        delete [] buffer;
}

void Image::smooth_y(float sigma){

        if (m_n_channels != 1) {
                cerr << "Can only gaussian filter [y] grayscale images" << endl;
                exit(EXIT_FAILURE);
        }

        int kernel_radius = round(sigma);                       // same with smooth_x but work in y-direction
        int kernel_size = 2*(kernel_radius) + 1;
        int border_size = kernel_size/2;
        int x = -(kernel_size/2);
        double kernel[kernel_size];
        uchar *buffer = new uchar[2*border_size + m_height];

        //creating filter 
        double sum = 0.0;
        int t = 0;

        for (int i = x; i <= -(x); i++)
        {
                kernel[t] = exp(-(0.5)*(i*i)/(sigma*sigma));
                sum += kernel[t];
                t += 1;
        }

        for (int i = 0; i < kernel_size; i++)
        {
                kernel[i] /= sum;
        }
    
        for (int x = 0; x < m_width; x++)
        {       
                copy_column_to_buffer(buffer, m_data + x, m_height, m_step, border_size);

                for (int y = 0; y < m_height; y++)
                {       
                        int temp_sum = 0;
                        for (int i = 0; i < kernel_size; i++)
                        {
                                temp_sum += buffer[y + i]*kernel[i];
                        }
                        m_data[y*m_step + x] = uchar(temp_sum);
                }
        }
        delete [] buffer;
}

void Image::smooth(float sigma){

        smooth_x(sigma);                // this function apply both direction w.r.t sigma
        smooth_y(sigma);        
}

short *Image::deriv_x(){

        short kernel_x[] = {-1, 0, 1};                          // kernel coefficients. Separated to x and y directions
        short kernel_y[] = {1, 2, 1};
        short border_size = 1;
        short *result_array = new short[m_height*m_step];               // result array

        int lengt_buffer = max(m_height,m_width);
        uchar *buffer = new uchar[lengt_buffer + 2*border_size];

        for (int y = 0; y < m_height; y++)                                     // convolution operation in x-direction
        {       
                copy_row_to_buffer(buffer, data(y), m_width, border_size);
                int temp_sum;

                for (int x = 0; x < m_width; x++)
                {       
                        temp_sum = 0;
                                
                        temp_sum = buffer[x]*kernel_x[0] + buffer[x + 1]*kernel_x[1] + buffer[x + 2]*kernel_x[2];       //applying to buffer 
                        
                        result_array[x+y*m_width] = temp_sum;                                      // assigning output result array with type short
                        m_data[x+y*m_width] = temp_sum;                 // for seeing result in image. It's not include in the HW2. Can be COMMENTED
                }
        }

        for (int x = 0; x < m_width; x++)                                       // convolution operation in y-direction
        {       
                copy_column_to_buffer(buffer, m_data + x, m_width, m_step, border_size);                
                
                int temp_sum;
                for (int y = 0; y < m_height; y++)
                {       
                        temp_sum = 0;
                                
                        temp_sum = buffer[y]*kernel_y[0] + buffer[y + 1]*kernel_y[1] + buffer[y + 2]*kernel_y[2];
                        
                        result_array[x+y*m_width] = temp_sum;
                        m_data[x+y*m_width] = temp_sum;                 // for seeing result in image. It's not include in the HW2. Can be COMMENTED
                }
        }
        return result_array;
}

short *Image::deriv_y(){

        short kernel_x[] = {1, 2, 1};                                           //same operations like deriv_x dunction but takes derivative y
        short kernel_y[] = {-1, 0, 1};
        short border_size = 3/2;
        short *result_array = new short[m_height*m_step];

        int lengt_buffer = max(m_height,m_width);
        uchar *buffer = new uchar[lengt_buffer + 2*border_size];

        for (int y = 0; y < m_height; y++)
        {       
                copy_row_to_buffer(buffer, data(y), m_width, border_size);
                
                int temp_sum; 
                for (int x = 0; x < m_width; x++)
                {       
                        temp_sum = 0;
                                
                        temp_sum = buffer[x]*kernel_x[0] + buffer[x + 1]*kernel_x[1] + buffer[x + 2]*kernel_x[2];
                        
                        result_array[x+y*m_width] = temp_sum;
                        m_data[x+y*m_width] = temp_sum;                         // for seeing result in image. It's not include in the HW2. Can be COMMENTED
                }
        }
       
        for (int x = 0; x < m_width; x++)
        {       
                copy_column_to_buffer(buffer, m_data + x, m_width, m_step, border_size);
                
                int temp_sum;
                for (int y = 0; y < m_height; y++)
                {       
                        temp_sum = 0;
                                
                        temp_sum += buffer[y]*kernel_y[0] + buffer[y + 1]*kernel_y[1] + buffer[y + 2]*kernel_y[2];
                        
                        result_array[x+y*m_width] = temp_sum;
                        m_data[x+y*m_width] = temp_sum;                         // for seeing result in image. It's not include in the HW2. Can be COMMENTED
                }
        }

        delete [] buffer;
        return result_array;         
}

void Image::rotate(float angle, Image *out, bool bilinear_sampling, bool around_center){

        angle = angle*PI/180;

        Image *target_image;
        double new_x, new_y;
        uchar intensity;

        double center_x = out->m_width/2.0;                     // for determine the center of image
        double center_y = out->m_height/2.0;

        target_image = new Image(out->m_width,out->m_height,1,out->m_step);                   //creating a copy image 

        for (int y = 0; y < out->m_height; y++)
        {
                for (int x = 0; x < out->m_width; x++)
                {
                        
                        double xac = x;
                        double yac = y;
                        if (around_center)
                        {                                       // It works only rotate around center is true. For this purpose we choose 
                                xac -= center_x;                //reference point as center of image
                                yac -= center_y;
                        }
                        new_x = cos(angle)*xac + sin(angle)*yac;        // calculating new coordinates
                        new_y = -sin(angle)*xac + cos(angle)*yac;

                        if (around_center)
                        {       
                                new_x += m_width/2.0;            // It works only rotate around center is true.
                                new_y += m_height/2.0;
                        }
                        
                        int co_x = round(new_x);                // rounding for new pixel coordinates to integer
                        int co_y = round(new_y);

                        intensity  = out->m_data[y*m_step + x];                 // copiying intensity from source

                        if (co_x >= 0 && co_y >= 0 && co_x <= m_width && co_y <= m_height)
                        {                                                                       // It assigns if only the pixel in image range
                                target_image->m_data[co_y*out->m_step +co_x] = intensity;

                        }
                        
                }
                
        }

        if (bilinear_sampling)                                          // for bilinear sampling
        {
                for (int y = 0; y < target_image->m_height; y++)
                {
                        for (int x = 0; x < target_image->m_width; x++)
                        {
                                intensity = target_image->m_data[y*target_image->m_step + x];

                                if (intensity == 0 && x+1<target_image->m_width && y+1<target_image->m_height)
                                {

                                        double alpha = 0.5;                     // bilinear interpolation equation
                                        double beta = 0.5;                      
                                        intensity = (1 - alpha)*(1 - beta)* target_image->m_data[y*m_step + x] + 
                                                     alpha     *(1 - beta)* target_image->m_data[y*m_step + x + 1] + 
                                                    (1 - alpha)* beta     * target_image->m_data[(y + 1)*m_step + x] +
                                                     alpha     * beta     * target_image->m_data[(y + 1)*m_step + x + 1];
                                        intensity = intensity < 0 ? 0 : intensity;
                                        intensity = intensity > 255 ? 255 : intensity;

                                        target_image->m_data[x + y*target_image->m_step] = intensity;

                                }
                                
                        }
                
                }
        }
        else 
        {
                for (int y = 0; y < target_image->m_height; y++)                //if bilinear_sampling is false then do nearest neighbour sampling
                {
                        for (int x = 0; x < target_image->m_width; x++)
                        {
                                intensity = target_image->m_data[y*target_image->m_step + x];

                                if (intensity == 0 && x+1<target_image->m_width)
                                {
                                        target_image->m_data[x + y*target_image->m_step] = target_image->m_data[x+1 + y*target_image->m_step];
                                }
                                
                        }
                        
                }

        }
        
        out->m_data = target_image->m_data;                             // copying back to the source picture

        free(target_image);
}
}
