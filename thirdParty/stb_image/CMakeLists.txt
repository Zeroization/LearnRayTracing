cmake_minimum_required(VERSION 3.27)

# stb_image
add_library(stb_image STATIC "stbi/stb_image.cpp")
target_include_directories(stb_image PUBLIC .)
target_compile_features(stb_image PUBLIC cxx_std_20)