#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>



int main () {
	FILE* fp = fopen("test.text", "w");
	if (!fp){
		perror("fopen");
		return -1;
	}
	printf("fopen success\n");
	const char* str = "hahahaha";
	fwrite(str, 1, sizeof(str), fp);



	fclose(fp);
	fp = nullptr;




	return 0;
}