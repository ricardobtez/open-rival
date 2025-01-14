#include "pch.h"

#include <filesystem>
#include <iostream>

#include "setup-utils.h"
#include "texture-builder.h"

using namespace Rival::Setup;

int main(int argc, char* argv[])
{
    // Expected:
    // texture-builder.exe DEFINITIONS_DIR

    // Check arg count
    if (argc < 2)
    {
        std::cout << "No directory name provided\n";
        return -1;
    }
    else if (argc > 2)
    {
        std::cout << "Too many arguments\n";
        return -1;
    }

    // Read parameters
    bool atlasMode = false;
    std::string definitionsDir;
    definitionsDir = argv[1];

    // Create the "textures" directory
    if (!createDirectory("textures"))
    {
        std::cout << "Could not create \"textures\" directory\n";
        return -1;
    }

    buildTextures(definitionsDir, "images", "textures");
}
