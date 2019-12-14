#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[]) {
	FILE *fp1, *fp2;
	char file1[100], file2[100];
	fp1 = fopen(argv[1], "r");
	if (fp1 == NULL)
		exit(1);
	fp2 = fopen(argv[2], "r");
	if (fp2 == NULL)
		exit(1);

	int isDiff = 0;

	for( int i = 0 ; 1 ; ++i)
	{
		int c1 = fgetc(fp1);
		int c2 = fgetc(fp2);
		if (c1 == EOF || c2 == EOF)
			break;
		if (c1 != c2) {
			++isDiff;
			printf("%d ", i+1);
		}
	}
	if( isDiff == 0 )
		printf("TRUE\n");
	else
		printf("FALSE\n");
	fclose(fp1);
	fclose(fp2);
	return 0;
}
