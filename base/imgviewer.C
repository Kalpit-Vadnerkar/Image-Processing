#include <iostream>
#include <memory>
#include <cstring>
#include "ImgProc.h"
#include "StarterViewer.h"

int main(int argc, char **argv) {
    std::string imageFilename;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-image") == 0) {
            imageFilename = argv[i + 1];
            break;
        }
    }
    
    img::ImgProc image;
    
    if (!image.load(imageFilename)) {
        std::cerr << "Error: Failed to load image." << std::endl;
        return 1;
    }
    
    starter::StarterViewer* viewer = starter::CreateViewer();
    std::vector<std::string> args(argv, argv + argc);
    viewer->Init(args);

    viewer->SetImageData(&image);
    viewer->MainLoop();
    
    return 0;
}

