#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <math.h>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = color_ambient*world.ambient_color*world.ambient_intensity;
    // TODO; //determine the color

    // color = color_ambient + color_diffuse + color_specular;

    // light ray is position - intersection point

    for (unsigned i = 0; i < world.lights.size(); ++i) {
        vec3 v = world.lights.at(i) -> position - intersection_point;

        double k = dot(normal, v.normalized());

        vec3 r = 2*k*normal - v.normalized();

        double temp = (-1)*dot(ray.direction, r);

        Ray shadow = Ray(intersection_point, v.normalized());

        if ((world.enable_shadows) && 
            ((world.Closest_Intersection(shadow).dist * shadow.direction).magnitude() > small_t) && 
            ((world.Closest_Intersection(shadow).dist * shadow.direction).magnitude() < v.magnitude())) {
                continue;
        }
        else {
            if (k > 0) {
                color += color_diffuse*world.lights.at(i)->Emitted_Light(v)*k;
            }

            if (temp > 0) {
                color += color_specular*world.lights.at(i)->Emitted_Light(v)*pow(temp, specular_power);
            }
        }
    }
    return color;
}
