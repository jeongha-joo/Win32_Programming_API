#include <windows.h>

// 함수 선언 : 메시지 처리 함수 선언
LRESULT CALLBACK WndMsgProc(HWND, UINT, WPARAM, LPARAM);

// WINMAIN 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;

	//1. 윈도우 클래스를 설정
	// 구조체  -->  구조체이름.멤버변수
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndMsgProc;	// 메세지 처리 함수 등록
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("FIRST API");
	
	//2. 윈도우 클래스를 레지스트리에 등록
	RegisterClass(&wc);

	//3. 메모리에 생성
	hwnd = CreateWindow(TEXT("FIRST API"), TEXT("FIRST API START"), WS_EX_LAYERED
		, CW_USEDEFAULT	// X좌표)
		, CW_USEDEFAULT // Y좌표
		, CW_USEDEFAULT	// 윈도우 폭
		, CW_USEDEFAULT	// 윈도우 높이
		, NULL
		, NULL
		, hInstance
		, NULL			// createstruct 구조체를 통해 전달되는 값
	);

	// 메모리에 create 되었는지 확인
	if (!hwnd) {
		return FALSE;
	}

	//4. 메모리에 생성된 윈도우를 화면에 보여주기
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//5. 메시지 루프 -> 메시지 큐로부터 메시지를 읽어서 번역해서 메시지 처리함수로 전달
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


// 함수 정의 : 메시지처리함수 정의
LRESULT CALLBACK WndMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("I'm hungry"), TEXT("I'm hungry"), MB_ICONEXCLAMATION);
		break;
	case WM_PAINT:	// 클라이언트 영역에 출력관련된 역할
		// 그리기 도구 얻는 방법
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 10, 10, TEXT("hello 안녕"), strlen("hello 안녕")-2);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);	//WM_QUIT 메시지를 발생시켜 메시지 루프를 중단
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}