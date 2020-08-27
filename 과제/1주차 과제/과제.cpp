#include <Windows.h>

LRESULT CALLBACK WndMsgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstace, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wnd;

	static LPCTSTR szAppName = "Drone";
	static LPCTSTR szTitleName = "20074008 Joo Jeong Ha";

	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndMsgProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance;
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = szAppName;

	RegisterClass(&wnd);

	hwnd = CreateWindow(
		  szAppName			// 클래스 이름
		, szTitleName		// 타이틀 이름
		, WS_EX_LAYERED		// 윈도우 스타일
		, CW_USEDEFAULT		// 위치 x 좌표
		, CW_USEDEFAULT		// 위치 y 좌표
		, CW_USEDEFAULT		// 윈도우 폭
		, CW_USEDEFAULT		// 윈도우 높이
		, NULL				// 부모 윈도우 핸들
		, NULL				// 메뉴 핸들
		, hInstance			// 응용 프로그램 ID
		, NULL				// 생성된 윈도우 정보
	);

	if (!hwnd) {
		return FALSE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	static UINT data = 0;
	static POINT MousePos;
	static bool dragCheck;
	LPSTR tMsg = (LPSTR)"배고픔 수치 : ";
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc;

	switch (uMsg) {
	case WM_MOUSEMOVE:
		TCHAR str[100];
		data++;
		_itoa_s(data, str, 100, 10);
		hdc = GetDC(hwnd);

		if (dragCheck) {
			MoveToEx(hdc, 300, 100, NULL);
			LineTo(hdc, 700, 100);
			MoveToEx(hdc, 700, 100, NULL);
			LineTo(hdc, 700, 300);
			MoveToEx(hdc, 500, 200, NULL);
			LineTo(hdc, 500, 450);
			MoveToEx(hdc, 300, 450, NULL);
			LineTo(hdc, 700, 450);
			dragCheck = false;
		}

		TextOut(hdc, 10+100, 20, str, lstrlen(str));
		TextOut(hdc, 10, 20, tMsg, lstrlen(tMsg));
		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONUP:
		dragCheck = false;
		MessageBox(hwnd, TEXT("참아"), TEXT("경고"), MB_ICONEXCLAMATION | MB_YESNO);
		break;

	case WM_LBUTTONDOWN:
		dragCheck = true;

		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("치킨 시킬까?"), strlen("치킨 시킬까?"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		ReleaseDC(hwnd, hdc);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}