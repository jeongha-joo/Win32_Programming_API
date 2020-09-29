// Menu_01.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Menu_01.h"
#include "resource.h"
#include <commdlg.h>
#include <stdio.h>
#include <locale.h>
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
    LoadStringW(hInstance, IDC_MENU01, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENU01));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MENU01));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MENU01);
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
#define     PEN         1
#define     BRUSH       2
// #define     LINE        1
// #define     ELLIPSE     2
// #define     RECTANGLE   3
enum FIGURE {LINE=1,ELLIPSE,RECTANGLE};
TCHAR buffer[60];
//도형 저장
struct SHAPE {
    int shape_type;
    int x1;
    int x2;
    int y1;
    int y2;
};

SHAPE arr_shape[500] = { 0 };

int count = 0;
// 글자입력

// 파일 저장 함수
void SaveFile(TCHAR filename[]) {
    FILE* fPtr;


    // 저장할 파일 지정
    _tfopen_s(&fPtr, filename, _T("w"));

    for (int i = 0; i < count; i++) {
        _ftprintf_s(fPtr,_T("%d,%d,%d,%d,%d,"), arr_shape[i].x1, arr_shape[i].x2, arr_shape[i].y1, arr_shape[i].y2, arr_shape[i].shape_type);
    }
    fclose(fPtr);
}

// 파일 열기 함수
void OutFromFile(TCHAR filename[], HWND hwnd) {
    FILE* fPtr;
    // 유니코드 , 멀티바이트코드
#ifdef _UNICODE // 문자 집합이 유니코드일때
    _tfopen_s(&fPtr, filename, _T("r"));
#else
    _tfopen_s(&fPtr, filename, _T("r"));
#endif

    count = 0;
    while (_ftscanf_s(fPtr, _T("%d,%d,%d,%d,%d,"), arr_shape[count].x1, arr_shape[count].x2, arr_shape[count].y1, arr_shape[count].y2, arr_shape[count].shape_type) != NULL) {
        count++;
    }
    fclose(fPtr);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    // 열기 대화상자용 선언
    OPENFILENAME ofn;

    // 저장 대화상자용 선언
    OPENFILENAME sfn;

    TCHAR lpstrfile[100] = _T("");  // 파일 패스를 저장할 버퍼
    TCHAR filter[] = _T("API 소스와 헤더\0*.cpp\0모든 파일\0*.*\0");

    // WM_PAINT 사용 변수
    int n = 0;

    static SIZE size;


    // 폰트 대화상자를 위한 선언
    CHOOSEFONT font;
    static COLORREF fColor;
    HFONT hFont, oldFont;
    static LOGFONT  logFont;


    // 도형 그리기를 위한 선언
    static BOOL bDrag;
    static int startX, startY, endX, endY, oldX, oldY;
    static int obj_mode;
    
  
    // 색상 지정대화상자를 위한 선언
    static COLORREF tmp[16], retColor;
    CHOOSECOLOR COLOR;


    // 브러쉬
    HBRUSH hBrush, oldBrush;

    // POPUP MENU를 위한 선언
    HMENU hMenu, hPopup;
    switch (message)
    {
    case WM_CONTEXTMENU:
        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_POPUP_MENU));
        hPopup = GetSubMenu(hMenu, 0);

        // 팝업 메뉴 생성
        TrackPopupMenu(hPopup, TPM_RIGHTBUTTON
            , 10, 10, 0, hWnd, NULL);
        DestroyMenu(hMenu);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_FACE_COLOR:
                for (n = 0; n < 16; n++)
                    tmp[n] = RGB(rand() % 256, rand() % 256, rand() % 256);
                memset(&font, 0, sizeof(CHOOSECOLOR));
                COLOR.lStructSize = sizeof(CHOOSECOLOR);
                COLOR.hwndOwner = hWnd;
                COLOR.lpCustColors = tmp;
                COLOR.Flags = CC_FULLOPEN;
                if (ChooseColor(&COLOR) != 0) {
                    retColor = COLOR.rgbResult;
                  //  InvalidateRgn(hWnd, NULL, TRUE);
                }
                
                break;
            case IDM_FILE_SAVE:
                memset(&sfn, 0, sizeof(OPENFILENAME));
                sfn.lStructSize = sizeof(OPENFILENAME);
                sfn.hwndOwner = hWnd;
                sfn.lpstrFilter = filter;
                sfn.lpstrInitialDir = _T("d:\\bluebird\\api\\");
                sfn.lpstrFile = lpstrfile;
                sfn.nMaxFile = 100;

                if (GetSaveFileName(&sfn) != 0)
                {
                    SaveFile(sfn.lpstrFile);
                }
                else {
                    MessageBox(hWnd, _T("파일 열기 실패!"), _T("에러")
                        , MB_OK | MB_ICONEXCLAMATION);
                }
                break;
            case IDM_FILE_OPEN:
                memset(&ofn, 0, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = filter;
                ofn.lpstrInitialDir = _T("d:\\bluebird\\api\\");
                ofn.lpstrFile = lpstrfile;
                ofn.nMaxFile = 100;

                if (GetOpenFileName(&ofn) != 0)
                {
                    OutFromFile(ofn.lpstrFile, hWnd);
                    InvalidateRgn(hWnd, NULL, TRUE);
                }
                else {
                    MessageBox(hWnd, _T("파일 열기 실패!"), _T("에러")
                        , MB_OK | MB_ICONEXCLAMATION);
                }
                break;

            case IDM_SHAPE_LINE:
                obj_mode = LINE;
                break;
            case IDM_SHAPE_ELLIPSE:
                obj_mode = ELLIPSE;
                break;
            case IDM_SHAPE_RECTANGLE:
                obj_mode = RECTANGLE;
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_LBUTTONDOWN:
        startX = endX = LOWORD(lParam);
        startY = endY = HIWORD(lParam);
        bDrag = TRUE;
        break;
    case WM_LBUTTONUP:
        bDrag = FALSE;
        if (count < 500) {
            arr_shape[count].shape_type = obj_mode;
            arr_shape[count].x1 = startX;
            arr_shape[count].x2 = endX;
            arr_shape[count].y1 = startY;
            arr_shape[count].y2 = endY;
            count += 1;
        }
        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_MOUSEMOVE: {
        HDC hdc = GetDC(hWnd);
        if (bDrag) {

            endX = LOWORD(lParam);
            endY = HIWORD(lParam);

            switch (obj_mode) {
            case LINE:
                MoveToEx(hdc, startX, startY,NULL);
                LineTo(hdc, endX, endY);
                break;
            case ELLIPSE:
                Ellipse(hdc, startX, startY, endX, endY);
                break;
            case RECTANGLE:
                Rectangle(hdc, startX, startY, endX, endY);
                break;
            }
        }
        ReleaseDC(hWnd, hdc);
    }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            for (int i = 0; i < count; i++) {
                switch (arr_shape[i].shape_type) {
                case LINE:
                    MoveToEx(hdc, arr_shape[i].x1, arr_shape[i].y1, NULL);
                    LineTo(hdc, arr_shape[i].x2, arr_shape[i].y2);
                    break;
                case ELLIPSE:
                    Ellipse(hdc, arr_shape[i].x1, arr_shape[i].y1, arr_shape[i].x2, arr_shape[i].y2);
                    break;
                case RECTANGLE:
                    Rectangle(hdc, arr_shape[i].x1, arr_shape[i].y1, arr_shape[i].x2, arr_shape[i].y2);
                    break;
                }
            }
            EndPaint(hWnd, &ps);
        }
        break;
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
