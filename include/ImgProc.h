#ifndef IMGPROC_H
#define IMGPROC_H

#include <string>
#include <vector>

namespace img {

class ImgProc {

private:
    int width, height, channels;
    long Nsize; 
    float *image_data_ptr;  

public:

    ImgProc();    
    ~ImgProc();
    
    void clear();
    void clear(int nX, int nY, int nC);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    float* getData() const { return image_data_ptr; }
    
    void value( int i, int j, std::vector<float>& pixel) const;
    void set_value( int i, int j, const std::vector<float>& pixel);
    
    ImgProc( const ImgProc& img ); // copy constructor
    ImgProc& operator=(const ImgProc& img ); // copy assignment
 
    bool load( const std::string& filename );
    bool writeImage(const std::string& filename) const;
    
    long index(int i, int j, int c) const { return c + channels * (i + width * j); }
    long index(int i, int j) const { return i + width * j; }
    
    void gamma(float s);
    void rms_contrast();
};

} // namespace img

#endif // IMGPROC_H

