#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    TODO;
    double k;

    // if (!(Intersect_Triangle(ray, part, k))) {
    //     return {nullptr, 0, 0};
    // }

    if (part <= 0) {
        // return {this, k, part};
        for (unsigned i = 0; i < triangles.size(); ++i) {
            if (Intersect_Triangle(ray, i, k)) {
                return {this, k, i};
            }
        }
    }
    else {
        for (unsigned i = 0; i < triangles.size(); ++i) {
            if (Intersect_Triangle(ray, i, k)) {
                return {this, k, part};
            }
        }
    }
    return {nullptr, 0, 0};
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
    // TODO;
    vec3 A = vertices.at(triangles.at(part)[0]);
    vec3 B = vertices.at(triangles.at(part)[1]);
    vec3 C = vertices.at(triangles.at(part)[2]);

    return (cross((B-A),(C-A))).normalized();
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    TODO;
    vec3 A = vertices.at(triangles.at(tri)[0]);
    vec3 B = vertices.at(triangles.at(tri)[1]);
    vec3 C = vertices.at(triangles.at(tri)[2]);
    vec3 n = Normal(vec3(), tri);

    double k = dot(n, ray.direction);

    if (k == 0) {
        return false;
    }
    else {
        k = dot(A - ray.endpoint, n) / k;
    }

    vec3 P = ray.endpoint + k*ray.direction;

    double a = dot(cross(C - B, P - B), n)/2;
    if (a < - weight_tol) {
        return false;
    }
    double b = dot(cross(A - C, P - C), n)/2;
    if (b < - weight_tol) {
        return false;
    }
    double c = dot(cross(B - A, P - A), n)/2;
    if (c < - weight_tol) {
        return false;
    }

    if (a > 1.001 || b > 1.001 || c > 1.001 || b + c > 1.001 || a + b > 1.001 || a + c > 1.001) {
        return false;
    }

    P = a*A + b*B + c*C;

    dist = (P - ray.endpoint).magnitude();

    return true;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
