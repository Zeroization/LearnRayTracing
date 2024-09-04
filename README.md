# 基于CPU的简易离线光追渲染器

## 项目简介

本项目参考[光追三部曲](https://raytracing.github.io/)编写，使用 C++ 实现，旨在通过路径追踪技术生成逼真的图像。渲染器能够处理光的反射、折射、抗锯齿、运动模糊等复杂效果，并用多线程技术提升运行效率，最终输出PPM格式的图片。

## 项目特点

- 多线程：利用线程池对每个像素进行并行运算。
- BVH：使用BVH结构加速光线求交运算，提升程序运行效率。
- 渲染物体：支持渲染球体、由三角形组成的模型和四边形等2D图元；实现`.obj`模型的读取；简单平移，旋转变换。
- 物体材质：支持渲染漫反射材质，金属材质，介质材质，柏林噪声材质，自定义纹理材质，自发光材质（光源），恒定密度的参与介质。
- 视觉效果：支持抗锯齿，散焦模糊和球体运动模糊。

## 图片展示

![三部曲第二部](resources/images/book2.png)

![康奈尔盒](resources/images/cornell_box.png)

![渲染自定义模型](resources/images/model.png)

![三部曲第一部](resources/images/book1.png)



# 参考资料

- [Ray Tracing in One Weekend Series](https://raytracing.github.io/)
- [从0开始的光追渲染器 bilibili](https://www.bilibili.com/video/BV1MJYAeYEDk)

