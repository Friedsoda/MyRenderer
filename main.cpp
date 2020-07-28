#include <vector>
#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    // height > width
    bool steep = false;
    if (std::abs(x1 - x0) < std::abs(y1 - y0)){
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    // left to right
    if (x0 > x1){
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

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    if (t0.y == t1.y && t1.y == t2.y) return;
    
    // sort vertexes according to y value
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    
    // divide the triangle into two parts and draw
    int total_height = t2.y - t0.y;
    for (int y = t0.y; y < t2.y; y++){
        bool above_segment = y > t1.y || t0.y == t1.y;
        int segment_height = above_segment? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)(y - t0.y) / total_height; // t0-t2 side
        float beta = (float)(y - (above_segment? t1.y : t0.y)) / segment_height; // t0-t1/t1-t2 side
        Vec2i vec_a = t0 + (t2 - t0) * alpha;
        Vec2i vec_b = above_segment? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        // draw each pixel at height y
        if (vec_a.x > vec_b.x) std::swap(vec_a, vec_b);
        for (int x = vec_a.x; x <= vec_b.x; x++){
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("/Users/manmanto/Desktop/Pro/CG/Projects/MyRenderer/MyRenderer/obj/african_head.obj");
    }
    
    TGAImage image(width, height, TGAImage::RGB);
    Vec3f light_dir(0, 0, -1);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
            world_coords[j]  = v;
        }
        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0) {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }
    
	image.flip_vertically(); // origin at the left bottom corner
    image.write_tga_file("output.tga");
    delete model;
	return 0;
}

