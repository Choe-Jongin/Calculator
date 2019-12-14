#include <stdio.h>
#include <stdlib.h>
#include "Num.h"
DATA RShiftList( List * list, DATA _dv )
{
        DATA dv = Pow(10,_dv);
        DATA nsmax = Pow(10,NSLEN-_dv);

        DATA re = (list->tail->value%dv)*nsmax;
        list->tail->value /= dv;

        for( Node * it = list->tail->pre ; it != NULL ; it = it ->pre )
        {
                it->next->value += (it->value%dv)*nsmax;
                it->value /= dv;
        }

        return re;
}

//초기화
void InitNum(Num * num)
{
        InitList(&(num->intList));
        InitList(&(num->decList));
        num->sign = 0;
}
//정수값 추가
void Push_backInt(Num * num, int v)
{
        Push_backNode(&(num->intList), v);
}
//소수값 추가
void Push_backDec(Num * num, int v)
{
        Push_backNode(&(num->decList), v);
}
//전체 삭제
void ClearNum(Num * num)
{
      ClearList(&(num->intList));
      ClearList(&(num->decList));
}
//소수 부분의 값들을 정수 부분에 옮기고 옴긴 자릿수를 반환
int MoveDeciToInt(Num * num)
{
	if( num == NULL || num->decList.head == NULL )
		return 0;
	int index = num->decList.size;
	num->intList.size += num->decList.size;
	
	if( num->intList.tail == NULL )
		num->intList = num->decList;
	else
		num->intList.tail->next = num->decList.head;
	
	num->decList.head->pre = num->intList.tail;
	num->intList.tail = num->decList.tail;

	num->decList.head = num->decList.tail = NULL;
	num->decList.size = 0;
	
	return index;
}
//Num구조체 덧셈
Num * AddNum(Num * num1, Num * num2)
{
	Num * re = (Num*)malloc(sizeof(Num));	// 반활 될 무한수. 연산의 결과를 담고 있음
	InitNum(re);
	SetListName(&(re->intList),"AddNum");

	List * temp = (List*)malloc(sizeof(List));
	InitList(temp);
	int tempsign = postive;

	int deci1 = 0, deci2 = 0;	//각 무한수의 소수가 정수부분으로 몇번 옮겨 졌나 저장
	int deciMax = 0;		//둘중 큰 값을 닮을 거임

	//소수 부분을 정수 부분으로 옮기고 몇 자리 옮겼는지 저장
	deci1 = MoveDeciToInt(num1);	
	deci2 = MoveDeciToInt(num2);	
	//둘 중 큰 값을 저장
	deciMax = deci1>=deci2 ? deci1 : deci2;

	//만약 더 적게 옮겨진 쪽이 있다면 더 적게 옮겨진 만큼 정수 부분 뒤에 0을 추가 해줌
	//ex) 11.111 + 1.1 -> 11111+11 -> 11111+1100
	for( int i = 0 ; i < deci1 - deci2 ; ++i )
		Push_backNode(&(num2->intList),0);
	for( int i = 0 ; i < deci2 - deci1 ; ++i )
		Push_backNode(&(num1->intList),0);
	
	//대소 비교
	Relational numrelational = ListCmp(num1->intList, num2->intList);	//실제 숫자(부호없음
	Relational length = equal;						//자릿수
	if( num1->intList.size != num2->intList.size )
		length = num1->intList.size > num2->intList.size ? greater : smaller ;

	//앞에 남는 자리 짤라줌
	if( length == greater )
		temp = DivideList(&(num1->intList), num2->intList.size);
	if( length == smaller )
		temp = DivideList(&(num2->intList), num1->intList.size);	

	//두 수의 부호가 다르면
        if( num1->sign ^ num2->sign)
        {
                //음수의 경우 보수화를 하고 자릿수를 맞춰준다
                if( num1->sign == negative )
		{
			if( numrelational == greater )
				re->sign = negative;
                        ComplimentList(&(num1->intList),0);
		}
		else
		{
			if( numrelational == smaller )
				re->sign = negative;
		     	ComplimentList(&(num2->intList),0);
		}
        }
	
	//********두 리스트를 더하는 함수 호출하고 자리 올림 저장**********//
	int carry = SumList(&(re->intList), (num1->intList), (num2->intList));
	
	//************************사실상 뺄셈 부분*************************//
        //음수 부호가 하나 있다면
        if( num1->sign ^ num2->sign)
        {
		int sign = postive;
                if( carry > 0 )	//자리 올림 발생 시(결과가 양수임)
                {
                        //맨 뒷자리 숫자 1증가
                        for( Node * it = re->intList.tail ; it != NULL ; it = it->pre )
                        {
                                it->value += 1;
                                if( it->value >= NS ) //맨 뒷자리가 NS-1이였다면 0으로 만들고 그 앞자리에 1을 더해 주는 반복문
                                        it->value = 0;
                                else
                                        break;
                        }
                }
                else		//자리 올림 미 발생 시(결과가 음수임)
                {
			ComplimentList(&(re->intList), 0);	//결과 보수화
			sign = negative;
                }
		//덧셈의 부호와 결과의 부호가 다르면
		if( re->sign != sign )
		{
			//임시리스트에서 1빼줌
			Node * it = NULL;
			for( it = temp->tail ; it != NULL ; it = it->pre  )
			{
				it->value -= 1;
				if( it->value  < 0 ) //맨 뒷자리가 0이였다면 NS-1으로 만들고 그 앞자리에 1을 빼 주는 반복문
					it->value = NS-1;
				else
					break;
			}
			if( it == NULL ) //임시 리스트가 전부 0이였거나 없었으면
				re->sign = negative;
			else
			{	
				ComplimentList(&(re->intList), 0);	//결과 보수화
				for( it = re->intList.tail ; it != NULL ; it = it->pre )
				{
					it->value += 1;
					if( it->value >= NS ) //맨 뒷자리가 NS-1이였다면 0으로 만들고 그 앞자리에 1을 더해 주는 반복문
						it->value = 0;
					else
						break;
				}
			}
		}
        }
        //*******************************덧셈 부분*************************//
	else
	{
		if( carry > 0 )				//자리올림이 발생하였다면 
		{
			Node * it = NULL;
			for( it = temp->tail ; it != NULL ; it = it->pre  )
			{					
				it->value += 1;		//임시리스트에 1을 더해준다
				if( it->value >= NS )
					it->value = 0;
				else
					break;
			}
			if( it == NULL )		//만약 임시리스트가 없었거나 9999......9 였다면
				Push_frontNode(temp,1);	//1 or 1000.......0꼴로 만들어 준다
		}
		re->sign = num1->sign;
	}
	AppendList(temp,&(re->intList));

	//더해져서 나온 결과 값에 아까 옮긴 소수만큼 정수 부분에서 소수 부분으로 이동
	//ex 11111+1100 -> 12211 -> 12.111
	for( int i = 0 ; i < deciMax ; ++i )
		Push_frontNode(&(re->decList),Pop_backNode(&(re->intList)));
	
	free(temp);
	TrimNum(re);
	return re;
}		
//Num구조체 곱셈
Num * MulNum(Num * num1, Num * num2)
{
	Num * re = (Num*)malloc(sizeof(Num));
	InitNum(re);
	SetListName(&(re->intList),"MulNum");

	re-> sign = num1->sign^num2->sign;	//	 XOR연산(다르면 negative)

       	int deci1 = 0, deci2 = 0;		//각 무한수의 소수가 정수부분으로 몇번 옮겨 졌나 저장
        int deciMax = 0;			//옮겨진 자릿수의 합
	
	//소수 부분을 정수 부분으로 옮기고 몇 자리 옮겼는지 저장
  	deci1 = MoveDeciToInt(num1);	
  	deci2 = MoveDeciToInt(num2);	
 	deciMax = (deci1+deci2)*NSLEN;

	if( deci1 > 0 )
	{
		DATA value = num1->intList.tail->value;
		int margin = 0;
		while ( value%10 == 0)
		{
			value /= 10;
			++margin;
		}
		RShiftList( &(num1->intList), margin);
		deciMax -= margin;
	}
	if( deci2 > 0 )
        {
                DATA value = num2->intList.tail->value;
                int margin = 0; 
		while ( value%10 == 0)
                {
                        value /= 10;
                        ++margin;
                }
		RShiftList( &(num2->intList), margin);
		deciMax -= margin;
        }
	///////////////리스트 곱셈 호출///////////////
	MulList(&(re->intList), (num1->intList), (num2->intList));
	
	if( deciMax > 0 )
	{
		//소수점 적용
		Push_frontNode(&(re->decList), RShiftList(&(re->intList), deciMax%NSLEN));	
		for( int i = 0 ; i < deciMax/NSLEN ; ++i )
		{
			DATA front = Pop_backNode(&(re->intList));
			Push_frontNode(&(re->decList), front == -1 ? 0 : front);
		}
	}

	TrimNum(re);
	return re;
}
//앞 뒤 0제거, 정수부분이 비였으면 0추가 값이 0이면 부호 0으로
void TrimNum(Num * num)
{
	for( Node * it = num->intList.head ; it != NULL && it->value == 0 ;)
	{
		it = it->next;
		Pop_frontNode(&(num->intList));
	}
	for( Node * it = num->decList.tail ; it != NULL && it->value == 0 ;)
	{	
		it = it->pre;
		Pop_backNode(&(num->decList));
	}
	//만약 정수 부분이 하나도 없으면 0추가
	if( num->intList.head == NULL )
	{
		if( num->decList.head == NULL )	//소수부분까지도 없으면 즉 0이면 부호는 +로..
			num->sign = postive;
		Push_backNode(&(num->intList), 0);
	}
}
//값 화면 출력
void PrintNum(Num * num)
{	
	if( num == NULL )
		return ;
	
	if( g_str == NULL )
	{
		g_str = (char*)malloc(5);
		g_str[0] = '%';
		g_str[1] = '0';
		g_str[2] = '0'+NSLEN;
		g_str[3] = 'd';
		g_str[4] = '\0';
	}
	printf("[");
	if( num -> sign == negative )
		printf("-");

	Node * it = num->intList.head;
		printf("%d",it->value);
	for( it = it->next ; it != NULL ; it = it->next )
	{
		printf(g_str,it->value);
	}
	if( num->decList.size > 0 )
        {
                printf(".");
                for( it = num->decList.head ; it != num->decList.tail ; it = it->next )
                       printf(g_str,it->value);
		int last = it->value;
		char laststr[5] = "%00d";
		laststr[2] = '0' + NSLEN;
		while( last%10 == 0 )	
		{
			last/=10;
			--laststr[2];
		}
		printf( laststr, last);
        }
	printf("]");
}
void fPrintNum(FILE * file, Num * num)
{	
	if( num == NULL )
		return;

	if( g_str == NULL )
	{
		g_str = (char*)malloc(5);
		g_str[0] = '%';
		g_str[1] = '0';
		g_str[2] = '0'+NSLEN;
		g_str[3] = 'd';
		g_str[4] = '\0';
	}
	if( file == NULL )
	{
		printf("저장파일이 올바르지 않습니다.\n");
		return;
	}
	if( num -> sign == 1 )
		fprintf(file, "-");
	Node * it = num->intList.head;
        fprintf(file,"%d", it->value);
	for( it = it->next ; it != NULL ; it = it->next )
                fprintf(file, g_str, it->value);
        if( num->decList.size > 0 )
        {
                fprintf(file, ".");
                for( it = num->decList.head ; it != num->decList.tail ; it = it->next )
                      fprintf(file, g_str,it->value);

		int last = it->value;
		char laststr[5] = "%00d";
		laststr[2] = '0' + NSLEN;
		while( last%10 == 0 )	
		{
			last/=10;
			--laststr[2];
		}
		fprintf(file, laststr, last);
	}
}
Num * StringToNum( char * str, int size )
{
	Num * re = (Num*)malloc(sizeof(Num));
	InitNum(re);
	int isdec = 0;
	if( str[0] == '-' )
		re->sign = negative;

	while( *str < '0' || *str > '9' )
	{
		--size;
		++str;
	}
	
	DATA value = 0;
	while( isdec < size && str[isdec] != '.' )
		++isdec;
	//정수 부분 추가
	for(int i = isdec-1 ; i >= 0 ; i -= NSLEN )
	{
		value = 0;
		if( i >= NSLEN )
		{	
			for( int j = 0 ; j < NSLEN ; ++j )
				value += (str[i-j]-'0')*Pow(10,j);
			Push_frontNode(&(re->intList), value);
		}else
		{
			for( int j = 0 ; i-j >= 0 ; ++j )
				value += (str[i-j]-'0')*Pow(10,j);
			Push_frontNode(&(re->intList), value);
		}
	}
	
	//소수 부분 추가
	if( str[isdec] == '.' )
	{
		str = &(str[isdec+1]);
		value = 0;
		for( int i = 0 ; str[i] >= '0' && str[i] <= '9' ; ++i )
		{
			value += (str[i]-'0')*Pow(10,NSLEN-(i%NSLEN)-1);
			if( i%NSLEN == NSLEN-1 )
			{
				Push_backNode(&(re->decList), value);
				value = 0;
			}
		}
		if( value != 0 )
			Push_backNode(&(re->decList), value);
	}

	TrimNum(re);	
	return re;
}
