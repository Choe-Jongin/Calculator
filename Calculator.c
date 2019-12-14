#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "NumList.h"

int IsThis(char a, const char * list);	//a가 list중 하나인지
int IsInt(char a);			//a가 '0'~'9' 이면 1을 반환
int OperOrder( char ch1, char ch2 );	//연산자 우선순위 비교 a가 높거나 같으면 1
int  ReIn();				//입력이 잘못 되었을때 다시 입력 할 것인지 의사를 물음 0: 없음, 1: 다시입력
Num * StackCal( char * str );		//수식 문자열을 계산
float GetSec( struct timeval t1, struct timeval t2 );	//두 시간 사이 흐른 초를 반환
int main(int argc, char *argv[])
{
	int ns = NS-1;
	int NSlen = 1;
	while( ns >= 10 )
	{
		ns /= 10;
		++NSlen;
	}
	if( NSLEN != NSlen )
	{
		printf(" List.h의 NSLEN을 재 설정 하고 컴파일 하십시오\n");
		return 0;
	}
	struct timeval startProtime;
       	gettimeofday(&startProtime,NULL);
	struct timeval startCaltime, nowtime;
	List inString;		//입력된 문자열을 담을 변수
	InitList(&inString);

	//수식 문자열 입력
	unsigned int in = 0 ;		//키보드 버퍼에서 한 글자씩 빼온 값을 담을 변수
	unsigned int wrongcount = 0;	//잘못된 문자(ex:alphabet)나 위치해선 안될 위치에 존재하는 문자(ex:++,--)의 수
	unsigned int modicount = 0;	//수식이 성립되게 임의로 추가 한 글자의 수
	unsigned int inputlength = 0;	//총 입력된 문자열의 길이
	unsigned int temp = 0;		//이전에 입력했던 문자를 담는 변수
	unsigned int dotcount = 0;	//이번 항에 '.'의 개수(1이 넘으면 안됨)
	int bracket = 0;		//괄호의 개수를 세어 유효성 검사를 하는 변수(음수이면 에러)
	char *_pstring = NULL;		//링크드 리스트 문자열을 배열로 바꿔줌
	
	int force = 0;
	int inputmode = 0;		// 0:keyboard, 1:file
	int (*readchar)(FILE *);	// 함수 포인터
	readchar = &getchar;		// 사실 함수 포인터를 안쓰고 fgetc(infile)만 써도 되지만 배운김에..

        FILE * infile = stdin;
	if( argc == 2 || argc == 3 || argc == 4)
        {
        	infile = fopen( argv[1], "r" );
                if( infile != NULL ) 
		{
			inputmode = 1;
			readchar = &fgetc;
				
			if( argc == 4 && argv[3][0] == 'f')	// 파일 입력시 강제 입력($ 실행파일명 수식파일 결과저장파일 f )
			       force = 1;	
		}
        	else
			printf("파일명이 옳지 않습니다.");
	}

	//메인루프.. 같겠지만 사실 아니고 재 입력시 반복 그 외 경우엔 반복하지 않고 탈출
	while(1)
	{
		if( inputmode == 0 )
			printf("수식을 입력 하세요\n->");
		__fpurge( stdin );
		ClearList(&inString);	//입력 문자열 리스트의 값을 전부 제거해줌
		InitList(&inString);	//입력 문자열 리스트를 사용할 수 있게 초기화
		SetListName(&inString,"inString");
		in = wrongcount = modicount = inputlength = temp = bracket = dotcount = 0;	//입력에 관련된 변수 초기화
	/*
		in = readchar(infile);	//첫 문자 인식
		++inputlength;		//입력받은 문자열 수 하나 증가
		if( in == '-' )					//첫 글자가 '-'로 시작하면 앞에 0을 붙여줌				
		{*
			Push_backNode(&inString,'0');
			Push_backNode(&inString,'-');
			temp = '-';
			++modicount;
		
		}
		else if( in == '*' || in == '+')		// 첫 글자가 '*' or '+'로 시작하면 아무것도 하지 않음
		{
			++modicount;
		}
		else if( IsInt(in) || IsThis(in,"-()") )		// 첫 글자가 숫자나 괄호면 문자열에 값 추가
		{	
			Push_backNode(&inString,in);
			temp = in;				
		}
		else if( in == '.' )				//소수점으로 시작하면 맨 앞에 0추가
		{
			Push_backNode(&inString,'0');
			Push_backNode(&inString,'.');
			++dotcount;
			++modicount;
		}else
			++wrongcount;				// 조건에 하나도 걸리지 않으면 유효하지 않은 문자임
	*/	
		in = readchar(infile);	//첫 문자 인식
		//EOF or 개행 or '=' 문자가 나올때 까지 키보드 버퍼에서 값을 하나씩 빼와서 비교
		for(; in != EOF && in != '=' && (in != '\n' || inputmode != 0 ); in = readchar(infile))
		{	
			if( IsInt(in) )					//유효한 숫자 입력
			{
				if( temp == ')' && IsInt(in) )
				{						//괄호닫기 뒤에 숫자가 올 경우
					Push_backNode(&inString,'*');		//사이에 '*'를 넣어 )*n 꼴로 만들어 줌
					++modicount;
				}
				Push_backNode(&inString,in);			//문자열을 하나 추가
				temp = in;					//방금 넣은 문자을 이전입력 문자에 대입
			}
			else if( IsThis(in,"+-*().") )			//유효한 기호 입력
			{							
				//예외처리
				if( in == '.' )					//이번 문자가 소수점이면 특별 처리를 해줌
				{
					if( dotcount++ > 0  )			//한개의 항에 소수점이 두개 이상일 경우
					{
						++wrongcount;
						continue;			//이번 문자는 처리하지 않음
					}
					if( IsInt(temp) == 0 )			//만약 소수점 앞에 숫자가 없으면
					{
						Push_backNode(&inString,'0');	//앞에 0추가
                        			++modicount;
					}
				}
				else dotcount = 0;				//기호가 왔으면 한 항의 소수점 개수를 0으로 초기화

				if( IsThis(temp,"*+-.") && IsThis(in,"+*") ) 		//직전 노드와 이번 노드 둘 다 기호일경우
				{
					Pop_backNode(&inString);			//앞 기호 제거
					++wrongcount;
				}
				else if( temp == '-' && in == '(' )	 		//직전 문자가'-'이고 이번 문자가 '('일 경우
				{
					Push_backNode(&inString,'1');			//사이에 '1'를 넣어 -1*( 꼴로 만들어 줌
					Push_backNode(&inString,'*');			//사이에 '1'를 넣어 -1*( 꼴로 만들어 줌
					++modicount;
				}
				else if( temp == '-' && in == '-' )
				{
					Push_backNode(&inString,'1');			//사이에 '1'를 넣어 -1*( 꼴로 만들어 줌
					Push_backNode(&inString,'*');			//사이에 '1'를 넣어 -1*( 꼴로 만들어 줌
				}
		/*		else if( IsThis(temp,"(") && IsThis(in,"+-*.") )	//열기괄호 뒤에 바로 기호가 올 경우
				{
					
					if( in == '*' )
						Push_backNode(&inString,'1');		//(1*A 꼴로 만들어 줌
					else
						Push_backNode(&inString,'0');		//(0+A or (0-A or (0.A 꼴로 만들어 줌
					
				}
		*/		else if( IsThis(temp,"+-*.") && in == ')' )		//닫기괄호 바로 앞에 기호가 올 경우	
				{
					++wrongcount;
					Pop_backNode(&inString);			//기호  제거
				}
				else if( IsInt(temp) && in == '(') 			 
				{							//숫자 바로 뒤에 괄호가 올 경우
					Push_backNode(&inString,'*');			//사이에 '*'를 넣어 )*( 꼴로 만들어 줌
					++modicount;
				}
				else if( temp == ')' && in == '(' )
				{							//괄호닫기 뒤에 괄호열기가 올 경우
					Push_backNode(&inString,'*');			//사이에 '*'를 넣어 )*( 꼴로 만들어 줌
					++modicount;
				}
				else if( temp == '(' && in == ')' )			
				{				
					Push_backNode(&inString,'1');			//사이에 1을 넣어 줌
					++modicount;
				}
				Push_backNode(&inString,in);			//문자열을 하나 추가
				temp = in;					//방금 넣은 문자열을 이전입력 문자열에 대입
			}else if( isspace(in) )
				++modicount;
			else
				++wrongcount;					//유효하지 않은 문자
			
			++inputlength;					//입력한 문자열의 길이 1증가
		}

		__fpurge( stdin );

		if( IsThis(temp,"+-*(.") )
		{					//맨 마지막에 기호로 끝났다면
			Pop_backNode(&inString);	//마지막 노드 제거
			++wrongcount;
		}

		if( inString.size == 0 )//들어온 수식이 하나도 없으면
		{	
			printf("계산할 수식이 없습니다.\n");
			if( inputmode == 1 )
				break;
			if( ReIn() == 0 )		//수식을 다시 입력 받을 것인지 물음 
				break;			//프로그램 종료
			else
				continue;
		}
		//문자열 동적 할당
		_pstring = ListToString(&inString);	//링크드 리스트 문자열을 배열로 바꿔줌
		
		//괄호 예외처리
		bracket = 0;
		int _pstrlen = strlen(_pstring);
		for( int i = 0 ; i < _pstrlen ; ++i)
		{
			if( _pstring[i] == '(' )
				bracket++;
			else if( _pstring[i] == ')' )
				bracket--;
			if( bracket < 0 )		//괄호가 열리기 전에 닫힘
				break;
		}

//		printf("문자열 유효성 검사 완료\n");
		
		//파일 스트림 닫기
		if( inputmode == 1 && infile != NULL )
			fclose(infile);

		if( bracket != 0 )
		{
			printf("괄호의 갯수가 잘못되었습니다.");
			if( inputmode == 1 || ReIn() == 0 )
				break;
			else
				continue;
		}
		if( bracket < 0 )
		{
			printf("괄호가 열리기 전에 닫혔습니다.");
			if( inputmode == 1 || ReIn() == 0 )
				break;
			else
				continue;
		}

		printf("총 %d개 문자중에 %d개의 잘못된 문자가 검출 되었고 %d번 수정되었습니다.\n", inputlength, wrongcount, modicount);
		
		if( wrongcount > 0 )			//유효하지 않은 문자가 있었으면
		{
			if( inputmode == 0 )
			{
				printf("%s 이것을 입력 하려던 것입니까? (y/n)\n", _pstring);
				do
					in = getchar();
				while( !IsThis(in,"YNyn")&&in!=-1 );
				
				if( in == 'N' || in == 'n' )
				{
					if( ReIn() == 0 )		//수식을 다시 입력 받을 것인지 물음 
						break;			//프로그램 종료
					else
						continue;		//다시 입력 부분으로
				}
			}
			else if( inputmode == 1 )
			{
				if( force == 0 )		//종료
				{
					printf("%s의 저장된 수식이 옳지 않습니다. %s 이렇게 수정해 보시겠어요?\n", argv[1], _pstring);
					break;
				}
				if( force == 1 )		//강제 계산 모드
					printf("%s 이 수식으로 계산됩니다.\n", _pstring);
			}
		}

		ClearList(&inString);				//입력 문자열 리스트 삭제
	
		printf("계산 수식 %s\n", _pstring);
		gettimeofday(&startCaltime,NULL);
	/**************************************************/
	/**/	Num * resultnum = StackCal( _pstring );	/**/
	/**************************************************/
		gettimeofday(&nowtime,NULL);
		printf("결과 : ");	
		PrintNum(resultnum);
		printf("\n계산에 걸린 시간 %f초 \n", GetSec(startCaltime, nowtime));  
		
		if( inputmode == 1 )
		{	
			FILE * outfile;
			if(argc > 2 )
			{
				outfile = fopen(argv[2], "w");
				fPrintNum( outfile, resultnum );
			}
			else
			{ 
				outfile = fopen("re.txt", "w");
				fPrintNum( outfile, resultnum );
				printf("출력 파일명을 지정하지 않아 자동으로 re.txt에 저장하였습니다.\n");
			}
			fclose(outfile);
		}

		ClearNum(resultnum);
		break;	/*** 무한반복 탈출 절대 지우지 말것***/
	}

	//메모리 해제
	free(_pstring);
	gettimeofday(&nowtime,NULL);
	printf("프로그램 실행 시간 %f초 \n", GetSec(startProtime, nowtime)); 	
	return 0;
}
Num * StackCal( char * str )
{
	Num * re = malloc(sizeof(Num));
	int size = strlen(str);
	int numIndex = 0;
	
	List operStack;		// 변활 할때 필요한 연사자 스택
	NumList numPostfix;	// 변환된 식의 숫자 부분 저장
	List operPostfix;	// 변환된 식의 연산자 부분 저장
	List orderList;		// 변환된 식의 n번째 자리가 숫자인지 연산자 인지를 저장하는 리스트

	InitList(&operStack);
	InitNumList(&numPostfix);
	InitList(&operPostfix);
	InitList(&orderList);

	SetListName(&(operStack), "operStacki");
	SetListName(&(operPostfix), "operPosfix");
	SetListName(&(orderList), "oderList");

	//중위 표기된 문자열을 후위표기된 리스트(스택)으로 변환
	int i = 0;
	if( str[i] == '+' || str[i] == '*' )
	       ++i;	
	for( ; i < size ; ++i )
	{
		if( IsInt(str[i]) || str[i] == '.' )
		{
			++numIndex;
		}
		else if( i!= 0 && IsThis( str[i], "+-*(") )
		{
			//연산자를 만났으면 연산자 앞에 있는 숫자를 후위식에 넣어준다;
			if( numIndex > 0 )
			{
				Push_backNum(&numPostfix, StringToNum(str+(i-numIndex-1*((i-numIndex)>0)), numIndex+1*((i-numIndex)>0) ));
				Push_backNode(&orderList,0);
			}
			else if( str[i>0?i-1:0] != ')' && IsThis(str[i], "+-*"))
			       	continue;	
			numIndex = 0;

			//연산자 스택이 비였으면 그냥 스택에 방금 읽은 연산자 추가
			if( operStack.head == NULL )
				Push_backNode(&operStack, str[i]);
			else	//연산자 스택이 안 비였으면 
			{	//하나를 팝해서 방금 만난 연산자와 우선 순위를 비교한다
				
				char ch = Pop_backNode(&operStack);
				while( ch != -1 )
				{
					if( OperOrder(ch, str[i]) && str[i] != '(' )	//이전에 들어있던 연산자의 우선 순위가 더 크거나 같다면
					{						//이전 연산자를 후위표기식에 넣어주고
						if( ch =='-' ) ch = '+';
						Push_backNode(&operPostfix, ch);
						Push_backNode(&orderList,1);
						ch = Pop_backNode(&operStack);
					}else	//방금 만난 연산자의 우선순위가 더 높다면
					{	//꺼냈던 이전 연사자는 다시 스택에 넣어준다
						Push_backNode(&operStack,ch);
						break;
					}
				}
				Push_backNode(&operStack, str[i]);
			}
		}
		else if( str[i] == ')' )
		{
			if( numIndex != 0 )
			{
				Push_backNum(&numPostfix, StringToNum(str+(i-numIndex-1*((i-numIndex)>0)), numIndex+1*((i-numIndex)>0) ));
				Push_backNode(&orderList,0);
			}
			numIndex = 0;
			while( 1 )
			{
				char ch = Pop_backNode(&operStack);
				if( ch != '(' && ch != -1)					
				{
					if( ch == '-' ) ch = '+';
					Push_backNode(&operPostfix, ch);
					Push_backNode(&orderList,1);
				}else break;
			}
		}
	}
	if( numIndex != 0 )
	{	
		Push_backNum(&numPostfix, StringToNum(str+(i-numIndex-1*((i-numIndex)>0)), numIndex+1*((i-numIndex)>0) ));
		Push_backNode(&orderList,0);
	}
	for( Node * it = operStack.tail ; it != NULL ; it = it->pre )
	{
		if( it->value == '-' ) it->value = '+';
		Push_backNode(&operPostfix, it->value);
		Push_backNode(&orderList,1);
	}
	
	//Postfix변환 완료
	NumNode * it2 = numPostfix.head;
	Node * it3 = operPostfix.head;
	printf("Postfix : ");
	for( Node * it = orderList.head ; it != NULL ; it = it->next )
	{
		if( it->value == 0 )
		{
			PrintNum(&(it2->num));
			it2 = it2->next;
		}else
		{
			printf("%c", it3->value);	
			it3 = it3->next;
		}
	}
	printf("\n");

	//계산시작
	NumList numStack;
	InitNumList(&numStack);
	for( Node * it = orderList.head ; it != NULL ; it = it->next )
	{	
		if( it->value == 0 )
		{
			Num * num = Pop_frontNum(&numPostfix); 	
			Push_backNum(&numStack, num);
		}
		else if( it->value != 0 )
		{
			Pop_frontNode(&orderList);
			char oper = Pop_frontNode(&operPostfix);
		
			Num * num2;
			Num * num1;
			
			num2 = Pop_backNum(&numStack);
			num1 = Pop_backNum(&numStack);

			switch(oper)
			{
				case '+':Push_backNum(&numStack, AddNum(num1,num2)); break;
				case '*':Push_backNum(&numStack, MulNum(num1,num2)); break;
			}

			ClearNum(num1);
			ClearNum(num2);
			free(num1);
			free(num2);

		}
	}
	
	*re = *Pop_backNum(&numStack);

	ClearNumList(&numStack);
	ClearList(&operStack);
	ClearNumList(&numPostfix);
	ClearList(&operPostfix);
	ClearList(&orderList);

	return re;
}
//a가 list중 하나인지
int IsThis(char a, const char * list)
{	
	for( int i = 0 ; i < strlen(list); ++i)
		if( list[i] == a )
			return 1;
	return 0;
}
// '0'~'9' 이면 1을 반환
int IsInt(char a)
{
	if( a >= '0' && a <='9' )
		return 1;
	return 0;
}
//연산자 우선순위 비교 a가 높거나 같으면 1
int OperOrder( char ch1, char ch2 )
{	
	int a = (ch1=='*')*3 + (ch1=='+'||ch1=='-')*2 + (ch1=='('||ch1==')');
	int b = (ch2=='*')*3 + (ch2=='+'||ch2=='-')*2 + (ch2=='('||ch2==')');
	
	if( a>=b )
		return 1;	
	return 0;
}
//수식 재 입력 의사 묻기
int ReIn()
{
	__fpurge( stdin );
	char in;

	printf("다시 입력 하시겠습니까? (y/n)");

	do
		in = fgetc(stdin);
	while( !IsThis(in,"YyNn") && in != -1 && in != 10);


	if( IsThis(in,"Nn") )
	{
		system("clear");
		printf("프로그램이 종료됩니다\n");
		return 0;
	}

	return 1;
}
//두 시간 사이 흐른 초를 반환
float GetSec( struct timeval t1, struct timeval t2 )
{
	return t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000000.0;
}
