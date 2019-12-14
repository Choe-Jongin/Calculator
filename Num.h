#include "List.h"

char * g_str;

enum Sign {postive, negative};
typedef enum Sign Sign;
//값의 표현범위가 없는 실수 구조체
typedef struct
{
        List intList;  	//정수부분을 담는 리스트
        List decList;   //소수부분을 담는 리스트
	Sign sign;
}Num;

void InitNum(Num * num);		//초기화
void Push_backInt(Num * num, int v);	//정부부분 추가
void Push_backDec(Num * num, int v);	//소수부분 추가
void ClearNum(Num * num);		//전체삭제(메모리 해제)
int MoveDeciToInt(Num * num);		//소수 부분의 값들을 정수 부분에 옮기고 옴긴 자릿수를 반환
Num * AddNum(Num * num1, Num * num2);	//덧셈
Num * MulNum(Num * num1, Num * num2);	//곱셈
void TrimNum(Num * num);		//정수부 맨 앞, 소수부 맨 뒤 0 제거, 정수부에 값 없으면 0추가
void PrintNum(Num * num);		//값 출력
void fPrintNum(FILE * file, Num * num);		//값 출력
Num * StringToNum( char * str, int size ); //문자열을 무한수 구조체로 바꿔줌
