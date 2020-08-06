//
//  main.cpp
//  MyRenderer
//
//  Created by 馒馒头 on 2020/7/29.
//  Copyright © 2020 馒馒头. All rights reserved.
//

#include <vector>
#include <iostream>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "render.h"

Model *model = NULL;
const int width = 800;
const int height = 800;

Vec3f light_dir(1, 1, 1);
Vec3f       eye(0, 0, 3);
Vec3f    center(0, 0, 0);
Vec3f        up(0, 1, 0);

struct GouraudShader : public IShader {
    Vec3f varying_intensity;

    virtual Vec4f vertex(int iface, int n) {
        Vec4f gl_Vertex = embed<4>(model->vert(iface, n)); // read vertex
        gl_Vertex = Viewport * Projection * ModelView * gl_Vertex; // transform
        varying_intensity[n] = std::max(0.f, model->normal(iface, n) * light_dir); // get diffuse lighting intensity
        return gl_Vertex;
    }

    virtual bool fragment(Vec3f bar, TGAColor &color) {
        float intensity = varying_intensity * bar; // interpolate intensity
        color = TGAColor(255, 255, 255)*intensity;
        return false;
    }
};

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("/Users/manmanto/Desktop/Pro/CG/Projects/MyRenderer/MyRenderer/obj/cow/cow.obj");
    }

    lookat(eye, center, up);
    viewport(width/8, height/8, width*3/4, height*3/4);
    projection(-1.f/(eye-center).norm());
    light_dir.normalize();

    TGAImage image(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    GouraudShader shader;
    for (int i = 0; i < model->nfaces(); i++) {
        Vec4f screen_coords[3];
        for (int j = 0; j < 3; j++) {
            screen_coords[j] = shader.vertex(i, j);
        }
        triangle(screen_coords, shader, image, zbuffer);
    }

    image.flip_vertically(); // to place the origin in the bottom left corner of the image
    zbuffer.flip_vertically();
    image.write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    delete model;
    return 0;
}
