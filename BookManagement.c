#include<stdio.h>  //표준 입출력 라이브러리 여러 입출력(printf등)합수가 포함된 헤더 파일
#include<stdlib.h> //표준 라이브 러리 문자열 변환 메모리 관리등 함수를 포함한 헤더 파일
typedef struct booktype {				//구조체 선언
	int serial_num;			//시리얼 넘버 입력하고 있는 시리얼 넘버를 알려주고 삭제할때 쓰임
	char title[30];			//책의 제목 데이터를 담음
	char author[15];		//저 자 데이터를 담음
	char price[10];			//가격 데이터를 담음
	struct booktype *previous; //노드에 앞부분을 가르킬 포인터
	struct booktype *next;		//연결리스트에 넘어가는 부분
} BOOK;			//구조체(연결리스트) 이름
BOOK *delete(BOOK *list, int sern);  //데이터 삭제를 위한 함수
void free_list(BOOK *head);			//재귀 함수로 메모리 할당 없애줌
int main()
{
	BOOK *list, *temp;      //공간을 가르켜줄 포인터 변수
	int num = 0, numb, dnum = 0, end = 0;   //numb=사용자에게 입력받은 메뉴 번호를 swich로 전달해줄 수 dnum=삭제할때 쓰일 사용자 입력 수 end=종료시 반복문 끝낼 수
	list = (BOOK*)malloc(sizeof(BOOK));  //메모리 할당
	temp = list;		//list와 같은곳을 가르킴
	while (1)   //반복문 사용자가 종료 할때가지 반복해줌
	{                                                   //메뉴
		printf("도서 관리 프로그램 입니다.\n");
		printf("1.추가 \n2.삭제\n3.출력\n4.나가기\n");
		scanf_s("%d", &numb);

		switch (numb)						//사용자가 입력한 명령 실행해줄 swich
		{
		case 1:								//사용자가 입력한 도서 정보
			temp->serial_num = ++num;		 //시리얼 넘버
			printf("%d ", temp->serial_num); //시리얼 넘버 보여줌
			printf("제목을 입력하시오:");
			scanf_s("%s", temp->title, 20);			//문자열로 저장, temp 공간에 제목에 저장해줌 , 입력받을 크기
			printf("저자를 입력하시오:");
			scanf_s("%s", temp->author, 20);		//문자열로 저장, temp 공간에 저자에 저장해줌 , 입력받을 크기
			printf("가격을 입력하시오:");
			scanf_s("%s", temp->price, 20);		//문자열로 저장, temp 공간에 가격에 저장해줌 , 입력받을 크기
			temp->next = (BOOK*)malloc(sizeof(BOOK));    //temp->next에 공간을 할당해줌 즉,다음으로 넘어가기 위한 공간 할당
			temp = temp->next;		//다음 할당된 공간으로 넘어감
			temp->next = NULL;		//널을 넣어 종료 시킬때 사용
			break;  //스위치문을 빠져 자감
		case 2:                            //사용자가 입력한 삭제할 시리얼 넘버
			printf("삭제하고 싶은 시리얼 넘버를 입력하시오.");
			scanf_s("%d", &dnum);
			list = delete(list, dnum); //삭제 노드 함수 사용자가 입력한 시리얼 넘버에 노드를 삭제함
			break;  // 스위치문을 빠져 나감
		case 3:     //현재 연결리스트를 모두 출력
			temp = list;		//처음부터 다보여주기위해 list가 가르키는 곳으로 보냄
			while (temp->next != NULL)		//마지막이 나올때까지
			{
				printf("번호: %d\t", temp->serial_num);   //출력
				printf("제목 :%s\t", temp->title);
				printf("저자 :%s\t ", temp->author);
				printf("가격 :%s\t\n", temp->price);
				temp = temp->next;   //다음공간으로 넘어감. 다음 공간을 출력하기 위함
			}
			break;  //스위치문을 빠져 나감
		case 4:   //나가기
			free_list(list);    //재귀함수를 이용하여 메모리 할당해제
			end = 10; //반복문을 종료 시키기 위함
			break; //스위치문 빠져나감
		default:    //오류. 사용자에 잘못된 입력
			printf("잘못 되었습니다.\n");
			break;
		}
		if (end == 10)  //반복문 종료
			break;
	}
	return 0;  //프로그램 종료
}

BOOK *delete(BOOK *list, int sern) {     //삭제함수
	BOOK *current;          //데이터를 삭제하기 위해 필요함
	BOOK *follow;			//current를 따라가면서 삭제위치가 나오면 안따라가서 삭제하게 만듬
	current = follow = list;   //list와 같은곳을 가르키게 함

	while ((current != NULL) && (current->serial_num != sern))  // 삭제위치찾기
	{
		follow = current;			//같은곳을 가르킨다면
		current = current->next;   //넘어감
	}
	if (current == NULL)				//데이터가 없다면
	{
		printf("'%c' is not found...\n", sern);
		return list;
	}
	if (list == current)  // 가장 앞이 삭제할 책일시
	{
		list = current->next;
	}
	else if (current->next == NULL)  //가장 마지막이 삭제할 책일시
	{
		follow->next = NULL;
	}
	else  //중간이 삭제할 책일시
	{
		follow->next = current->next;  //current가 삭제할 대상이 되어 링크드에서 뺌
	}
	free(current);  //삭제할 대상이된 노드는 메모리 삭제
	return list;

}


void free_list(BOOK *head) { //종료 재귀함수
	if (head != NULL) {
		free_list(head->next);
		free(head);
	}
}
