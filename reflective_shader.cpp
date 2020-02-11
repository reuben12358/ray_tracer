#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    TODO; // determine the color

    vec3 shader_surface = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    
    Ray reflected = Ray(intersection_point, ray.direction - 2*dot(ray.direction, normal)*normal);

    color = shader_surface*(1 - reflectivity);
    // make sure recursion doesn't go out of control
    if (recursion_depth++ >= world.recursion_depth_limit) {
        return color;
    }
    else {
        color += reflectivity * world.Cast_Ray(reflected, recursion_depth++);
    }
    return color;

    // Ray reflected = Ray(intersection_point, ray.direction - (2*dot(ray.direction, normal)*normal));

    // vec3 view_color = shader -> Shade_Surface(ray, intersection_point, normal, recursion_depth);
    // vec3 reflected_color = world.Cast_Ray(reflected, recursion_depth++);

    // color = (1 - reflectivity)* view_color + reflectivity* reflected_color;
}
