#include "noiseutils.h"
#include <stdlib.h>
#include <cmath>

float NoiseUtils::genericGaussianNoise(float mu, float sigma) {
	static float v1, v2, s;
	static int phase = 0;
	float x;
	float u1, u2;
	if (phase == 0) {
		do {
			u1 = (float)rand() / RAND_MAX;
			u2 = (float)rand() / RAND_MAX;

			v1 = 2 * u1 - 1;
			v2 = 2 * u2 - 1;
			s = v1 * v1 + v2 * v2;
		} while (s >= 1 || s == 0);

		x = v1 * sqrt(-2 * log(s) / s);
	}
	else {
		x = v2 * sqrt(-2 * log(s) / s);
	}
	phase = 1 - phase;
	return mu + sigma * x;
}


NoiseUtils::NoiseUtils() :
	m_persistence(0.5), m_numberOfOctaves(4) {};


NoiseUtils::NoiseUtils(float persistence, int numberOfOctaves) :
	m_persistence(persistence), m_numberOfOctaves(numberOfOctaves) {};


float NoiseUtils::Noise(int x, int y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n*(n*n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float NoiseUtils::SmoothedNoise(int x, int y) {
	float corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	float sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	float center = Noise(x, y) / 4;
	return corners + sides + center;
}

float NoiseUtils::CosineInterpolate(float a, float b, float x) {
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5;
	return a * (1 - x) + b * f;
}

float NoiseUtils::LinearInterpolate(float a, float b, float x) {
	return a * (1 - x) + b * x;
}

float NoiseUtils::InterpolateNoise(float x, float y) {
	int int_x = int(x);
	float fraction_x = x - int_x;
	int int_y = int(y);
	float fraction_y = y - int_y;
	float v1 = SmoothedNoise(int_x, int_y);
	float v2 = SmoothedNoise(int_x + 1, int_y);
	float v3 = SmoothedNoise(int_x, int_y + 1);
	float v4 = SmoothedNoise(int_x + 1, int_y + 1);
	float i1 = CosineInterpolate(v1, v2, fraction_x);
	float i2 = CosineInterpolate(v3, v4, fraction_x);
	return CosineInterpolate(i1, i2, fraction_y);
}

float NoiseUtils::PerlinNoise(float x, float y) {
	float noise = 0;
	for (int i = 0; i < m_numberOfOctaves; i++) {
		float frequency = pow(2, i);
		float amplitude = pow(m_persistence, i);
		noise = noise + InterpolateNoise(x * frequency, y*frequency)*amplitude;
	}
	return noise;
}

