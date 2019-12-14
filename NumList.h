#include "Num.h"

//무한수 구조체 노드
typedef struct NumElement
{
        Num num;
        struct NumElement * next;
        struct NumElement * pre;

}NumNode;

//무한수 구조체 링크드 리스트
typedef struct
{
        int size;
        NumNode * head;
        NumNode * tail;

}NumList;

void InitNumList(NumList * list);			//무한수 구조체 초기화
void Push_backNum(NumList * list, Num * num);		//맨 뒤에 무한수 구조체 하나 추가
Num * Pop_backNum(NumList * list);			//맨 뒤 구조체를 지우고 그 주소를 반환
Num * Pop_frontNum(NumList * list);
void PrintNumList(NumList * list);			//구조체에 있는 모든 값 화면 표시
void ClearNumList(NumList * list);			//구조체 해제
void FreeNumNode(NumNode *node, int * size);		//노드 한개와 그 노드 이후의 모든 노드들을 해제
