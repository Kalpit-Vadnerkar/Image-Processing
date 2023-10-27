#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "ImgProc.h"
  
namespace img
{

class Stencil{
    public:
        Stencil(int halfwidth = 5);
        ~Stencil();
        
        int halfwidth() const { return half_width; }
        int size() const { return width; }
        void setValue(int i, float value) { stencil_values[i] = value; }
        float& operator()(int i, int j);
        const float& operator()(int i, int j) const;
    private:
        int half_width;
        int width;
        float *stencil_values;
};

void BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );

Stencil makeRandomFilter();

}

#endif
