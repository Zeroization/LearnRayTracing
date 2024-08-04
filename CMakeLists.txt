cmake_minimum_required (VERSION 3.27)

project (
	LearnRayTracing
	LANGUAGES CXX C
	DESCRIPTION "学习光线追踪!"
	VERSION 0.0.1
)

file(GLOB_RECURSE SOURCE_CODE "src/*.cpp" "src/*.h" "src/*.hpp")

add_executable (LearnRayTracing "${SOURCE_CODE}")

target_compile_features(LearnRayTracing PUBLIC cxx_std_20)
