#include "spheregenerator.h"
#include <string>
#include <unistd.h> // for getopt

void help()
{    
    printf("           _                                   \n");
    printf(" ___ _ __ | |__   ___ _ __ ___  __ _  ___ _ __  \n");
    printf("/ __| '_ \\| '_ \\ / _ \\ '__/ _ \\/ _` |/ _ \\ '_ \\ \n");
    printf("\\__ \\ |_) | | | |  __/ | |  __/ (_| |  __/ | | |\n");
    printf("|___/ .__/|_| |_|\\___|_|  \\___|\\__, |\\___|_| |_|\n");
    printf("    |_|                        |___/            \n");
    printf("\nSpheregen is a command line tool which generates an ASCII STL file of a sphere");
    printf("\nwith the parameters you may specify. It may be useful if you need to");
    printf("\ngenerate a quick 3D model for some sort of test. Enjoy!\n");
    printf("\nUsage:\n");
    printf("-R [value]\t:\tsphere radius\t\t\t(default: 1)\n"); 
    printf("-r [value]\t:\tsphere horizontal segments\t(default: 10)\n"); 
    printf("-s [value]\t:\tsphere vertical segments\t(default: 10)\n");
    printf("-n [name]\t:\tspecify the filename\t\t(default: sphere)\n");
    printf("-l\t\t:\tuse this flag to print out logs\t(default: disabled)\n");
    printf("For any further questions and bug report go to https://github.com/Stegogo/generate-stl-sphere/issues\n\n");

}

int main(int argc, char *argv[])
{
    // Initializing sphere parameters
    int opt;
    int rings = 10;                      // RINGS
    int slices = 10;                     // SLICES
    bool logs = false;
    double radius = 1;                   // RADIUS
    std::string name = "sphere";
    std::string fileName = "";
    std::vector<double> origin{0, 0, 0}; // ORIGIN
    

    while((opt = getopt(argc, argv, "R:r:s:n:lh")) != -1) 
    { 
        switch(opt) 
        { 
            case 'l': 
                printf("Logs: enabled"); 
                logs = true;
                break; 
            case 'R':
                printf("Radius: %s\n", optarg);
                
                radius = atof(optarg);
                break;
            case 'r':
                printf("Horiontal segments: %s\n", optarg); 
                rings = atoi(optarg);
                break; 
            case 's': 
                printf("Vertical segments: %s\n", optarg);
                slices = atoi(optarg);
                break;
            case 'n':
                printf("Filename: %s\n", optarg);
                name.assign(optarg);
                break;
            case 'h':
                help();
                return 0;
                break;
            case '?': 
                printf("Unknown option: %c!!\nUse -h flag for help", optopt);
                return 0;
                break; 
        } 
    } 

    fileName = name + ".stl";

    // Calling a function to give us an ASCII STL file
    SphereGenerator sg(radius,rings, slices, origin, logs, fileName);
    sg.exec();
    return 0;
}
