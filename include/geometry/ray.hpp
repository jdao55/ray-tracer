#ifndef RAY_H
#define RAY_H


#include "vec.hpp"
#include <algorithm>
namespace geometry{

using vec3 = vec<float,3>;

class Ray
{
  public:
    vec3 orig;//origin vector
    vec3 dir;//direction vector
    float time;

    Ray(){};
    Ray (const vec3 &a, const vec3 &b, const float ti = 0.0):orig(a),dir(b),time(ti){}
    vec3 origin() const {return orig;}
    vec3 direction() const {return dir;}
    vec3 point_at_parameter(const float t) const {
        return orig +(t*dir);
    }

};

}

#endif
