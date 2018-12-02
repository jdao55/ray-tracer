#ifndef RAY_H
#define RAY_H


#include "vec.hpp"
#include <algorithm>
namespace geometry{

using vec3 = vec<float,3>;

class Ray
{
  public:
    vec3 A;
    vec3 B;
    float time;

    Ray(){};
    Ray (const vec3 &a, const vec3 &b, const float ti = 0.0):A(a),B(b),time(ti){}
    inline vec3 origin() const {return A;}
    inline vec3 direction() const {return B;}
    inline vec3 point_at_parameter(float t) const {
        return A +(t*B);
    }

};

}

#endif
