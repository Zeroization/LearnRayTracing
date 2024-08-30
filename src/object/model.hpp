#pragma once

#include "triangle.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

class Model : public Hittable
{
public:
	Model(const std::vector<Triangle>& triangles, shared_ptr<Material> material)
		: triangles(triangles), material(material)
	{
		build_aabb();
	}
	Model(const std::filesystem::path& filename, shared_ptr<Material> material)
		: material(material)
	{
		// obj格式文件
		// 顶点:		v	1 2 3
		// 法向量:	vn	1 2 3
		// 三角形:	f	p0//n0 p1//n1 p2//n2
		std::vector<Point3> positions;	
		std::vector<Vec3> normals;		 

		std::ifstream file(filename);
		if (!file.good())
		{
			std::cout << "打开文件 " << filename << " 失败!\n";
			return;
		}

		std::string line;
		char trash;
		while (!file.eof())
		{
			std::getline(file, line);
			std::istringstream iss(line);

			// 读取顶点
			if (line.compare(0, 2, "v ") == 0)
			{
				Point3 pos;
				iss >> trash >> pos.e[0] >> pos.e[1] >> pos.e[2];
				positions.push_back(pos);
			}
			// 读取法向量
			else if (line.compare(0, 3, "vn ") == 0)
			{
				Vec3 normal;
				iss >> trash >> trash >> normal.e[0] >> normal.e[1] >> normal.e[2];
				normals.push_back(normal);
			}
			// 读取三角形
			else if (line.compare(0, 2, "f ") == 0)
			{
				int p0, p1, p2, n0, n1, n2;
				iss >> trash;
				iss >> p0 >> trash >> trash >> n0;
				iss >> p1 >> trash >> trash >> n1;
				iss >> p2 >> trash >> trash >> n2;

				triangles.push_back(Triangle(
					positions[p0 - 1], positions[p1 - 1], positions[p2 - 1],
					normals[n0 - 1], normals[n1 - 1], normals[n2 - 1],
					material
				));
			}
		}
		build_aabb();
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		bool isHit = false;
		for (const auto& triangle : triangles)
		{
			HitRecord tri_rec;
			if (triangle.hit(r, ray_t, tri_rec))
			{
				isHit = true;
				ray_t.max = tri_rec.t;
				rec = tri_rec;
			}
		}

		return isHit;
	}

	AABB bounding_box() const override { return bbox; }

private:
	AABB bbox;
	std::vector<Triangle> triangles;
	shared_ptr<Material> material;

	// 构建包围盒
	void build_aabb()
	{
		for (const auto& triangle : triangles)
		{
			bbox = AABB(bbox, triangle.bounding_box());
		}
	}
};