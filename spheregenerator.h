#include <vector>

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

class SphereGenerator
{
private:
    double radius;
    int rings;
    int slices;
    int triangleCount;

    void print(TriangleSoup Sphere);

    std::vector<double> origin;
    std::vector<double> getNormals(Vec A, Vec B, Vec C);

    TriangleSoup generateStl();

public:
    SphereGenerator();
    SphereGenerator(double radius, int rings, int slices, std::vector<double> origin);
    ~SphereGenerator();

    void exec();
};