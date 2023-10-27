#ifndef FRACTALS_H
#define FRACTALS_H
 
#include <vector>
#include <cmath>

#include "ImgProc.h"  
#include "Warp.h"

namespace img
{ 
  
class ColorLUT
{
  private:
  
    double gamma;
    std::vector<float> black;
    std::vector< std::vector<float> > bands;
    
  public:
  
    ColorLUT(double gamma = 1.0);
   ~ColorLUT(){}
  
    // Generate color value from the input value
    void operator()(const double& value, std::vector<float>& C) const;
};
  
class JuliaSet : public Warp
{
  private:
  
   Point c = {0.8*cos(254.3 * 3.14159265/180.0), 0.8*sin(254.3 * 3.14159265/180.0)};
   const int nb_iterations;
   const int cycles;
  
  public:
  
   JuliaSet(const Point& P0 = Point()) 
      : nb_iterations(500), cycles(2) {}

  ~JuliaSet(){}
  
   Point operator()(const Point& P) const;
};

void ApplyFractalWarpLUT(
    ImgProc& out,
    const Point& center = {0.03811, 0.01329},
    const double range = 1.0e-6,
    const Warp& w = JuliaSet(),
    const ColorLUT& lut = ColorLUT()
); 

}
#endif
