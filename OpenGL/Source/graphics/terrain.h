#pragma once
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <vector>
#include "../maths/maths_func.h"
#include "../utilities/Perlin.h"


const vec3 LIGHT_GREEN = vec3(0.8, 1.0, 0.0);
const vec3 DEEP_GREEN = vec3(0.6, 0.8, 0.2);
const vec3 LAND = vec3(0.8, 0.4, 0.2);
const vec3 ICE = vec3(0.8, 1.0, 1.0);

class Terrain {
private:
	int m_mapWidth;
	int m_mapHeight;
	int m_size;

	float m_k;
	float m_gap;

	Perlin m_perlin;

	std::vector <vec3> m_map;
	std::vector <vec3> m_normals;
	std::vector <vec3> m_vertexes;
	std::vector <vec3> m_colors;

public:
	Terrain();
	Terrain(const int & mapWidth, const int & mapHeight);

	void Init(int octaves, int persistence);	//��ʼ������Ϣ
	void GetColor(float height);				//���ݸ߶�������ɫ
	void Generate();							//����Ƭ����㷨��

	void DataTrasnform(std::vector <vec3> & vertexes, std::vector <vec3> & normals, std::vector <vec3> & colors );

	inline void size();
};

#endif