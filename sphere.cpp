#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    double k = dot(ray.direction, ray.direction);
    if (k < small_t) {
        return {NULL,0,0};
    }

    vec3 diff = ray.endpoint - center;
    double square_rt = dot(ray.direction, diff)*dot(ray.direction
                       ,diff) - k*(dot(diff,diff)- radius*radius);

    if (square_rt < 0) {
        return {NULL,0,0};
    }
    else if (square_rt == 0) {
        if (dot(ray.direction, diff)/k <= small_t) {
            return {NULL,0,0};
        }
        else {
            return {this, dot(ray.direction, diff)/k, part};
        }
    }
    else {// 2 intersects, choose lower
        double k1 = ((-1)*dot(ray.direction, diff) + sqrt(square_rt));
        double k2 = ((-1)*dot(ray.direction, diff) - sqrt(square_rt));
        // if (k1 > small_t) {
        //     if (k2 > small_t) {
        //         if (k1 > k2) {
        //             return {this, k2, part};
        //         }
        //         else {
        //             return {this, k1, part};
        //         }
        //     }
        //     else {
        //         return {this, k1, part};
        //     }
        // }
        // else {
        //     if (k2 > small_t) {
        //         return {this, k2, part};
        //     }
        //     else {
        //         return {NULL,0,0};
        //     }
        // }
        if ((k1 > small_t) && (k2 > small_t)) {
            if (k1 > k2) {
                return {this, k2, part};
            }
            else {
                return {this, k1, part};
            }
        }
        else if ((k1 < small_t) && (k2 < small_t)) {
            return {NULL, 0, 0};
        }
        else if (k1 < small_t) {
            return {this, k2, part};
        }
        else {
            return {this, k1, part};
        }
    }
    // double t = (-1)*dot(ray.direction, diff) + sqrt();

    return {NULL,0,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    normal = (point - center).normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
