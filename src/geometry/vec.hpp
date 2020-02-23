#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <array>
#include <cstddef>
#include <iostream>
#include <math.h>
namespace geometry
{

    template <typename T, size_t n = 3>
    class vec
    {
    public:
        std::array<T,n> e;
        constexpr const vec& operator+(){return *this;}
        constexpr const vec& operator-(){return vec{-e[0],-e[1],-e[2]};}
        constexpr T& operator[](size_t i) {return e[i];}
        constexpr T operator[](size_t i) const {return e[i];}

        constexpr vec& operator+=(const vec &v2)
        {
            for(size_t i=0; i<n; i++)
            {
                e[i]+=v2.e[i];
            }
            return *this;
        }

        constexpr vec& operator-=(const vec &v2)
        {
            for(size_t i =0; i<n; i++)
            {
                e[i]-=v2.e[i];
            }
            return *this;
        }

        constexpr vec& operator*=(const vec &v2)
        {
            for(size_t i=0; i<n;i++)
            {
                e[i]*=v2.e[i];
            }
            return *this;
        }

        constexpr vec& operator/=(const vec &v2)
        {
            for(size_t i=0; i<n;i++)
            {
                e[i]/=v2.e[i];
            }
            return *this;
        }

        constexpr vec& operator*=(const T &t2)
        {
            for(size_t i=0; i<n;i++)
            {
                e[i]*=t2;
            }
            return *this;
        }

        constexpr vec& operator/=(const T &t2)
        {
            for(size_t i=0; i<n;i++)
            {
                e[i]/=t2;
            }
            return *this;
        }

        constexpr T length() const
        {
            T sum=0;
            for(size_t i=0; i<n;i++)
            {
                sum+=e[i]*e[i];
            }
            return sqrt(sum);
        }

        constexpr T square_len() const
        {
            T sum=0;
            for(size_t i=0; i<n;i++)
            {
                sum+=e[i]*e[i];
            }
            return sum;
        }

        inline std::istream& operator>>(std::istream &is)
        {
            for(size_t i=0; i<n; i++)
            {
                is >> e[i];
            }
            return is;
        }

        inline std::ostream& operator<<(std::ostream &os)
        {
            for(size_t i=0; i<n; i++)
            {
                os << e[i]<<" ";
            };
            return os;
        }

        constexpr void make_unit_vector()
        {
            T k = 1.0 / sqrt(square_len());
            for(size_t i=0; i<n; i++)
            {
                e[i] *= k;
            }
        }

        inline vec unit_vector() const
        {
            return *this/length();
        }

        constexpr vec operator+(const vec &v2) const
        {
            std::array<T,n> temp{};
            for(size_t i=0; i<n; i++)
            {
                temp[i] = e[i] + v2.e[i];
            }
            return vec<T,n>{temp};
        }

        constexpr vec operator-(const vec &v2) const
        {
            std::array<T,n> temp{};
            for(size_t i=0; i<n; i++)
            {
                temp[i] = e[i] - v2.e[i];
            }
            return vec<T,n>{temp};
        }

        constexpr vec operator*(const vec &v2) const
        {
            std::array<T,n> temp{};
            for(size_t i=0; i<n; i++)
            {
                temp[i] = e[i] * v2.e[i];
            }
            return vec<T,n>{temp};
        }

        constexpr vec operator/(const vec &v2) const
        {
            std::array<T,n> temp{0.0};
            for(size_t i=0; i<n; i++)
            {
                temp[i] = e[i] / v2.e[i];
            }
            return vec<T,n>{temp};
        }

        constexpr vec operator*(const T t2) const
        {
            std::array<T,n> temp{};
            for(size_t i=0; i<n; i++)
            {
                temp[i] = e[i] * t2;
            }
            return vec<T,n>{temp};
        }

        constexpr vec operator/(const T t2) const
        {
            std::array<T,n> temp{};
            for(size_t i=0; i<n; i++)
            {
                temp[i] = e[i] / t2;
            }
            return vec{temp};
        }


    };
    template<typename T, size_t c>
    constexpr vec<T,c> operator*(float n, const vec<T,c> &v)
    {
        std::array<T,c> temp{};
        for(size_t i=0; i<c; i++)
        {
            temp[i] = v[i] * n;
        }
        return vec<T,c>{temp};
    }


    template<typename T, size_t c>
    constexpr auto dot(const vec<T,c> &v1, const vec<T,c> &v2)
    {
        T dot_result= (v1.e[0] * v2.e[0]);
        for(size_t i=1; i<c; i++)
        {
            dot_result += (v1.e[i] * v2.e[i]);
        }
        return dot_result;
    }

    template<typename T>
    constexpr vec<T,3> cross(const vec<T,3> &v1, const vec<T,3> &v2)
    {
        return vec<T,3> { (v1[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
                v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0] };
    }

    template<typename T, size_t n>
    T length(const vec<T,n> &v)
    {
        T res = 0;
        for(size_t i=1; i<n; i++)
        {
            res += v[i]*v[i];
        }
        return sqrt(res);
    }

     template<typename T, size_t n>
     T length(const vec<T,n> &v1, const vec<T,n> &v2)
    {
        return length(v1 - v2);
    }
}

#endif
