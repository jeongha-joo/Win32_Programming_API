// menu01.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "menu01.h"
#include "resource.h"
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
#define PEN 1
#define BRUSH 2
#define LINE 1
#define ELLIPSE 2
#define RECTANGLE 3

TCHAR buffer[500][500];
int line = 0, count = 0;

void SaveFile(TCHAR filename[], HWND hWnd) {
    FILE* fptr;

    // 저장 locale 위치 지정
    _tsetlocale(LC_ALL, _T("korean"));
    _tfopen_s(&fptr, filename, _T("w"));

    for (int i = 0; i <= line; i++) {
        _fputts(buffer[i], fptr);
        _fputts(_T("\0"), fptr);
    }
    fclose(fptr);
    // 저장할 파일 지정
}

void OutFromFile(TCHAR filename[], HWND hWnd) {
    FILE* fptr;
#ifdef _UNICODE //문자집합 유니코드
    _tfopen_s(&fptr, filename, _T("r, ccs=UNICODE"));
#else
    _tfopen_s(&fptr, filename, _T("r"));
#endif

    while (_fgetts(buffer[line], 500, fptr) != NULL) {
        if (buffer[line][_tcslen(buffer[line]) - 1] == _T('\n'))
            buffer[line][_tcslen(buffer[line]) - 1] = NULL;
        line++;
    }
    fclose(fptr);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAME ofn;   //오픈대화상자
    OPENFILENAME sfn;   //저장대화상자

    TCHAR lpstrfile[100] = _T("");
    TCHAR filter[] = _T("API 소스와 헤더\0*.cpp\0모든 파일\0*.*\0");

    int n = 0;

    static SIZE size;

    // 폰트
    CHOOSEFONT font;
    static COLORREF fColor;
    HFONT hFont, oldFont;
    static LOGFONT logFont;

    // 도형 그리기
    static BOOL bDrag;
    static int startX, startY, endX, endY, oldX, oldY;
    static int obj_mode;

    HDC hdc;

    // 면 색
    static COLORREF tmp[16], retColor;
    CHOOSECOLOR COLOR;
    HBRUSH hBrush, oldBrush;

    // 팝업 선언
    HMENU hMenu, hPopup;

    switch (message)
    {
    case WM_CONTEXTMENU:
        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_POPUP_MENU));
        hPopup = GetSubMenu(hMenu, 0);

        // 팝업 메뉴 생성
        TrackPopupMenu(hPopup, TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
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
            case IDM_FACECOLOR:
                for (n = 0; n < 16; n++) {
                    tmp[n] = RGB(rand() % 256, rand() % 256, rand() % 256);
                }
                memset(&COLOR, 0, sizeof(CHOOSECOLOR));
                COLOR.lStructSize = sizeof(CHOOSECOLOR);
                COLOR.hwndOwner = hWnd;
                COLOR.lpCustColors = tmp;
                COLOR.Flags = CC_FULLOPEN;
                if (ChooseColor(&COLOR) != 0) {
                    retColor = COLOR.rgbResult;
                    //InvalidateRgn(hWnd, NULL, TRUE);
                }
                break;
            case IDM_FONT_COLOR:
                memset(&font, 0, sizeof(CHOOSEFONT));
                font.lStructSize = sizeof(CHOOSEFONT);
                font.hwndOwner = hWnd;
                font.lpLogFont = &logFont;
                font.Flags = CF_EFFECTS | CF_SCREENFONTS;
                if (ChooseFont(&font) != 0) {
                    fColor = font.rgbColors;
                    InvalidateRgn(hWnd, NULL, TRUE);
                }
                break;
            case IDM_SAVE:
                memset(&sfn, 0, sizeof(OPENFILENAME));
                sfn.lStructSize = sizeof(OPENFILENAME);
                sfn.hwndOwner = hWnd;
                sfn.lpstrInitialDir = _T("d:\\jjh\\");
                sfn.lpstrFile = lpstrfile;
                sfn.lpstrFilter = filter;
                sfn.nMaxFile = 100;

                if (GetSaveFileName(&sfn) != 0)
                {
                    SaveFile(sfn.lpstrFile, hWnd);
                }
                else
                    MessageBox(hWnd, _T("에러임"), _T("에러임"), MB_OK | MB_ICONEXCLAMATION);
                break;
            case IDM_OPEN:
                memset(&ofn, 0, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrInitialDir = _T("d:\\jjh\\");
                ofn.lpstrFile = lpstrfile;
                ofn.lpstrFilter = filter;
                ofn.nMaxFile = 100;
                
                if (GetOpenFileName(&ofn) != 0)
                {
                    OutFromFile(ofn.lpstrFile, hWnd);
                    InvalidateRgn(hWnd, NULL, FALSE);
                }
                else
                    MessageBox(hWnd, _T("에러임"), _T("에러임"), MB_OK | MB_ICONEXCLAMATION);
                    break;
            case IDM_STRAGHIT:
                obj_mode = LINE;
                break;
            case IDM_SHAPE_ELLIPSE:
                obj_mode = ELLIPSE;
                break;
            case IDM_SHAPE_RECT:
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
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;
    case WM_LBUTTONDOWN:
        startX = oldX = LOWORD(lParam);
        startY = oldY = HIWORD(lParam);
        bDrag = TRUE;
        break;
    case WM_LBUTTONUP:
        bDrag = FALSE;
        break;
    case WM_MOUSEMOVE:
        hdc = GetDC(hWnd);
        hBrush = CreateSolidBrush(retColor);
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        endX = LOWORD(lParam);
        endY = HIWORD(lParam);
        if (bDrag) {
            switch (obj_mode) {
            case LINE:
                MoveToEx(hdc, startX, startY, NULL);
                LineTo(hdc, oldX, oldY);
                MoveToEx(hdc, oldX, oldY, NULL);
                LineTo(hdc, endX, endY);
                break;
            case ELLIPSE:
                Ellipse(hdc, startX, startY, oldX, oldY);
                Ellipse(hdc, startX, startY, endX, endY);
                break;
            case RECTANGLE:
                Rectangle(hdc, startX, startY, oldX, oldY);
                Rectangle(hdc, startX, startY, endX, endY);
                break;
            }
            oldX = endX;
            oldY = endY;
        }
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
        ReleaseDC(hWnd, hdc);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            // 1) 폰트 생성
            hFont = CreateFontIndirect(&logFont);
            // 2) 폰트 지정
            oldFont = (HFONT)SelectObject(hdc, hFont);
            // 3) 폰트 색상 지정
            SetTextColor(hdc, fColor);
            for (n = 0; n < line; n++) {
                TextOut(hdc, 0, n * 20, buffer[n], _tcslen(buffer[n]));
            }
            GetTextExtentPoint(hdc, buffer[line], _tcslen(buffer[line]),&size);
            TextOut(hdc, 0, n * 20, buffer[n], _tcslen(buffer[n]));
            SetCaretPos(size.cx, n * 20);
            SelectObject(hdc, oldFont);
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CHAR:
        if (wParam == VK_BACK) count--;
        else if (wParam == VK_RETURN) {
            count = 0;
            line++;
        }
        else buffer[line][count++] = wParam;
        buffer[line][count] = NULL;
        InvalidateRgn(hWnd, NULL, TRUE);
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
