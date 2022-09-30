#include "spheregenerator.h"

int main()
{
    // Initializing sphere parameters
    double radius = 1.5;                 // RADIUS
    std::vector<double> origin{0, 0, 0}; // ORIGIN
    int rings = 100;                      // RINGS
    int slices = 100;                     // SLICES

    // Calling a function to give us an ASCII STL file
    SphereGenerator sg(radius,rings, slices, origin);
    sg.generateStl();
    return 0;
}