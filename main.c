#define _CRT_SECURE_NO_WARNINGS
#define LOGMAX 1000
#define UP 72//위쪽 방향키
#define DOWN 80//아래쪽 방향키
#define ENTER 13//엔터키
#define ESC 27//ESC 키
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
typedef struct login {     //로그인 관련 구조체  -  형성재
	char id[20];
	char password[20];
	char name[30];
}LOGIN;
int length; //암호화를 위한 전역 변수
int codeing(char *input, long *output);//암호화 함수
int decodeing(long *outputs, char *inputs);//복호화 함수
int menu();
int menu_switch();
void membership();
int loginsystem();
void main_menu();
//
void gotoxy(int x, int y);//좌표 함수
//


int main()
{
	system("mode con: cols=70 lines=30"); //크기
	system("color 8F");
	main_menu();

		return 0;
}

void main_menu()
{
	system("title 개인정보관리 시스템");
	FILE *fpass = fopen("membershippass.txt", "r");  // 암호화 된 패스 워드 불러옴
	LOGIN uinform, userq; //유저 정보 받는 구조체
	long sendpw[100000]; //암호화된 패스워드 받는 변수
	int i = 0; //메인 메뉴에 반복문을 빠져 나오기 위해 쓰인 변수
	int logresult = 0; //로그인함수의 반환값 저장하기 위해 쓰인 변수, 로그인 성공과 실패를 판별 한다.
	while (1) {

		system("cls");
		menu(); //회원 가입 메뉴를 보여줌.
		i = menu_switch();
		if (i == 7)
			break;
		switch (i)  //회원가입, 로그인을 위한 스위치
		{
		case 5: //회원가입
			membership();
			//membership(e,n);
			break;
		case 6: //로그인을 위한 스위치
			logresult = loginsystem();
			//logresult = loginsystem(d, n);
			if (logresult == 0)  //로그인 실패라면 i에 4를 주고 종료
				i = 7;
			else
				printf("login success\n");//로그인 성공시 밑에 메뉴로 넘어감
		}
	}
}

int menu_switch()
{
	int key;//입력받은 키의 int값을 저장할 변수
	int x = 16, y = 5;//화살표의 시작 좌표 지정 x :x축 좌표  y:y축 좌표

	while (1)
	{
		key = getch();//키를 입력 받으면
		if (key == 0xE0 || key == 0)
			key = getch();
		switch (key)
		{
		case UP: //위쪽 방향키를 입력받으면
			system("cls");
			menu(); //메뉴창을 띄우고
			y -= 1;	   //화살표("=>")의 y좌표를 1칸 아래로 내림
			if (y <= 5)  //화살표("=>")의 y좌표가 메뉴 위쪽으로는 올라가지 않도록 고정
				y = 5;
			gotoxy(x, y);
			printf("=>");//바뀐 좌표에 화살표 출력
			break;

		case DOWN://아래쪽 방향키를 입력받으면
			system("cls");
			menu();//메뉴창을 띄우고
			y += 1;	   //화살표("=>")의 y좌표를 1칸 위로 올림
			if (y >= 13)  //화살표("=>")의 y좌표가 메뉴 아래쪽으로는 내려가지 않도록 함
				y = 13;
			gotoxy(x, y);
			printf("=>");//바뀐 좌표에 화살표 출력
			break;

		case ESC:
			system("cls");
			system("color 07");
			return 0;

		case ENTER://엔터키를 입력받으면
			return y;
		}

	}

}
long mod(long m, long e, long n)         //m^e  mod  n구하는함수, 암호화 복호화를 위해 쓰임.
{
	long i, mul = 1;
	for (i = 1; i <= e; i++)
	{
		mul *= m;
		mul %= n;
	}

	return mul;
}

int codeing(char *input, long *output) //암호화 하기 위한 함수
{
	int i;
	length = strlen(input);
	for (i = 0; i < length; i++) {
		output[i] = (long)mod(input[i], 89, 7387);//공개키/랜덤으로 생성할수잇도록 함수를 만들었지만, 시간에 따라 바뀌어서 임의로 공개키 선택
	}
	output[i] = '\0';
	return 0;
}

int decodeing(long *outputs, char *inputs)  //복호화를 위한 함수
{
	int i = 0;

	inputs[0] = (char)mod(outputs[0], 3081, 7387);   //input에 복호화하는과정으로 3081,7387을 앞전과 마찬가지로 임의로 선택한 비밀키
	return 0;
}

int menu() {                        //회원 가입을 위한 메뉴
	system("title Online Book Management Security Program");  //제목 변화
	system("color 8F");
	printf("=====================================================================\n");
	printf("                           L o g i n  \n");
	printf("=====================================================================\n");
	gotoxy(18, 5);
	printf(" 1. 회 원 가 입\n");
	gotoxy(18, 6);
	printf(" 2. 로 그 인\n");
	gotoxy(18, 7);
	printf(" 3. 프로그램 종료....\n");
}

void membership() {                    //회원 가입을 위한 메뉴
	FILE *fpout, *fid, *fpw;        //파일 입출력을 위한 포인터 선언.
	LOGIN userp;                  //유저 정보를 담을 구조체
	long sendpw[20];              //입력받은 패스 워드 암호화 하고 저장할때 쓰이는 배열.
	int y;
	fpout = fopen("membershipinform.txt", "w");
	fpw = fopen("membershippass.txt", "w");

	if (fpout == NULL)
	{
		printf("파일에 쓰기를 실패하였습니다..\n");
		fclose(fpout);
		return;
	}
	printf("회원가입을 선택하셨습니다.\n");
	printf("ID를 입력하세요.\n");
	scanf("%s", userp.id);
	printf("Password를 입력하세요.\n");
	scanf("%s", userp.password);
	printf("이름을 입력하세요.\n");
	scanf("%s", userp.name);
	codeing(userp.password, sendpw);                  //입력한 패스워들 암호화 해서 sendpw에 저장
	y = 0;
	while (1) {                                            //sendpw를 파일에 저장함.
		fprintf(fpw, "%d\t", sendpw[y]);
		y++;
		if (sendpw[y] == 0) {
			break;
		}
	}
	fprintf(fpout, "%s\t%s\n", userp.id, userp.name);
	fclose(fpw);
	fclose(fpout);
}

int loginsystem() {

	FILE *fpin = fopen("membershipinform.txt", "r");   //로그인시 저장되는 데이터 파일
	FILE *fpass = fopen("membershippass.txt", "r");
	LOGIN uinform;
	int a = 0;                      //a는 로그인 할때 쓰임.
	LOGIN userq;

	long sendpw[10000]; //sendpw는 암호화된 데이터 파일을 불러올때쓰는 배열
	char recv[100] = { NULL }, buffer[1000], check[100] = { NULL }; //recv는 복호화된 데이터 저장 배열,check는 복호화된 데이터를 이어붙이는 배열
	system("cls");
	printf("=====================================================================\n");
	printf("                           L o g i n  \n");
	printf("=====================================================================\n");
	printf("ID를 입력하세요.\n");
	scanf("%s", userq.id);
	printf("Password를 입력하세요.\n");
	scanf("%s", userq.password);
	while (!feof(fpass))
	{
		fscanf(fpass, "%d\t", sendpw);
		decodeing(sendpw, recv);  //암호화된 데이터 복호화해서 recv에 저장
		strcat(check, recv);   //check배열에 복호화된 데이터 이어붙이기

	}

	while (!feof(fpin)) {  //파일이 끝날때까지 아이디,이름,힌트를 uinform구조체에 저장
		fscanf(fpin, "%s\t%s\n", uinform.id, uinform.name);
	}
	if (strcmp(check, userq.password) == 0 && strcmp(uinform.id, userq.id) == 0)  //입력한데이터와 id,PW비교
	{
		a++;
	}
	if (a == 1) {
		printf("로그인 성공!\n");
		system("pasue");
		fclose(fpin);
		return 1;
	}
	else {
		printf("입력 ID: %s\n", userq.password);
		printf("로그인 실패 ㅠ..ㅠ\n");
		system("pasue");
		fclose(fpin);
		return 0;
	}
}

void gotoxy(int x, int y) //좌표함수 콘솔창내 커서의 좌표지정
{
	COORD Pos = { x , y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
