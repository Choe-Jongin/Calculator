#include "Setting/Setting.h"

typedef enum{smaller = -1, equal = 0, greater = 1} Relational;

//정수형 하나를 표현 하기 위한 노드
typedef struct Element
{
        DATA value;
        struct Element * next;
	struct Element * pre;
}Node;

//정수형 연결 리스트 구조체 
typedef struct
{
        int size;
	char name[20];
        Node * head;
	Node * tail;
}List, String;  // 정수형 리스트를 문자열로 사용 할 것임

void InitList(List * list);   					//리스트 초기화
void SetListName(List * list, char * str);			//리스트 이름 설정(디버깅용)
void Push_backNode(List * list, DATA value);			//맨 뒤에 노드 추가
DATA Pop_backNode(List * list);					//맨 뒤 노드 제거
void Push_frontNode(List * list, DATA value);			//맨 앞에 노드 추가
DATA Pop_frontNode(List * list);				//맨 아에 노드 제거
void ClearList(List * list);					//노드 초기화
char * ListToString(List * list);				//리스트를 문자열로 변환
int SumList(List * tar, List list1, List list2);		//두개의 리스트를 덧셈을 함
int SumNode(List * list, Node *n1, Node * n2, DATA s);		//두개의 노드를 더하고 다음 노드에 자리올림수를 더함
void MulList(List * tar, List list1, List list2);		//두개의 리스트 곱셈
int MulNode(List * list, Node *n1, DATA n, DATA s);		//한개의 리스트 에 한자리 수를 전부 곱함
void ComplimentList(List * list, int size);			//리스트 9의 보수
void PrintList( List * list );					//리스트 정보 화면 표시
void AppendList(List * headlist, List * taillist );		//리스트 두개를 하나로  붙임
void ListCpy(List * tar, List * src);				//리스트 복사
List *DivideList(List * src, int size );			//한개의 리스트를 size만큼 짤라서 반환하고 남아있는 값은 소스(src)에 저장
Relational ListCmp(List list1, List list2);
int Pow(int v, int e );
