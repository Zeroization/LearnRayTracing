#pragma once

#include "../rtweekend.h"

#include <fstream>
#include <filesystem>
#include <vector>

// 相机的胶片, 负责存储整个视口
class Film
{
public:
	Film(size_t width, size_t height) : width(width), height(height)
	{
		pixels.resize(width * height);
	}

	// 存储PPM P3格式
	void save(const std::filesystem::path& filename)
	{
		std::ofstream file(filename, std::ios::binary);
		file << "P6\n" << width << ' ' << height << "\n255\n";

		for (size_t y = 0; y < height; ++y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				const Color& color = getPixel(x, y);
				writeColor(file, color);
			}
		}
	}

	// Getter & Setters
	size_t getWidth() const { return width; }
	size_t getHeight() const { return height; }
	Color getPixel(size_t x, size_t y) const { return pixels[y * width + x]; }
	void setPixel(size_t x, size_t y, const Color& color) { pixels[y * width + x] = color; }

private:
	size_t width, height;
	std::vector<Color> pixels;
};
