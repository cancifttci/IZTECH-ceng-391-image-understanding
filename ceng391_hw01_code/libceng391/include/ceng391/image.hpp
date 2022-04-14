// CAN ÇİFTÇİ -- 270201080
#include <string>

namespace ceng391 {
typedef unsigned char uchar;

class Image
{
public:
        Image(int width, int height, int n_channels, int step=-1);
        ~Image();

        int w() const { return m_width; }
        int h() const { return m_height; }
        int step() const { return m_step; }
        int n_channels() const { return m_n_channels; }
        const uchar *data() const { return m_data; }
        uchar *      data()       { return m_data; }
        const uchar *data(int y) const { return m_data + y*m_step; }
        uchar *      data(int y)       { return m_data + y*m_step; }

        void set(uchar value) { set_rect(0, 0, m_width, m_height, value); }          // set function with object that rgba or rgb support
        void set_RGB(uchar red, uchar green, uchar blue){set_rect_RGB(0, 0, m_width, m_height, red, green, blue);}  // set function with RGB
        void set_RGBA(uchar red, uchar green, uchar blue, uchar alpha){set_rect_RGBA(0, 0, m_width, m_height, red, green, blue, 255);} // set function with RGBA

        void set_rect(int x_tl, int y_tl, int width, int height, uchar value);
        void set_rect_RGB(int x_tl, int y_tl, int width, int height, uchar red, uchar green, uchar blue);
        void set_rect_RGBA(int x_tl, int y_tl, int width, int height, uchar red, uchar green, uchar blue, uchar alpha=255);
        void xsave_pnm(const std::string& filename) const;

        Image *read_pnm(const std::string& filename);
        Image *to_rgb();
        Image *to_grayscale();
        Image *to_rgba();
private:
        int m_n_channels;
        int m_width;
        int m_height;
        int m_step;
        
        uchar *m_data;
};

}
