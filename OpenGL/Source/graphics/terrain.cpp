#include "terrain.h"

Terrain::Terrain() {
	m_mapHeight = 100;
	m_mapWidth = 100;
	m_size = 0;
	m_perlin = Perlin(255);
	m_k = 16;
	m_gap = 0.05;

}

Terrain::Terrain(const int & mapWidth, const int & mapHeight) {
	m_mapHeight = mapHeight;
	m_mapWidth = mapWidth;
	m_size = 0;
	m_perlin = Perlin(255);
	m_k = 16;
	m_gap = 0.05;
}

void Terrain::Init(int octaves, int persistence) {

	for (int x = 0; x < m_mapWidth; x++) {
		for (int z = 0; z < m_mapHeight; z++) {
			m_map.push_back(vec3(x - m_mapWidth / 2, m_k * m_perlin.octavePerlin(x * m_gap, 0, z * m_gap, 32, 1 / 32), z - m_mapHeight / 2));
		}
	}
}

void Terrain::GetColor(float height) {
	float level = m_k / 4;
	vec3 nowColor;
	if (height < level) {
		nowColor = LAND;
	}
	else if (height < 2 * level) {
		nowColor = LIGHT_GREEN;
	}
	else if (height < 3 * level) {
		nowColor = DEEP_GREEN;
	}
	else {
		nowColor = ICE;
	}
	for (int i = 0; i < 3; i++) {
		m_colors.push_back(nowColor);
	}
}

void Terrain::Generate() {
	int startPos = 0;
	for (int i = 0; i < m_mapHeight - 1; i++) {//行
		int nowStartPos = startPos;
		int lineDownPos = i * m_mapWidth;
		int lineUpPos = (i + 1) * m_mapWidth;
		int nextPos[2] = { lineDownPos + 1,lineUpPos + 1 };
		for (int j = 0; j < 2 * (m_mapWidth - 1); j++) {

			int thirdPos = nextPos[nowStartPos];

			m_vertexes.push_back(m_map[lineDownPos]);
			m_vertexes.push_back(m_map[lineUpPos]);
			m_vertexes.push_back(m_map[thirdPos]);

			float avg_y = (m_map[lineDownPos].y + m_map[lineUpPos].y + m_map[thirdPos].y) / 3;
			GetColor(avg_y);

			vec3 norml = vec3::cross(m_map[lineDownPos] - m_map[thirdPos], m_map[lineDownPos] - m_map[lineUpPos]);
			for (int i = 0; i < 3; i++) {
				m_normals.push_back(norml);
			}
			if (nowStartPos == 0) {//下顶点移动
				lineDownPos++;
			}
			else {//上顶点移动 整个nextPos移动
				lineUpPos++;
				nextPos[0] = lineDownPos + 1;
				nextPos[1] = lineUpPos + 1;
			}
			nowStartPos = 1 - nowStartPos;
		}
	}
}

void Terrain::DataTrasnform(std::vector<vec3> & vertexes, std::vector<vec3> & normals, std::vector<vec3> & colors) {
	for (int i = 0; i < m_vertexes.size(); i++) {
		vertexes.push_back(m_vertexes[i]);
		normals.push_back(m_normals[i]);
		colors.push_back(m_colors[i]);
	}
}

