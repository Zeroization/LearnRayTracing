# 基于CPU的简易离线光追渲染器

## 项目简介

本项目参考[光追三部曲](https://raytracing.github.io/)编写，使用 C++ 实现，旨在通过路径追踪技术生成逼真的图像。渲染器能够处理光的反射、折射、抗锯齿、运动模糊等复杂效果，并用多线程技术提升运行效率，最终输出PPM格式的图片。

## 项目特点

- 多线程：利用线程池对每个像素进行并行运算。
- 渲染物体：支持渲染球体和三角形组成的模型，实现`.obj`模型的读取。
- 物体材质：支持渲染漫反射材质，金属材质（全反射），介质材质（反射+折射）物体。
- 视觉效果：支持抗锯齿，散焦模糊和球体运动模糊。

## 图片展示

![三部曲第一部](resources/images/book1.png)

![渲染自定义模型](resources/images/model.png)



# 参考资料

- [Ray Tracing in One Weekend Series](https://raytracing.github.io/)
- [从0开始的光追渲染器 bilibili](https://www.bilibili.com/video/BV1MJYAeYEDk)

