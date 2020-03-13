#ifndef RAY_H
#define RAY_H


#include "vec.hpp"
#include <algorithm>
namespace geometry{

using vec3 = vec<float,3>;

class Ray
{
  public:
    vec3 A;//origin vector
    vec3 B;//direction vector
    float time;

    Ray(){};
    Ray (const vec3 &a, const vec3 &b, const float ti = 0.0):A(a),B(b),time(ti){}
    vec3 origin() const {return A;}
    vec3 direction() const {return B;}
    vec3 point_at_parameter(const float t) const {
        return A +(t*B);
    }

};

}

#endif
