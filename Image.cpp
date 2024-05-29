#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)

#include "Dependencies/stb_image.h"
#include "Dependencies/stb_image_write.h"
#include "Image.h"
#include <iostream>
#include <thread>
#include <chrono>

using std::chrono::high_resolution_clock;

using std::chrono::duration;
using std::chrono::milliseconds;

Image::Image(const char* filename, int channel_force) {
	if (read(filename, channel_force)) {
		printf("Read %s\n", filename);
		size = w * h * channels;
	}
	else {
		printf("Failed to read %s\n", filename);
	}
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
	size = w * h * channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
	memcpy(data, img.data, size);
}

Image::~Image() {
	stbi_image_free(data);
}

bool Image::read(const char* filename, int channel_force) {
	data = stbi_load(filename, &w, &h, &channels, channel_force);
	channels = channel_force == 0 ? channels : channel_force;
	return data != NULL;
}

bool Image::write(const char* filename) {
	ImageType type = get_file_type(filename);
	int success;
	switch (type) {
		case PNG:
			success = stbi_write_png(filename, w, h, channels, data, w*channels);
			break;
		case BMP:
			success = stbi_write_bmp(filename, w, h, channels, data);
			break;
		case JPG:
			success = stbi_write_jpg(filename, w, h, channels, data, 100);
			break;
		case TGA:
			success = stbi_write_tga(filename, w, h, channels, data);
			break;
	}

	return success != 0;
}

ImageType Image::get_file_type(const char* filename) {
	const char* ext = strrchr(filename, '.');
	if (ext != nullptr) {
		if (strcmp(ext, ".png") == 0) {
			return PNG;
		}
		else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
			return JPG;
		}
		else if (strcmp(ext, ".bmp") == 0) {
			return BMP;
		}
		else if (strcmp(ext, ".tga") == 0) {
			return TGA;
		}
	}
	else {
		return PNG;
	}
}

Image& Image::grayscale_avg() {
	if (channels < 3) {

	}
	else {
		for (int i = 0; i < size; i += channels) {
			int gray = (data[i] + data[i + 1] + data[i + 2])/3;
			memset(data + i, gray, 3);
		}
	}
	return *this;
}
Image& Image::grayscale_lum() {
	if (channels < 3) {

	}
	else {
		for (int i = 0; i < size; i += channels) {
			int gray = 0.3*data[i]+ 0.59*data[i+1] + 0.11*data[i+2];
			memset(data + i, gray, 3);
		}
	}
	return *this;
}

void colorMaskMultiplier(uint8_t* data, int start, int size, int chan, float val) {
	for (int i = start; i < size; i += chan) {
		data[i] *= val;
	}
}

Image& Image::colorMask(float r, float g, float b) {
	if (channels < 3) {

	}
	else {
		std::thread rMask(colorMaskMultiplier, data, 0, size, channels, r);
		std::thread gMask(colorMaskMultiplier, data, 1, size, channels, g);
		std::thread bMask(colorMaskMultiplier, data, 2, size, channels, b);

		rMask.join();
		gMask.join();
		bMask.join();
	}
	return *this;
}

/*Image& Image::brightness_map(float val) {
    std::chrono::steady_clock::time_point t1, t2;
	using std::chrono::duration_cast;
	if (channels < 3) {

	}
	else {
		t1 = high_resolution_clock::now();
		for (int i = 0; i < size; i += channels) {
			if (val > 0) {
				data[i] = fmin(data[i] + val, 255);
				data[i + 1] = fmin(data[i + 1] + val, 255);
				data[i + 2] = fmin(data[i + 2] + val, 255);
			}
			else {
				data[i] = fmax(data[i] + val, 1);
				data[i + 1] = fmax(data[i + 1] + val, 1);
				data[i + 2] = fmax(data[i + 2] + val, 1);
			}
		}
		t2 = high_resolution_clock::now();
	}

	duration<double, std::milli> ms_double = t2 - t1;

	std::cout << ms_double.count() << "ms\n";

	return *this;
}*/

void mChannelBright(uint8_t* data, float val, int stp, int sz, int chn) {
	if (val > 0) {
		for (int i = stp; i < sz; i += chn) {
			data[i] = fmin(data[i] + val, 255);
		}
	}
	else {
		for (int i = stp; i < sz; i += chn) {
			data[i] = fmax(data[i] + val, 1);
		}
	}
}

Image& Image::brightness_map(float val) {
	std::chrono::steady_clock::time_point t1, t2;
	using std::chrono::duration_cast;
	if (channels < 3) {

	}
	else {
		t1 = high_resolution_clock::now();
		std::thread rChannel(mChannelBright, data, val, 0, size, channels);
		std::thread gChannel(mChannelBright, data, val, 1, size, channels);
		std::thread bChannel(mChannelBright, data, val, 2, size, channels);

		rChannel.join();
		gChannel.join();
		bChannel.join();
		t2 = high_resolution_clock::now();
	}
	
	duration<double, std::milli> ms_double = t2 - t1;

	std::cout << ms_double.count() << "ms\n";
	return *this;
}

Image& Image::saturation_mask() {
	if (channels < 3) {

	}
	else {
		for (int i = 0; i < size; i += channels) {
			int gray = fmax(1.6 * data[i],255) + fmax(1.59 * data[i + 1],255) + fmax(1.11 * data[i + 2],255);
			memset(data + i, gray, 3);
		}
	}
	return *this;
}

Image& Image::diffmap(Image& img) {
	int compare_width = fmin(w,img.w);
	int compare_height = fmin(h,img.h);
	int compare_channels = fmin(channels,img.channels);
	for(uint32_t i=0; i<compare_height; ++i) {
		for(uint32_t j=0; j<compare_width; ++j) {
			for(uint8_t k=0; k<compare_channels; ++k) {
				data[(i*w+j)*channels+k] = BYTE_BOUND(abs(data[(i*w+j)*channels+k] - img.data[(i*img.w+j)*img.channels+k]));
			}
		}
	}
	return *this;
}