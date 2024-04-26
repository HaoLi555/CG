#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.

    SceneParser sp(inputFile.c_str());
    Image image(sp.getCamera()->getWidth(), sp.getCamera()->getHeight());
    for (int x = 0; x < sp.getCamera()->getWidth(); ++x)
    {
        for (int y = 0; y < sp.getCamera()->getHeight(); ++y)
        {
            Ray cam_ray = sp.getCamera()->generateRay(Vector2f(x, y));
            assert(cam_ray.getDirection().length() == 1.0f); // make sure the ray direction is normalized
            Group *base_group = sp.getGroup();
            Hit hit;
            if (base_group->intersect(cam_ray, hit, 0.0f))
            {
                Vector3f final_color = Vector3f::ZERO;
                for (int li = 0; li < sp.getNumLights(); ++li)
                {
                    Light *light = sp.getLight(li);
                    Vector3f L, light_color;
                    light->getIllumination(cam_ray.pointAtParameter(hit.getT()), L, light_color);
                    final_color += hit.getMaterial()->Shade(cam_ray, hit, L, light_color);
                }
                image.SetPixel(x, y, final_color);
            }
            else
            {
                image.SetPixel(x, y, sp.getBackgroundColor());
            }
        }
    }

    image.SaveImage(outputFile.c_str());
    cout << "Hello! Computer Graphics!" << endl;
    return 0;
}

