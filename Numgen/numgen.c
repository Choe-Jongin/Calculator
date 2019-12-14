#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT 10000
#define filename argv[1]
#define wholestring argv[2]
#define loopstring argv[3]

int StringToInt(char arg[]) // xxx string -> xxx number
{
	int result = 0;
	int temp = 1;
	int i = 0, j = 0;
	int len = strlen(arg);

	for(; i<len; ++i)
	{
		for(j = i+1; j<len; ++j)
			temp *= 10;
		
		result += (int)(arg[i] - '0') * temp;
		temp = 1;
	}
	return result;
}
char Oper1()
{
	return '+';
}
char Oper2()
{
	return '-';
}
char Oper3()
{
	return '*';
}
char Oper4()
{
	switch( rand() % 3)
	{
	case 0: return '+'; 
	case 1: return '-';
	case 2: return '*';
	}
	return '+';
}
int main(int argc, char* argv[])
{
	FILE * file = NULL;
	int wholenumber = 0; // inner loop number
	int loopnumber = 0; // outer loop number
	int spacenum = 0; // just for input
	int mode = 0; // just for input
	int filenamelen = 0;
	char c = EOF;
	int wholelen = 0;
	int looplen = 0;
	// Max input length of each argument is MAX_INPUT
	int oper_input = -1;
	
	char (*Oper)();	//function pointer

	srand(time(NULL)); // seed initialize
	
	printf("파일 이름 각 숫자의 자릿수 반복횟수를 입력해주세요\n예시 : filename.txt 100 10\n");
	
	wholenumber =  StringToInt(wholestring);
	loopnumber  = StringToInt(loopstring);
	
	while(oper_input != 1 && oper_input != 2 && oper_input != 3 && oper_input != 4)
	{
		printf("Choose operators between numbers\n1 : +만 넣기, 2 : -만 넣기, 3 :*만 넣기, 4 : +-*를 무작위로 넣기\n");
		scanf("%d", &oper_input);
	}
	switch( oper_input )
	{
		case 1: Oper = Oper1; break;
		case 2: Oper = Oper2; break;
		case 3: Oper = Oper3; break;
		case 4: Oper = Oper4; break;
	}

	file = fopen(filename, "w");

	// file writing step
	int i = 0, j = 0, randomnum = 0;	
	char * string = malloc(sizeof(char) * (MAX_INPUT + 1));
	for(i = 0; i < loopnumber; ++i)
	{
		// write number
		int isdec = 0;
		j = 0;
		string[j] = '1'+rand()%9;
		for(j = 1; j < wholenumber; ++j)
		{
			randomnum = rand() % 10;
			string[j] = (char)(randomnum + '0');

			if( isdec == 0 && j < wholenumber -1 && rand()%(wholenumber) == 0 )
			{
				string[j] = (char)('.');
				isdec = 1;
			}
		}
		string[j] = '\0';
		fprintf(file, "%s", string);
		if(i < loopnumber -1)
			fprintf(file,"%c",Oper());
	}
	
	printf("File writing complete. Open \"%s\"\n", filename);
	
	fclose(file);
	free(string);
	return 0;
}

