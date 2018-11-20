#include "objutils.h"

#define MAXLEN 128

Model ObjUtils::readObj(const char * objpath) {

	FILE * pfile = fopen(objpath, "rt");

	Model model;

	char str[MAXLEN];//��ȡ����

	if (pfile) {
		while (!feof(pfile)) {
			fscanf(pfile, "%s", str);
			if (strcmp(str, "v") == 0) {//����
				//ֱ�Ӷ�ȡ������Ϣ
				vec3 v3;
				fscanf(pfile, "%f%f%f", &v3.x, &v3.y, &v3.z);
				model.m_vertexes.push_back(v3);
			}
			else if (strcmp(str, "vt") == 0) {//����
				vec2 v2;
				//ֱ�Ӷ�ȡ������Ϣ
				fscanf(pfile, "%f%f%f", &v2.x, &v2.y);
				model.m_textures.push_back(v2);
			}
			else if (strcmp(str, "vn") == 0) {//����
				//ֱ�Ӷ�ȡ������Ϣ
				vec3 n3;
				fscanf(pfile, "%f%f%f", &n3.x, &n3.y, &n3.z);
				model.m_normals.push_back(n3);
			}
			else if (strcmp(str, "f") == 0) {//�� 
				//��������
				fgets(str, MAXLEN, pfile);
				vec3u v3uVertex, v3uNormal, v3uTexture;
				char * pstr = &str[0];
				for (int i = 0; i < 3; i++) {//��������
					int count = 0;//ͳ��ʶ��������ĸ���
					unsigned short index = 0;
					while ((*pstr != '\n' || *pstr != '\0') && count < 3) {
						if (*pstr >= '0' && *pstr <= '9') {//���ֿ�ʼ��������
							index *= 10;
							index += *pstr - '0';
						}
						else if (*pstr == '/' || (count == 2 && *pstr == ' ') || (count == 2 && *pstr == '\n')) {//���ַָ�
							switch (count) {
							case 0://����
								v3uVertex.index[i] = --index; break;
							case 1://����
								v3uTexture.index[i] = --index; break;
							case 2://������
								v3uNormal.index[i] = --index; break;
							default:
								break;
							}
							count++;
							index = 0;
						}	
						pstr++;
					}
				}
				model.m_vertexIndices.push_back(v3uVertex);
				model.m_textureIndices.push_back(v3uTexture);
				model.m_normalIndices.push_back(v3uNormal);
			}
			else {
				fgets(str, MAXLEN, pfile);//��ȡ�����в�������
			}
		}
		fclose(pfile);
	}
	return model;
}