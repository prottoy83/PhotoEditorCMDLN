#pragma once
#include <stdint.h>
#include <cstdio>


enum ImageType {
	PNG, JPG, BMP, TGA
};

struct Image {
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;

	Image(const char* filename, int channel_force = 0);
	Image(int w, int h, int channels = 3);
	Image(const Image& img);
	~Image();

	bool read(const char* filename, int channel_force = 0);
	bool write(const char* filename);

	ImageType get_file_type(const char* filename);

	Image& grayscale_avg();
	Image& grayscale_lum();

	Image& colorMask(float r,float g, float b);

	Image& brightness_map(float val);
	Image& saturation_mask();

	Image& diffmap(Image& img);
};