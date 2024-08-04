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
			auto r = static_cast<double>(i) / (imgWidth - 1);
			auto g = static_cast<double>(j) / (imgHeight - 1);
			auto b = 0.0;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			std::cout << std::format("{} {} {}\n", ir, ig, ib);
		}
	}

	return 0;
}
