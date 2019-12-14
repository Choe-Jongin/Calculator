#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

//초기화
void InitList(List * list)
{
        list->head = NULL;
	list->tail = NULL;
        list->size = 0;
	strcpy(list->name,"no name");
}
//리스트의 이름 설정
void SetListName(List * list, char * str)
{
	strcpy(list->name,str);
}
//맨 뒤에 노드 추가
void Push_backNode(List * list, DATA value)
{	
	Node * newNode = (Node*)malloc(sizeof(Node));	//새로운 노드를 만들고
	newNode->value = value;				//그 노드에 값을 넣음
	newNode->next = NULL;				//새 노드니까 다음 노드가 없음

	//첫번째 노드 일 경우
        if( list->head == NULL )
        {
		newNode->pre = NULL;
		list->head = newNode;			//이 리스트에 헤드가 방금 추가한 노드가 됨
	}
        else	// 첫번째 노드가 아닐 경우
        {
		list->tail->next = newNode;		//원래 마지막 노드의 다음 노드는 방금 추가한 노드가 됨	
		newNode->pre = list->tail;		//새 노드의 이전 노드는 현재 마지막 노드인 테일이 됨	
	}
		
	list->tail = newNode;		//이 리스트의 테일은 방금 추가한 노드가 됨
	++(list->size);			//해당 구조체가 가지고 있는 노드의 수 증가

}
//맨 뒤에 노드를 제거하고 그 값을 반환
DATA Pop_backNode(List * list)
{
	if(list -> tail == NULL )
		return -1;
	
	DATA re = list->tail->value;	//지울 노드가 가지고 있던 값 저장 후 반환

	Node * temp = list->tail;	//지울 노드의 주소값 임시저장
	list->tail = list->tail->pre;	//마지막 노드는 원래 마지막 노드의 이전 노드가 됨
	
	if( temp == list->head )	//지울 맨 뒤 노드가 맨 앞의 노드라면
		list->head = NULL;	//헤드도 가르키는 노드가 없음
	else 				//노드가 더 남아 있는 경우라면
		list->tail->next = NULL;//마지막 노드가 된 테일의 다음 노드는 없어야 함
	free(temp);
	
	--(list->size);
	
	return re;
}
//맨 앞에 노드를 추가
void Push_frontNode(List * list, DATA value)
{
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->value = value;				//그 노드에 값을 넣음
	newNode->pre = NULL;				//새 노드니까 이전 노드가 없음

	//첫번째 노드 일 경우
        if( list->head == NULL )
        {
		newNode->next = NULL;
		list->tail = newNode;			//이 리스트에 테일이 방금 추가한 노드가 됨
	}
        else	// 첫번째 노드가 아닐 경우
	{
		list->head->pre = newNode;
		newNode->next = list->head;
        }

	list->head = newNode;
        ++(list->size);			//해당 구조체가 가지고 있는 노드의 수 증가
	
}
//맨 앞 노드를 제거하고 그 값을 반환
DATA Pop_frontNode(List * list)
{
	if( list->head == NULL )	//지울게 없으면 아무것도 하지 않고 -1을 반환
		return -1;
	
	DATA re = list->head->value;	//반환 값
	Node * temp = list->head;	//temp에는 지울 노드의 주소를 담아둠
	list->head = list->head->next;	//이제 헤드는 원래 헤드의 다음 노드가 됨

	if( temp == list->tail )	//지울 맨 앞 노드가 맨 뒤의 노드라면
		list->tail = NULL;	//테일도 가르키는 노드가 없음
	else 				//노드가 더 남아 있는 경우라면
		list->head->pre = NULL;//처음 노드가 된 헤드의 이전 노드는 없어야 함

	free(temp);
	--(list->size);
	return re;
}
//구조체 메모리 해제
void ClearList(List * list)
{
	//헤드가 존재 한다면
        while(list->head != NULL)
        {
		Node * temp = list->head;
		list->head = list->head->next;
		if( list-> head != NULL)
		       	list->head->pre = NULL;	
		free(temp);
		--(list->size);			//개수 감소(필요 없지만.. 뭔가 있어야 마음이 놓임)
	}
}
//리스트를 문자열로 변환
char * ListToString(List * list)
{
	char * re = malloc(list->size+1);
	int i = 0;
	for( Node * it = list->head ; it != NULL ; it = it->next)
	{
		re[i++] = it->value;	
	}
	re[i] = '\0';
	return re;
}
int SumList(List * tar, List list1, List list2)
{
	return SumNode( tar, list1.tail, list2.tail, 0 );
}
int SumNode(List * list, Node *n1, Node * n2, DATA s)
{
	DATA result = s;
	if( n1 != NULL )
		result += n1->value;
       	if( n2 != NULL )
		result += n2->value;
	
	DATA carry = result / NS;
	result %= NS;
	
	Node *pre1 = NULL;
	Node *pre2 = NULL;

	if( n1 != NULL )
		pre1 = n1->pre;
	if( n2 != NULL )
		pre2 = n2->pre;

	if( n1 != NULL || n2 != NULL)
	{
		Push_frontNode(list,result);
		carry = SumNode(list, pre1, pre2, carry);
	}
	else
		carry = s;
	return carry;
}
void MulList(List * tar, List list1, List list2)
{
	int i = 0;
	for( Node * it = list2.tail ; it != NULL ; it = it->pre )
	{
		if( (i+1)%100 == 0 )
			printf("%d자리 곱 result size:%d\n", i+1 , tar->size);

		List temp;
		List targetTemp;
		InitList(&temp);
		InitList(&targetTemp);
		SetListName(&temp,"temp");
		DATA n = it->value;

		if( n == 0 )
		{
			++i;
			continue;
		}
	 	else if( n == 1 )
			ListCpy(&temp, &list1);
		else
			MulNode(&temp, list1.tail, n, 0);
		
		for( int j = 0 ; j < i ; ++j )
			Push_backNode(&temp, 0);

		int carry = SumList(&targetTemp, temp, *tar);
		if( carry > 0 )
			Push_frontNode(&targetTemp,carry);
		ClearList(tar);
	
		*tar = targetTemp;
		++i;
	
		ClearList(&temp);
	}

}
int MulNode(List * list, Node *n1, DATA n, DATA s)
{
	LONGDATA result = s;
	if( n1 != NULL )
		result = (LONGDATA)n1->value * (LONGDATA)n + s;
	
	LONGDATA carry = (LONGDATA)result/(LONGDATA)NS;
	result %= NS;

	if( n1 == NULL )
	{
		if( s > 0 )
			Push_frontNode(list, s);
	}else{
		Push_frontNode(list,result);
		MulNode(list, n1->pre, n, carry);
	}
	return carry;
}
//List 9의 보수화
void ComplimentList(List * list, int size)
{
	for( Node * it  = list->head ; it != NULL ; it = it->next)
		it->value = NS-1-it->value;
	
	for( int i = 0 ; list->size < size ; ++i )
		Push_frontNode(list,NS-1);
}
void PrintList( List * list )
{
	if( list == NULL )
		printf("empty List\n");
	printf("%s List info size : %d,  value : ",list->name,list->size);
	for( Node * it = list -> head ; it != NULL ; it = it->next )
		printf("%d ",it->value);
	printf("\n");
}
//리스트 두개를 하나로  붙임
void AppendList(List * headlist, List * taillist) 
{
        if(headlist->tail == NULL)
                return;

        if(taillist->head == NULL)
                taillist = headlist;
        else
        {
                taillist->head->pre = headlist->tail;
                headlist->tail->next = taillist->head;
                taillist->head = headlist->head;
                taillist->size += headlist->size;
        }

        headlist->head = NULL;
        headlist->tail = NULL;
        headlist->size = 0;
}
//한개의 리스트를 size만큼 짤라서 반환하고 남아있는 값은 소스(src)에 저장
List *DivideList(List * src, int size )		
{
        if( src == NULL || size <= 0 )
                return NULL;

        List * re = (List*)malloc(sizeof(List));
        InitList(re);

        if( size >= src->size )
        {
                *re = *src;
                src->head = NULL;
                src->tail = NULL;
                return re;
        }

        re->head = src->head;
        for( re->tail = src->head ; re->tail != NULL ; re->tail = re->tail->next )
        {
		++(re->size);
                if( --(src->size) <= size )
                        break;
        }

        src->head = re->tail->next;
        src->head->pre = NULL;
        re->tail->next = NULL;
        return re;
}
int Pow(int v, int e )
{
	int re = 1;
	for( int i = 0 ; i < e ; ++i)
		re *= v;
	return re;
}
Relational ListCmp(List list1, List list2)
{
        if( list1.size > list2.size )
                return greater;
        if( list1.size < list2.size )
                return smaller;

        Node * n1 = list1.head;
        Node * n2 = list2.head;

        while( n1 != NULL || n2 != NULL ) // 논리곱으로 묶어야 하지만 동치이므로 의미 없는 조건이기에 논리합연산을 한다
        {
                DATA v1 = n1->value;
                DATA v2 = n2->value;
                if( v1 > v2 )
                        return greater;
                if( v1 < v2 )
                        return smaller;

                n1 = n1->next;
                n2 = n2->next;
        }

        return equal;
}
//리스트 복사
void ListCpy(List * tar, List * src)
{
	if( src == NULL || src->head == NULL )
		return ;

	for( Node * it = src->head ; it != NULL ; it = it->next)
		Push_backNode(tar, it->value);
}
