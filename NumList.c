#include <stdio.h>
#include <stdlib.h>
#include "NumList.h"

//무한수 링크드리스트 초기화
void InitNumList(NumList * list)
{
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
}
//매게변수로 받는 Num객체를 무한수 링크드 리스트 맨 뒤에 추가해줌
void Push_backNum(NumList * list, Num * num)
{
	NumNode * newNode = (NumNode*)malloc(sizeof(NumNode));
	newNode->num = *num;
	newNode->next = NULL;

	if( list->head == NULL )
        {
        	newNode->pre = NULL;
		list->head = newNode;
        }
        else
        {
		list->tail-> next = newNode;
		newNode->pre = list->tail;
	}

        list->tail = newNode;
	++(list->size);
}
Num * Pop_backNum(NumList * list)
{
	if( list->tail == NULL )
		return NULL;
	
	Num * re = (Num*)malloc(sizeof(Num));
	InitNum(re);
	*re = list->tail->num;

        NumNode * temp = list->tail;	//지울 노드의 주소값 임시저장
        list->tail = list->tail->pre;	//마지막 노드는 원래 마지막 노드의 이전 노드가 됨

        if( temp == list->head )        //지울 맨 뒤 노드가 맨 앞의 노드라면
                list->head = NULL;      //헤드도 가르키는 노드가 없음
        else                            //노드가 더 남아 있는 경우라면
                list->tail->next = NULL;//마지막 노드가 된 테일의 다음 노드는 없어야 함
        
	free(temp);
        --(list->size);

        return re;
}
Num * Pop_frontNum(NumList * list)
{
	if( list->head == NULL )
                return NULL;
        Num * re = (Num*)malloc(sizeof(Num));
       	InitNum(re);
	*re = list->head->num;
        NumNode * temp = list->head;    //temp에는 지울 노드의 주소를 담아둠
        list->head = list->head->next;  //이제 헤드는 원래 헤드의 다음 노드가 됨

        if( temp == list->tail )        //지울 맨 앞 노드가 맨 뒤의 노드라면
                list->tail = NULL;      //테일도 가르키는 노드가 없음
        else                            //노드가 더 남아 있는 경우라면
                list->head->pre = NULL;//처음 노드가 된 헤드의 이전 노드는 없어야 함

        free(temp);
        --(list->size);
	
        return re;
}
//모든 무한수들을 화면에 표시
void PrintNumList(NumList * list)
{
        if( list->head == NULL )
	{
		printf("Empty List\n");
		return ;
	}
        for( NumNode * it = list->head ; it != list->tail->next ; it = it->next )
       	{
                PrintNum(&(it->num));
                printf("\n");
        }
}
//현재 노드 메모리 해제 + 다음 노드 메모리 해제(재귀함수)
void FreeNumNode(NumNode * node, int *size)
{
	if( node == NULL)
		return ;
        if( node->next != NULL )
	  	FreeNumNode( node->next, size);
        
	ClearNum(&(node->num));
        --(*size);
        free(node);
}

//구조체 전체 초기화(메모리해제)
void ClearNumList(NumList * list)
{
        if( list->head != NULL )
                FreeNumNode(list->head, &(list->size));
}

