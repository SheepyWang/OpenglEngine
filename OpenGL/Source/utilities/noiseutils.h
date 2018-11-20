#pragma once

#ifndef _NOISE_UTILS_H_
#define _NOISE_UTILS_H_

class NoiseUtils {
private:
	float m_persistence;
	int m_numberOfOctaves;
public:
	static float genericGaussianNoise(float mu, float sigma);
	NoiseUtils();
	NoiseUtils(float persistence, int numberOfOctaves);
	float Noise(int x, int y);
	float SmoothedNoise(int x, int y);
	float CosineInterpolate(float a, float b, float x);
	float LinearInterpolate(float a, float b, float x);
	float InterpolateNoise(float x, float y);
	float PerlinNoise(float x, float y);
	
};

#endif // !_NOISE_UTILS_H_
