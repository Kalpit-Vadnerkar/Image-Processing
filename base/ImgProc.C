#include "ImgProc.h"
#include <OpenImageIO/imageio.h>
#include <iostream> 
#include <cmath> 

OIIO_NAMESPACE_USING

namespace img {

ImgProc::ImgProc() 
    : width(0), height(0), channels(0), Nsize(0), image_data_ptr(nullptr) 
{}

ImgProc::~ImgProc()
{
 clear();
}

void ImgProc::clear(){
    if( image_data_ptr != 0 ){ 
        delete[] image_data_ptr; 
        image_data_ptr = 0; 
    }
    width = 0;
    height = 0;
    channels = 0;
    Nsize = 0;
}

void ImgProc::clear(int w, int h, int ch) {
    width = w;
    height = h;
    channels = ch;
    Nsize = (long)w * (long)h * (long)ch;

    image_data_ptr = new float[Nsize];
    
#pragma omp parallel for 
    for(long i = 0; i < Nsize; i++) { image_data_ptr[i] = 0.0; }
}

bool ImgProc::load(const std::string& filename) {
    std::unique_ptr<ImageInput> in = ImageInput::open(filename);
    if (!in) {
        std::cerr << "Failed to open image " << filename << "\n";
        return false;
    }

    const ImageSpec &spec = in->spec();
    width = spec.width;
    height = spec.height;
    channels = spec.nchannels;

    Nsize = (long)width * (long)height * (long)channels;
    image_data_ptr = new float[Nsize];

    in->read_image(TypeDesc::FLOAT, image_data_ptr);
    in->close();

    return true;
}

bool ImgProc::writeImage(const std::string& filename) const {
    if (!image_data_ptr) {  
        std::cerr << "Image data not available." << std::endl;
        return false;
    }

    std::unique_ptr<ImageOutput> out = ImageOutput::create(filename);
    if (!out) return false;

    ImageSpec spec(width, height, channels, TypeDesc::FLOAT);
    out->open(filename, spec);
    out->write_image(TypeDesc::FLOAT, getData());
    out->close();

    return true;
}

ImgProc::ImgProc(const ImgProc& v) :
    width(v.width), height(v.height), channels(v.channels), Nsize(v.Nsize) 
{
    image_data_ptr = new float[Nsize];
#pragma omp parallel for
    for( long i=0;i<Nsize;i++){ image_data_ptr[i] = v.image_data_ptr[i]; }
}

ImgProc& ImgProc::operator=(const ImgProc& v)
{
    if (this == &v) { 
        return *this; 
    }
    clear();
    width = v.width;
    height = v.height;
    channels = v.channels;
    Nsize = v.Nsize;   
    image_data_ptr = new float[Nsize];

#pragma omp parallel for
    for( long i=0;i<Nsize;i++){ image_data_ptr[i] = v.image_data_ptr[i]; }
    return *this;
}

void ImgProc::value( int i, int j, std::vector<float>& pixel) const
{
    pixel.clear();
    if( image_data_ptr == nullptr ){ return; }
    if( i<0 || i>=width ){ return; }
    if( j<0 || j>=height ){ return; }
    pixel.resize(channels);
    for( int c=0;c<channels;c++ ) { pixel[c] = image_data_ptr[index(i,j,c)]; }
    return;
}

void ImgProc::set_value( int i, int j, const std::vector<float>& pixel)
{
    if( image_data_ptr == nullptr ){ return; }
    if( i<0 || i>=width ){ return; }
    if( j<0 || j>=height ){ return; }
    if( channels > (int)pixel.size() ){ return; }
#pragma omp parallel for
    for( int c=0;c<channels;c++ ){ image_data_ptr[index(i,j,c)] = pixel[c]; }
    return;
}

void ImgProc::gamma(float s) 
{
    #pragma omp parallel for
    for(long i = 0; i < Nsize; i++) 
    {
        image_data_ptr[i] = std::pow(image_data_ptr[i], s);
    }
}

} // namespace img

