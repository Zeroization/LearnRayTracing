#include "./util/vec3.hpp"
#include "./util/color.hpp"

#include <iostream>
#include <format>

int main()
{
	// 图片设定
	int imgWidth = 256;
	int imgHeight = 256;

	// 渲染
	std::cout << std::format("P3\n{} {}\n255\n", imgWidth, imgHeight);
	for (int j = 0; j < imgHeight; ++j) {
		for (int i = 0; i < imgWidth; ++i) {
			Color pixelColor = Color(static_cast<double>(i) / (imgWidth - 1),
									 static_cast<double>(j) / (imgHeight - 1),
									 0);
			writeColor(std::cout, pixelColor);
		}
	}

	return 0;
}
