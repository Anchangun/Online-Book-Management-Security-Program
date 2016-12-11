#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <Windows.h>
#include <process.h>

#define BUF_SIZE 100
#define MAX_CLNT 256
typedef struct login {     //로그인 관련 구조체  -  형성재
	char id[20];
	char password[20];
	char name[30];
}LOGIN;
unsigned WINAPI HandleClient(void* arg);//쓰레드 함수
void SendMsg(char* msg, int len);//메시지 보내는 함수
void ErrorHandling(char* msg);
int length; //암호화를 위한 전역 변수
long mod(long m, long e, long n);////m^e  mod  n구하는함수, 암호화 복호화를 위해 쓰임.
int codeing(char *input, long *output);//암호화 함수
int decodeing(long *outputs, char *inputs);//복호화 함수
void membership();
int loginsystem();
int menu=0;
int clientCount = 0;
SOCKET clientSocks[MAX_CLNT];//클라이언트 소켓 보관용 배열
HANDLE hMutex;//뮤텍스

int main() {
	WSADATA wsaData;
	SOCKET serverSock, clientSock;
	SOCKADDR_IN serverAddr, clientAddr;
	int clientAddrSize;
	HANDLE hThread;

	char port[100];

	printf("Input port number : ");
	gets(port);
	/*
	if(argc!=2){
	printf("Usage : %s <port>\n",argv[0]);
	exit(1);
	}
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //윈도우 소켓을 사용하겠다는 사실을 운영체제에 전달
		ErrorHandling("WSAStartup() error!");

	hMutex = CreateMutex(NULL, FALSE, NULL);//하나의 뮤텍스를 생성한다.
	serverSock = socket(PF_INET, SOCK_STREAM, 0); //하나의 소켓을 생성한다.

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi(port));

	if (bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //생성한 소켓을 배치한다.
		ErrorHandling("bind() error");
	if (listen(serverSock, 5) == SOCKET_ERROR)//소켓을 준비상태에 둔다.
		ErrorHandling("listen() error");

	printf("listening...\n");

	while (1) {
		clientAddrSize = sizeof(clientAddr);
		clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrSize);//서버에게 전달된 클라이언트 소켓을 clientSock에 전달
		WaitForSingleObject(hMutex, INFINITE);//뮤텍스 실행
		clientSocks[clientCount++] = clientSock;//클라이언트 소켓배열에 방금 가져온 소켓 주소를 전달
		ReleaseMutex(hMutex);//뮤텍스 중지
		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClient, (void*)&clientSock, 0, NULL);//HandleClient 쓰레드 실행, clientSock을 매개변수로 전달
		printf("Connected Client IP : %s\n", inet_ntoa(clientAddr.sin_addr));
	}
	closesocket(serverSock);//생성한 소켓을 끈다.
	WSACleanup();//윈도우 소켓을 종료하겠다는 사실을 운영체제에 전달
	return 0;
}

unsigned WINAPI HandleClient(void* arg) {
	SOCKET clientSock = *((SOCKET*)arg); //매개변수로받은 클라이언트 소켓을 전달
	int strLen = 0, i;
	char msg[BUF_SIZE];
	FILE *fpout, *fid, *fpw;        //파일 입출력을 위한 포인터 선언.
	LOGIN userp;                  //유저 정보를 담을 구조체
	long sendpw[20];              //입력받은 패스 워드 암호화 하고 저장할때 쓰이는 배열.
	int y;
	fpout = fopen("membershipinform.txt", "w");
	SendMsg("                          B o o k m a n a g e m e n t\n=====================================================================\n                           L o g i n  \n=====================================================================\n                         1. 회 원 가 입\n                         2. 로 그 인\n                         3. 프로그램 종료....\n", 357);
	while ((strLen = recv(clientSock, msg, sizeof(msg), 0)) != 0) { //클라이언트로부터 메시지를 받을때까지 기다린다.
		if (!strcmp(msg, "3")) {
			send(clientSock, "3", 1, 0);
			menu = 3;
			break;
		}
		if (!strcmp(msg, "1")) {
			SendMsg("회원가입을 선택하셨습니다.\n", 50);
			SendMsg("ID를 입력하세요.\n", 50);
			menu = 1;
			break;
		}
		if (!strcmp(msg, "2")) {
			SendMsg("로그인을 선택하셨습니다.\n", 50);
			SendMsg("ID를 입력하세요\n", 50);
			menu = 2;
			break;
		}
		SendMsg(msg, strLen);//SendMsg에 받은 메시지를 전달한다.
	}
	while ((strLen = recv(clientSock, msg, sizeof(msg), 0)) != 0) { //클라이언트로부터 메시지를 받을때까지 기다린다.
		if (menu == 3)
			break;
		if (menu == 1) {
			fpw = fopen(strcat(msg, "password.txt"), "w");
			fprintf(fpout,"%s\n",msg);
			SendMsg("PW를 입력하세요.",50);
			menu = 4;
			break;
		}
		if (menu == 2) {
			SendMsg("pw를 입력하세요.", 50);
			menu = 5;
			break;
		}
		SendMsg(msg, strLen);//SendMsg에 받은 메시지를 전달한다.
	}
	while ((strLen = recv(clientSock, msg, sizeof(msg), 0)) != 0) {
		if (menu == 4) {
			fprintf(fpw,"%s\n",msg);

		}
		if (menu == 5) {


		}
		printf("login member success!\n");
		break;
	}
    printf("client left the chat\n");
	//이 줄을 실행한다는 것은 해당 클라이언트가 나갔다는 사실임 따라서 해당 클라이언트를 배열에서 제거해줘야함
	WaitForSingleObject(hMutex, INFINITE);//뮤텍스 실행
	for (i = 0; i<clientCount; i++) {//배열의 갯수만큼
		if (clientSock == clientSocks[i]) {//만약 현재 clientSock값이 배열의 값과 같다면
			while (i++<clientCount - 1)//클라이언트 개수 만큼
				clientSocks[i] = clientSocks[i + 1];//앞으로 땡긴다.
			break;
		}
	}
	clientCount--;//클라이언트 개수 하나 감소
	ReleaseMutex(hMutex);//뮤텍스 중지
	closesocket(clientSock);//소켓을 종료한다.
	return 0;
}

void SendMsg(char* msg, int len) { //메시지를 모든 클라이언트에게 보낸다.
	int i;
	WaitForSingleObject(hMutex, INFINITE);//뮤텍스 실행
	for (i = 0; i < clientCount; i++)//클라이언트 개수만큼
		send(clientSocks[i], msg, len, 0);//클라이언트들에게 메시지를 전달한다.
	ReleaseMutex(hMutex);//뮤텍스 중지
}
void ErrorHandling(char* msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
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
void membership() {                    //회원 가입을 위한 메뉴
	FILE *fpout, *fid, *fpw;        //파일 입출력을 위한 포인터 선언.
	LOGIN userp;                  //유저 정보를 담을 구조체
	long sendpw[20];              //입력받은 패스 워드 암호화 하고 저장할때 쓰이는 배열.
	int y;
	fpout = fopen("membershipinform.txt", "w");

	if (fpout == NULL)
	{
		printf("파일에 쓰기를 실패하였습니다..\n");
		fclose(fpout);
		return;
	}
	printf("회원가입을 선택하셨습니다.\n");
	printf("ID를 입력하세요.\n");
	scanf("%s", userp.id);
	fpw = fopen(strcat(userp.id, "password.txt"), "w");
	printf("Password를 입력하세요.\n");
	scanf("%s", userp.password);
	printf("이름을 입력하세요.\n");
	scanf("%s", userp.name);
	codeing(userp.password, sendpw);                  //입력한 패스워들 암호화 해서 sendpw에 저장
	y = 0;
	while (1) {											//sendpw를 파일에 저장함.
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

	printf("ID를 입력하세요.\n");
	scanf("%s", userq.id);
	if ((fpass = fopen(strcat(userq.id, "password.txt"), "r")) == NULL) {
		printf("ID가 틀립니다.\n");
		return 0;
	}
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

		fclose(fpin);
		return 1;
	}
	else {
		printf("입력 ID: %s\n", userq.password);
		printf("로그인 실패 ㅠ..ㅠ\n");

		fclose(fpin);
		return 0;
	}
}
