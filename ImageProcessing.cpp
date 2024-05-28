#include <iostream>
#include "Image.h"

using namespace std;

int main()
{
    Image test("lena.png");
    /*Image grayAvg = test;
    grayAvg.grayscale_avg();
    grayAvg.write("Images/grayscale_avg.png");

    Image grayLum = test;
    grayLum.grayscale_lum();
    grayLum.write("Images/grayscale_lum.png");*/
    
    
    Image colorM = test;
    colorM.colorMask(0.7,0.9,0.8);
    colorM.write("Images/colorMix.png");
    
    /*Image brightM = test;
    brightM.brightness_map(80);
    brightM.write("Images/brightMapLighter.png");*/

    /*Image satMap = test;
    satMap.saturation_mask();
    satMap.write("Images/saturation.png");*/

    return 0;
}
