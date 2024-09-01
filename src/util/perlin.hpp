#pragma once

class Perlin
{
public:
	Perlin() 
	{
		for (int i = 0; i < point_count; ++i)
		{
			rand_vec[i] = unitVector(Vec3::random(-1, 1));
		}

		perlin_generate_perm(perm_x);
		perlin_generate_perm(perm_y);
		perlin_generate_perm(perm_z);
	}

	double noise(const Point3& p) const
	{
		double u = p.x() - std::floor(p.x());
		double v = p.y() - std::floor(p.y());
		double w = p.z() - std::floor(p.z());

		int i = static_cast<int>(std::floor(p.x()));
		int j = static_cast<int>(std::floor(p.y()));
		int k = static_cast<int>(std::floor(p.z()));
		Vec3 c[2][2][2];

		for (int di = 0; di < 2; ++di)
		{
			for (int dj = 0; dj < 2; ++dj)
			{
				for (int dk = 0; dk < 2; ++dk)
				{
					c[di][dj][dk] = rand_vec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
				}
			}
		}

		return perlin_interp(c, u, v, w);
	}

	double turb(const Point3& p, int depth) const
	{
		double accum = 0.0;
		Point3 tmp_p = p;
		double weight = 1.0;

		for (int i = 0; i < depth; ++i)
		{
			accum += weight * noise(tmp_p);
			weight *= 0.5;
			tmp_p *= 2;
		}

		return std::fabs(accum);
	}

private:
	static const int point_count = 256;
	Vec3 rand_vec[point_count];
	int perm_x[point_count];
	int perm_y[point_count];
	int perm_z[point_count];

	static void perlin_generate_perm(int* p)
	{
		for (int i = 0; i < point_count; ++i)
		{
			p[i] = i;
		}
		permute(p, point_count);
	}

	static void permute(int* p, int n)
	{
		for (int i = n - 1; i > 0; --i)
		{
			int target = random_int(0, i);
			std::swap(p[i], p[target]);
		}
	}

	static double perlin_interp(const Vec3 c[2][2][2], double u, double v, double w)
	{
		double uu = u * u * (3 - 2 * u);
		double vv = v * v * (3 - 2 * v);
		double ww = w * w * (3 - 2 * w);
		double accum = 0.0;

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < 2; ++k)
				{
					Vec3 weight_v(u - i, v - j, w - k);
					accum += (i * uu + (1 - i) * (1 - uu))
						* (j * vv + (1 - j) * (1 - vv))
						* (k * ww + (1 - k) * (1 - ww))
						* dot(c[i][j][k], weight_v);
				}
			}
		}

		return accum;
	}
};
