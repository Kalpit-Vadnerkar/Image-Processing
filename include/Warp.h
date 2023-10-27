#ifndef WARP_H
#define WARP_H
  
namespace img
{
  
typedef double fractal_t;
//typedef long double fractal_t;
struct Point
{
   fractal_t x;
   fractal_t y;
};
  
  
class Warp
{
  public:
  
   Warp(){}
   virtual ~Warp(){}
  
   virtual Point operator()(const Point& P) const = 0;
};  

}
#endif
