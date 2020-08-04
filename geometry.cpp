//
//  geometry.cpp
//  MyRenderer
//
//  Created by 馒馒头 on 2020/8/4.
//  Copyright © 2020 馒馒头. All rights reserved.
//

#include "geometry.h"

template <> template <> vec<3,int>  ::vec(const vec<3,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)),z(int(v.z+.5f)) {}
template <> template <> vec<3,float>::vec(const vec<3,int> &v)   : x(v.x),y(v.y),z(v.z) {}
template <> template <> vec<2,int>  ::vec(const vec<2,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)) {}
template <> template <> vec<2,float>::vec(const vec<2,int> &v)   : x(v.x),y(v.y) {}
