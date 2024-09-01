#pragma once

// 屏蔽MSVC编译器警告
#ifdef _MSC_VER
#pragma warning (push, 0)
#endif // _MSC_VER

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stbi/stb_image.h"

#include <cstdlib>
#include <iostream>

class RTWImage
{
public:
	RTWImage() {}
	RTWImage(const char* img_filepath)
	{
		auto filepath = std::string(img_filepath);
		if (load(filepath))	return;

		std::cerr << "ERROR: 加载图片文件 " << filepath << " 失败!\n";
	}

	~RTWImage()
	{
		delete[] bdata;
		STBI_FREE(fdata);
	}

	bool load(const std::string& filename)
	{
		auto n = bytes_per_pixel;
		fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
		if (fdata == nullptr)
		{
			return false;
		}

		bytes_per_scanline = image_width * bytes_per_pixel;
		convert_to_bytes();
		return true;
	}

	int width() const { return (fdata == nullptr) ? 0 : image_width; }
	int height() const { return (fdata == nullptr) ? 0 : image_height; }

	const unsigned char* pixel_data(int x, int y) const
	{
		static unsigned char magenta[] = {255, 0, 255};
		if (bdata == nullptr)
		{
			return magenta;
		}

		x = clamp(x, 0, image_width);
		y = clamp(y, 0, image_height);
		return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
	}

private:
	const int bytes_per_pixel = 3;
	float* fdata = nullptr;			// 线性浮点数像素数据
	unsigned char* bdata = nullptr;	// 线性8位像素数据
	int image_width = 0;
	int image_height = 0;
	int bytes_per_scanline = 0;

	// 返回将x限定至[low, high)的结果
	static int clamp(int x, int low, int high)
	{
		if (x < low)	return low;
		if (x < high)	return x;
		return high - 1;
	}

	// 将位于[0, 1]的颜色转换为[0, 255]
	static unsigned char float_to_byte(float value)
	{
		if (value <= 0.0)	return 0;
		if (1.0 <= value)	return 255;
		return static_cast<unsigned char>(256.0 * value);
	}
	
	// 将线性浮点像素数据转换为8位的
	void convert_to_bytes()
	{
		int total_bytes = image_width * image_height * bytes_per_pixel;
		bdata = new unsigned char[total_bytes];

		auto* bptr = bdata;
		auto* fptr = fdata;
		for (auto i = 0; i < total_bytes; ++i, ++fptr, ++bptr)
		{
			*bptr = float_to_byte(*fptr);
		}
	}
};

// 取消屏蔽MSVC警告
#if _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
