
// CAN ÇİFTÇİ - 270201080
// CENG 391 - HW2


#include <string>

namespace ceng391 {
typedef unsigned char uchar;

class Image
{
public:
        ~Image();

        static Image* new_grayscale(int width, int height, int step=-1) { return new Image(width, height, 1, step); }
        static Image* new_rgb      (int width, int height, int step=-1) { return new Image(width, height, 3, step); }
        static Image* new_rgba     (int width, int height, int step=-1) { return new Image(width, height, 4, step); }
        static Image* from_png(const std::string& filename, bool load_as_gray)
                { Image *img = new Image(10, 10, 1); img->xload_png(filename, load_as_gray); return img; }

        int w() const { return m_width; }
        int h() const { return m_height; }
        int n_ch() const { return m_n_channels; }
        int step() const { return m_step; }

        const uchar *data() const { return m_data; }
        uchar *      data()       { return m_data; }
        const uchar *data(int y) const { return m_data + y*m_step; }
        uchar *      data(int y)       { return m_data + y*m_step; }

        void set(uchar value) { set_rect(0, 0, m_width, m_height, value); }
        void set_rect(int x_tl, int y_tl, int width, int height, int value)
                { set_rect_rgb(x_tl, y_tl, width, height, value, value, value); }
        void set_rect_rgb(int x_tl, int y_tl, int width, int height,
                          uchar red, uchar green, uchar blue)
                { set_rect_rgba(x_tl, y_tl, width, height, red, green, blue, 255); }
        void set_rect_rgba(int x_tl, int y_tl, int width, int height,
                           uchar red, uchar green, uchar blue, uchar alpha);

        void xsave_pnm(const std::string& filename) const;
        void xsave_png(const std::string &filename) const;
        void xload_png(const std::string &filename, bool load_as_grayscale);

        void box_filter_x(int filter_size);
        void box_filter_y(int filter_size);
        void box_filter(int filter_size);
        void smooth_x(float sigma);
        void smooth_y(float sigma);
        void smooth(float sigma);
        short *deriv_x();
        short *deriv_y();
        void rotate(float angle, Image *out, bool bilinear_sampling, bool around_center);
private:
        Image(int width, int height, int n_channels, int step=-1);
        void reallocate(int width, int height, int n_channels);

        int m_width;
        int m_height;
        int m_n_channels;
        int m_step;

        uchar *m_data;
};

}
