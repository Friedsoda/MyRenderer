#include <vector>
#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int width  = 800;
const int height = 800;
Model *model = NULL;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    // height > width
    bool steep = false;
    if(std::abs(x1 - x0) < std::abs(y1 - y0)){
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    // left to right
    if(x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (float x = x0; x < x1; x++) {
        float t = (x - x0)/(float)(x1 - x0);
        int y = y0 + t * (y1 - y0);
        if(steep){
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    model = new Model("/Users/manmanto/Desktop/Pro/CG/Projects/MyRenderer/MyRenderer/obj/african_head.obj");
	TGAImage image(width, height, TGAImage::RGB);
    
    // Wireframe
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x + 1.) * width/2.;
            int y0 = (v0.y + 1.) * height/2.;
            int x1 = (v1.x + 1.) * width/2.;
            int y1 = (v1.y + 1.) * height/2.;
            line(x0, y0, x1, y1, image, white);
            std::cout << "Draw" << std::endl;
        }
    }
    
	image.flip_vertically(); // origin at the left bottom corner
    image.write_tga_file("output.tga");
    delete model;
	return 0;
}

