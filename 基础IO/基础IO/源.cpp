#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>



int main () {
	FILE* fp = fopen("test.txt", "w");
	if (!fp){
		perror("fopen");
		return -1;
	}
	printf("fopen success\n");
	const char* str = "hahahaha";
	//����ֵ:�ɹ�д��ĸ���
	fwrite(str, 1, 8, fp);

	char str2[1024] = { 0 };
	//ע�ⷵ��ֵ�Ͳ���
	//
	//����ֵ�Ƕ����ĸ���
	size_t s = fread(str2, 1, 8, fp);
	str2[s] = 0;
	printf("%s\n",str2);

	fclose(fp);
	fp = nullptr;
	return 0;
}