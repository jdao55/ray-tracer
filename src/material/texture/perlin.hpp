#ifndef PERLIN_H
#define PERLIN_H

#include "../../geometry/vec.hpp"
#include <random>
#include <array>

//TODO Refactor this too modern c++
using vec3 = geometry::vec<float,3>;

inline float perlin_interp(const vec3 c[2][2][2], const float u, const float v, const float w) {
    const float uu = u*u*(3-2*u);
    const float vv = v*v*(3-2*v);
    const float ww = w*w*(3-2*w);
    float accum = 0;
    for (int i=0; i < 2; i++)
        for (int j=0; j < 2; j++)
            for (int k=0; k < 2; k++) {
                const vec3 weight_v{u-i, v-j, w-k};
                accum += (i*uu + (1-i)*(1-uu))*
                    (j*vv + (1-j)*(1-vv))*
                    (k*ww + (1-k)*(1-ww))*
                         (c[i][j][k].dot(weight_v));
            }
    return accum;
}

class perlin {
    public:
        float noise(const vec3& p) const {
            float u = p[0] - floor(p[0]);
            float v = p[1] - floor(p[1]);
            float w = p[2] - floor(p[2]);
            int i = floor(p[0]);
            int j = floor(p[1]);
            int k = floor(p[2]);
            vec3 c[2][2][2];
            for (int di=0; di < 2; di++)
                for (int dj=0; dj < 2; dj++)
                    for (int dk=0; dk < 2; dk++)
                        c[di][dj][dk] = ranvec[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
            return perlin_interp(c, u, v, w);
        }
        float turb(const vec3& p, int depth=7) const {
            float accum = 0;
            vec3 temp_p = p;
            float weight = 1.0;
            for (int i = 0; i < depth; i++) {
                accum += weight*noise(temp_p);
                weight *= 0.5;
                temp_p *= 2;
            }
            return fabs(accum);
        }
        static vec3 *ranvec;
        static int *perm_x;
        static int *perm_y;
        static int *perm_z;

    ~perlin(){
        delete(ranvec);
        delete(perm_x);
        delete(perm_y);
        delete(perm_z);
    }
};

static vec3* perlin_generate() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> rand_float(0.0,1.0);
    vec3 * p = new vec3[256];
    for ( int i = 0; i < 256; ++i )
        p[i] = vec3{-1 + 2*rand_float(e2), -1 + 2*rand_float(e2), -1 + 2*rand_float(e2)};
    return p;
}

void permute(int *p, int n) {
     std::random_device rd;
     std::mt19937 e2(rd());
     std::uniform_real_distribution<float> rand_float(0.0,1.0);
     for (int i = n-1; i > 0; i--) {
        int target = int(rand_float(e2)*(i+1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
    return;
}

static int* perlin_generate_perm() {
    int * p = new int[256];
    for (int i = 0; i < 256; i++)
        p[i] = i;
    permute(p, 256);
    return p;
}

vec3 *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();



#endif /* PERLIN_H */
