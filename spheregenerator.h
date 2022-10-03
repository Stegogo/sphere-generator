#include <vector>
#include <string>
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

    bool logsEnabled;

    void print(TriangleSoup Sphere);

    std::vector<double> origin;
    std::vector<double> getNormals(Vec A, Vec B, Vec C);
    std::string fileName;

    TriangleSoup generateStl();

public:
    SphereGenerator();
    SphereGenerator(double radius, int rings, int slices, std::vector<double> origin, bool logsEnabled, std::string fileName);
    ~SphereGenerator();

    void exec();
};