#include <iostream>
#include "Image.h"

using namespace std;

int main()
{
    Image test("lena.png");
    Image copy = test;
    
    cout << copy.w << " " << copy.h << " " << copy.channels << endl;

    cout << "Brightness: " << endl;
    float brightness = 1;
    cin >> brightness;
    for (int i = 0; i < copy.w * copy.h * copy.channels; i++) {
        copy.data[i] = (int)(copy.data[i]* brightness) % 256;
    }
        copy.write("copy.png");
    return 0;
}
