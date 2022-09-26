#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

//==================================================
struct Vec
{
    double x, y, z;
    int ringIndex;
    int sliceIndex;
};

struct Triangle
{
    Vec A, B, C;
    Vec normal;
};

using TriangleSoup = std::vector<Triangle>;
//==================================================

std::vector<double> getNormals(Vec A, Vec B, Vec C)
{

    std::vector<double> v1, v2;
    std::vector<double> u(3);
    std::vector<double> n(3);

    // Calculate normals
    // 1) Find two vectors on a plane
    v1 = {B.x - A.x, B.y - A.y, B.z - A.z};
    v2 = {C.x - A.x, C.y - A.y, C.z - A.z};

    // 2) Find cross product of these vectors
    u[0] = (v1[1] * v2[2] - v1[2] * v2[1]);
    u[1] = (v1[2] * v2[0] - v1[0] * v2[2]);
    u[2] = (v1[0] * v2[1] - v1[1] * v2[0]);

    // 3) Turn it into a unit vector
    double unit_measure = (sqrt(pow(u[0], 2) + pow(u[1], 2) + pow(u[2], 2)));
    n[0] = (u[0] / unit_measure);
    n[1] = (u[1] / unit_measure);
    n[2] = (u[2] / unit_measure);

    return n;
}

TriangleSoup generateStl(double radius, const std::vector<double> &origin, int rings, int slices)
{
    TriangleSoup Sphere(300);
    int triangleCount = 0;

    std::ofstream out("sphere.stl");
    if (out.is_open())
    {
        out << "solid sphere"
            << "\n";
        double x, y, z;
        std::vector<Vec> points;

        //---------------------------------------------------
        //! Add the first triangle of the sphere
        //---------------------------------------------------

        // Add the first vertex
        points.push_back({0, radius, 0, 0, 0});

        for (int r = 1; r < rings; r++)
        {
            for (int s = 0; s < slices; s++)
            {

                // Calculate points
                x = origin[0] + radius * sin(M_PI * r / rings) * cos(2 * M_PI * s / slices);
                y = origin[1] + radius * sin(M_PI * r / rings) * sin(2 * M_PI * s / slices);
                z = origin[2] + radius * cos(M_PI * r / rings);

                points.push_back({-x, z, y, r, s});

                std::cout << "(" << x << " " << y << " " << z << ")" << std::endl;
                std::cout << "r = " << r << " s = " << s << std::endl;

                //? DETERMINE HOW TO PLACE TRIANGLES
                //? First triangle:
                /*
                 * A -- r 0 s 0
                 * B -- r 1 s 0
                 * C -- r 0 s 1
                 */
                if (points.size() == 3)
                {
                    std::vector<double> n;
                    n = getNormals(points[0], points[1], points[2]);

                    out << "\tfacet normal " << n[0] << " " << n[1] << " " << n[2] << "\n";
                    out << "\t\touter loop"
                        << "\n";

                    out << "\t\t\tvertex " << points[0].x << " " << points[0].y << " " << points[0].z << "\n";
                    Sphere[triangleCount].A = {points[0].x, points[0].y, points[0].z, points[0].ringIndex, points[0].sliceIndex};
                    out << "\t\t\tvertex " << points[1].x << " " << points[1].y << " " << points[1].z << "\n";
                    Sphere[triangleCount].B = {points[1].x, points[1].y, points[1].z, points[1].ringIndex, points[1].sliceIndex};
                    out << "\t\t\tvertex " << points[2].x << " " << points[2].y << " " << points[2].z << "\n";
                    Sphere[triangleCount].C = {points[2].x, points[2].y, points[2].z, points[2].ringIndex, points[2].sliceIndex};
                    triangleCount++;

                    out << "\t\tendloop"
                        << "\n";
                    out << "\tendfacet"
                        << "\n";
                }
            }
        }
        // Add the last vertex
        points.push_back({0, -radius, 0, rings, slices});

        //--------------------------------------------------------------------
        //! Add the first pole (rings = 1)
        //--------------------------------------------------------------------

        for (int s = 0; s < slices; s++)
        {
            /*
             * Constructing the first pole of the Sphere
             * Every A Vertex in the first pole should be the 
             * A -- r 0 s 0     so the next     A -- r 0 s 0 -- make all triangles connect to the pole
             * B -- r 1 s 0     triangle        B -- r 1 s 1
             * C -- r 1 s 1     should be       C -- r 1 s 2 -- we should find this one
             */

            Sphere[triangleCount].A = Sphere[triangleCount - 1].A;
            Sphere[triangleCount].B = Sphere[triangleCount - 1].C;

            // find a Vertex with r = 1 s = 1
            auto it = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                   { return vertex.ringIndex == 1 && vertex.sliceIndex == Sphere[triangleCount - 1].C.sliceIndex + 1; });

            if (it != points.end())
            {
                auto index = std::distance(points.begin(), it);
                Sphere[triangleCount].C = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};

                std::vector<double> n;
                n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

                out << "\tfacet normal " << n[0] << " " << n[1] << " " << n[2] << "\n";
                out << "\t\touter loop"
                    << "\n";

                out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
                out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << Sphere[triangleCount].B.z << "\n";
                out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

                out << "\t\tendloop"
                    << "\n";
                out << "\tendfacet"
                    << "\n";

                triangleCount++;
            }
        }
        // Last triangle of a ring------------------------------------------------------------------------------------------------
        Sphere[triangleCount].A = Sphere[0].A;
        Sphere[triangleCount].B = Sphere[0].C;
        Sphere[triangleCount].C = Sphere[0].B;

        std::vector<double> n;
        n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

        out << "\tfacet normal " << -n[0] << " " << -n[1] << " " << n[2] << "\n";
        out << "\t\touter loop"
            << "\n";

        out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
        out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << -Sphere[triangleCount].B.z << "\n";
        out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

        out << "\t\tendloop"
            << "\n";
        out << "\tendfacet"
            << "\n";

        triangleCount++;

        //--------------------------------------------------------------------
        //! Add the middle part of the sphere (1 < rings < 9)
        //--------------------------------------------------------------------

        for (int r = 1; r < rings - 1; r++)
        {
            for (int s = 0; s < slices - 1; s++)
            {
                
                // Adding first triangle

                auto it1 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                    { return vertex.ringIndex == r && vertex.sliceIndex == s; });
                if (it1 != points.end())
                {
                    auto index = std::distance(points.begin(), it1);
                    Sphere[triangleCount].A = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
                }

                auto it2 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                    { return vertex.ringIndex == r + 1 && vertex.sliceIndex == s; });
                if (it2 != points.end())
                {
                    auto index = std::distance(points.begin(), it2);
                    Sphere[triangleCount].B = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
                }
                        
                auto it3 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                {
                    return vertex.ringIndex == r && vertex.sliceIndex == s + 1;
                });

                if (it3 != points.end())
                {
                    auto index = std::distance(points.begin(), it3);
                    Sphere[triangleCount].C = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
                }

                    std::vector<double> n;
                    n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

                    out << "\tfacet normal " << n[0] << " " << n[1] << " " << n[2] << "\n";
                    out << "\t\touter loop"
                        << "\n";

                    out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << Sphere[triangleCount].B.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

                    out << "\t\tendloop"
                        << "\n";
                    out << "\tendfacet"
                        << "\n";

                    triangleCount++;


                // Adding second triangle
                Sphere[triangleCount].A = Sphere[triangleCount - 1].C;
                Sphere[triangleCount].B = Sphere[triangleCount - 1].B;
                
                auto it = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                    { return vertex.ringIndex == Sphere[triangleCount - 1].C.ringIndex  + 1 && vertex.sliceIndex == Sphere[triangleCount - 1].C.sliceIndex; });

                if (it != points.end())
                {
                    auto index = std::distance(points.begin(), it);
                    Sphere[triangleCount].C = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};

                    std::vector<double> n;
                    n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

                    out << "\tfacet normal " << n[0] << " " << n[1] << " " << n[2] << "\n";
                    out << "\t\touter loop" << "\n";

                    out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << Sphere[triangleCount].B.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

                    out << "\t\tendloop" << "\n";
                    out << "\tendfacet" << "\n";

                    triangleCount++;
                }
            }
        }

        //Last sequence----------------------------------------------------------------------------------

        for (int r = 1; r < rings - 1; r++)
        {
                
                // Adding first triangle

                auto it1 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                    { return vertex.ringIndex == r && vertex.sliceIndex == slices -1 ; });
                if (it1 != points.end())
                {
                    auto index = std::distance(points.begin(), it1);
                    Sphere[triangleCount].A = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
                }

                auto it2 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                    { return vertex.ringIndex == r + 1 && vertex.sliceIndex == slices - 1; });
                if (it2 != points.end())
                {
                    auto index = std::distance(points.begin(), it2);
                    Sphere[triangleCount].B = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
                }
                        
                auto it3 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                {
                    return vertex.ringIndex == r  && vertex.sliceIndex == 0;
                });

                if (it3 != points.end())
                {
                    auto index = std::distance(points.begin(), it3);
                    Sphere[triangleCount].C = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
                }

                    std::vector<double> n;
                    n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

                    out << "\tfacet normal " << n[0] << " " << n[1] << " " << n[2] << "\n";
                    out << "\t\touter loop"
                        << "\n";

                    out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << Sphere[triangleCount].B.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

                    out << "\t\tendloop"
                        << "\n";
                    out << "\tendfacet"
                        << "\n";

                    triangleCount++;


                // Adding second triangle
                Sphere[triangleCount].A = Sphere[triangleCount - 1].C;
                Sphere[triangleCount].B = Sphere[triangleCount - 1].B;
                
                auto it = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                    { return vertex.ringIndex == Sphere[triangleCount - 1].C.ringIndex + 1 && vertex.sliceIndex == 0; });

                if (it != points.end())
                {
                    auto index = std::distance(points.begin(), it);
                    Sphere[triangleCount].C = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};

                    std::vector<double> n;
                    n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

                    out << "\tfacet normal " << n[0] << " " << n[1] << " " << n[2] << "\n";
                    out << "\t\touter loop" << "\n";

                    out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << Sphere[triangleCount].B.z << "\n";
                    out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

                    out << "\t\tendloop" << "\n";
                    out << "\tendfacet" << "\n";

                    triangleCount++;
                }
            }

        //--------------------------------------------------------------------
        //! Add the last pole (rings = rings)
        //--------------------------------------------------------------------

        for (int s = 0; s < slices - 1; s++)
        {
            Sphere[triangleCount].A = {0, -radius, 0, rings, slices};
            
            auto it1 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                    { return vertex.ringIndex == rings - 1 && vertex.sliceIndex == s; });
                if (it1 != points.end())
                {
                    auto index = std::distance(points.begin(), it1);
                    Sphere[triangleCount].C = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
                }

            auto it = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
                                   { return vertex.ringIndex == rings - 1 && vertex.sliceIndex == s+1; });

            if (it != points.end())
            {
                auto index = std::distance(points.begin(), it);
                Sphere[triangleCount].B = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
            }
                std::vector<double> n;
                n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

                out << "\tfacet normal " << n[0] << " " << n[1] << " " << n[2] << "\n";
                out << "\t\touter loop"
                    << "\n";

                out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
                out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << Sphere[triangleCount].B.z << "\n";
                out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

                out << "\t\tendloop"
                    << "\n";
                out << "\tendfacet"
                    << "\n";

                triangleCount++;

        }

        // Last triangle of a ring------------------------------------------------------------------------------------------------
        Sphere[triangleCount].A = {0, -radius, 0, rings, slices};
        
        
        auto it2 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
        {
            return vertex.ringIndex == rings - 1 && vertex.sliceIndex == slices - 1;
        });
        if (it2 != points.end())
        {
            auto index = std::distance(points.begin(), it2);
            Sphere[triangleCount].C = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
        }
                        
        auto it3 = std::find_if(points.begin(), points.end(), [&](const Vec vertex)
        {
            return vertex.ringIndex == rings - 1 && vertex.sliceIndex == 0;
        });
        if (it3 != points.end())
        {
            auto index = std::distance(points.begin(), it3);
            Sphere[triangleCount].B = {points[index].x, points[index].y, points[index].z, points[index].ringIndex, points[index].sliceIndex};
        }

        //std::vector<double> n;
        n = getNormals(Sphere[triangleCount].A, Sphere[triangleCount].B, Sphere[triangleCount].C);

        out << "\tfacet normal " << -n[0] << " " << -n[1] << " " << n[2] << "\n";
        out << "\t\touter loop"
            << "\n";

        out << "\t\t\tvertex " << Sphere[triangleCount].A.x << " " << Sphere[triangleCount].A.y << " " << Sphere[triangleCount].A.z << "\n";
        out << "\t\t\tvertex " << Sphere[triangleCount].B.x << " " << Sphere[triangleCount].B.y << " " << -Sphere[triangleCount].B.z << "\n";
        out << "\t\t\tvertex " << Sphere[triangleCount].C.x << " " << Sphere[triangleCount].C.y << " " << Sphere[triangleCount].C.z << "\n";

        out << "\t\tendloop"
            << "\n";
        out << "\tendfacet"
            << "\n";

        triangleCount++;

    
        out << "endsolid sphere" << "\n";
        out.close();
    }
    return Sphere;
}

int main()
{
    // Initializing sphere parameters
    double radius = 1.0;                 // RADIUS
    std::vector<double> origin{0, 0, 0}; // ORIGIN
    int rings = 10;                      // RINGS
    int slices = 10;                     // SLICES

    // Calling a function to give us an ASCII STL file
    generateStl(radius, origin, rings, slices);
    return 0;
}