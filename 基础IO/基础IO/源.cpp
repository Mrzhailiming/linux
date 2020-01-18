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
	//返回值:成功写入的个数
	fwrite(str, 1, 8, fp);

	char str2[1024] = { 0 };
	//注意返回值和参数
	//
	//返回值是读到的个数
	size_t s = fread(str2, 1, 8, fp);
	str2[s] = 0;
	printf("%s\n",str2);

	fclose(fp);
	fp = nullptr;
	return 0;
}