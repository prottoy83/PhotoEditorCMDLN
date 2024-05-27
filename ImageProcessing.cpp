#include <iostream>
#include "Image.h"

using namespace std;

int main()
{
    Image test("lena.png");
    Image grayAvg = test;
    grayAvg.grayscale_avg();
    grayAvg.write("Images/grayscale_avg.png");

    Image grayLum = test;
    grayLum.grayscale_lum();
    grayLum.write("Images/grayscale_lum.png");
    

    Image colorM = test;
    colorM.colorMask(0.8,1,1);
    colorM.write("Images/colorMix.png");
    
    

    return 0;
}
