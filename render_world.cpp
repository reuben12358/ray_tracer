#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <limits>

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    // TODO;
    double min_t = std::numeric_limits<double>::max();
    int temp = -1;
    int part = 0;

    for (int i = 0; i < objects.size(); ++i) {
        for (int j = 0; j < objects.at(i)->number_parts; ++j) {
            if ((min_t > objects.at(i)->Intersection(ray, j).dist) && 
                (objects.at(i)->Intersection(ray, j).dist >= small_t)) {
                min_t = objects.at(i)->Intersection(ray, j).dist;
                temp = i;
                part = objects.at(i)->Intersection(ray, j).part;
            }
        }
    }
    if (temp == -1) {
        return {NULL, 0, 0};
    }
    return {objects.at(temp), min_t, part};
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    // TODO; // set up the initial view ray here
    Ray ray;

    ray.endpoint = camera.position;
    ray.direction = (camera.World_Position(pixel_index) - ray.endpoint).normalized();

    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{    
    vec3 color;

    Hit obj = Closest_Intersection(ray);

    if (obj.object != NULL) {
        color = obj.object->material_shader->Shade_Surface(ray, ray.Point(obj.dist), obj.object->Normal(ray.Point(obj.dist), obj.part), recursion_depth);
    }
    else {
        vec3 empty = vec3();
        color = background_shader->Shade_Surface(ray, empty, empty, recursion_depth);
    }

    // TODO; // determine the color here

    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
