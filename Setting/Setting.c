#include <stdio.h>

int main( int argc, char * argv[])
{
	if( argc <= 2 )
	{
		printf("잘못된 인자\n");
		return 0;			
	}
	int llint = sizeof(long long int);
	unsigned int NS = 1;
	int i = 0;

	while( argv[1][++i] == '0' )
	{
		NS *= 10;
	}

	if( argv[1][i] > '0' && argv[1][i] <= '9' || argv[1][0] != '1' ||  NS < 10 )
	{
		printf("10^n진수만 가능합니다.(n>0\n");
		return 0;
	}
	if( NS > 1000000000 )
	{
		printf("최대 10억 진수까지 가능합니다");
		return 0;
	}
	if( NS > 10000 )
	{
		if( llint != 8 )
		{
			printf("8바이트 변수를 지원하지 않습니다.\n");
			printf("10000진수로 컴파일 합니다.\n");
			NS = 10000;
		}
	}

	int ns = NS -1;
	int nslen = 1;
	while( ns/10 > 0)
	{
		ns/=10;
		++nslen;
	}
	FILE * fp = fopen("Setting/Setting.h","w");

	fprintf(fp, "#define NS %d\n",NS);
	fprintf(fp, "#define NSLEN %d\n", nslen);
	fprintf(fp, "#define DATA int\n");
	if(llint == 8 ) 
		fprintf(fp, "#define LONGDATA unsigned long long int\n");
	else
		fprintf(fp, "#define LONGDATA unsigned int\n");

	fclose(fp);
	printf("설정된 진수 %d, 한 자리 최대값 %d\n", NS, nslen);

	system("gcc *.c -o inf_calc\n");

	return 0;
}
