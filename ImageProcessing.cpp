#include <iostream>
#include "Image.h"

using namespace std;

int main()
{
    Image test("lena.png");
    Image grayAvg = test;
    grayAvg.grayscale_avg();
    grayAvg.write("grayscale_avg.png");

    Image grayLum = test;
    grayLum.grayscale_lum();
    grayLum.write("grayscale_lum.png");

    
    

    return 0;
}
