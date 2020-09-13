// MouseControl_01.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MouseControl_01.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MOUSECONTROL01, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOUSECONTROL01));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOUSECONTROL01));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MOUSECONTROL01);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

#define BSIZE 40
float LengthPts(int x1, int x2, int y1, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

BOOL InCircle(int x, int mx, int y, int my) {
    if (LengthPts(x, mx, y, my) < BSIZE)
        return TRUE;
    else
        return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)

{
    HDC hdc;
    PAINTSTRUCT ps;
    static int x, y;
    static BOOL Selection;
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    int mx, my;     //마우스의 좌표


    srand((unsigned)time(NULL));

    int n1 = rand() % 255;  //255중에 1가지로 랜덤한 값이 설정됨
    int n2 = rand() % 255;
    int n3 = rand() % 255;

    switch (iMessage)
    {
    case WM_CREATE:
        x = 50;  y = 50;
        Selection = FALSE;
        break;

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);
        Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);   //기본적인 원위치
        hPen = CreatePen(PS_SOLID, 2, RGB(n2, n3, n1));         //원 클릭시 테두리 색
        hBrush = CreateSolidBrush(RGB(n1, n2, n3));             //원 클릭시 바탕색
        oldPen = (HPEN)SelectObject(hdc, hPen);
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        if (Selection)
            Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hPen);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        if (InCircle(x, mx, y, my)) Selection = TRUE;
        InvalidateRgn(hWnd, NULL, TRUE);
        break;

    case WM_MOUSEMOVE:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        if (Selection) {
            x = mx;
            y = my;
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;

    case WM_LBUTTONUP:
        Selection = FALSE;
        InvalidateRgn(hWnd, NULL, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
