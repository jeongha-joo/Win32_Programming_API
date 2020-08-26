#include <windows.h>

// �Լ� ���� : �޽��� ó�� �Լ� ����
LRESULT CALLBACK WndMsgProc(HWND, UINT, WPARAM, LPARAM);

// WINMAIN �Լ�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;
	
	static LPCTSTR szAppName = "EasyText";		// ������ Ŭ���� �̸�
	static LPCTSTR szTitleName = "Hello API";	// Ÿ��Ʋ �ٿ� ��µ� ���ڿ�

	//1. ������ Ŭ������ ����
	// ����ü  -->  ����ü�̸�.�������
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndMsgProc;	// �޼��� ó�� �Լ� ���
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;

	//2. ������ Ŭ������ ������Ʈ���� ���
	RegisterClass(&wc);

	//3. �޸𸮿� ����
	hwnd = CreateWindow(szAppName, szTitleName, WS_EX_LAYERED
		, CW_USEDEFAULT	// X��ǥ)
		, CW_USEDEFAULT // Y��ǥ
		, CW_USEDEFAULT	// ������ ��
		, CW_USEDEFAULT	// ������ ����
		, NULL
		, NULL
		, hInstance
		, NULL			// createstruct ����ü�� ���� ���޵Ǵ� ��
	);

	// �޸𸮿� create �Ǿ����� Ȯ��
	if (!hwnd) {
		return FALSE;
	}

	//4. �޸𸮿� ������ �����츦 ȭ�鿡 �����ֱ�
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//5. �޽��� ���� -> �޽��� ť�κ��� �޽����� �о �����ؼ� �޽��� ó���Լ��� ����
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


// �Լ� ���� : �޽���ó���Լ� ����
LRESULT CALLBACK WndMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;		// �簢�� LEFT RIGHT TOP BOTTOM
	POINT MousePoint;	// point����ü x, y

	switch (uMsg) {

	case WM_CREATE:	// �����찡 ���۵� ��
		MessageBox(hwnd, TEXT("WM_CREATE"), TEXT("�����찡 �����Ҷ�"), MB_ICONEXCLAMATION);
		break;
	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, "TEST API", strlen("TEST API"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:	// Ŭ���̾�Ʈ ������ ��°��õ� ����
		// �׸��� ���� ��� ���
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 10, 10, TEXT("hello �ȳ�"), strlen("hello �ȳ�"));
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);	//WM_QUIT �޽����� �߻����� �޽��� ������ �ߴ�
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}