// Menu02.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Menu02.h"
#include <commdlg.h>
#include <stdio.h>
#include <clocale>

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
    LoadStringW(hInstance, IDC_MENU02, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENU02));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MENU02));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MENU02);
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
// 열기대화상자를 이용해서 파일 불러오기
TCHAR str1[10][100];
int line=0, count = 0;;
int i;
SIZE size;
void SaveFile(TCHAR filename[]) {
    FILE* fPtr;
    int i;
    _tsetlocale(LC_ALL, _T("korean"));
    _tfopen_s(&fPtr, filename, _T("w"));

    for (i = 0; i <= line; i++) {
        _fputts(str1[i], fPtr);
        _fputts(_T("\n"), fPtr);
    }
    fclose(fPtr);
}
void OutFromFile(TCHAR filename[], HWND hWnd) {
    FILE* fPtr;
    HDC hdc;
    int line;
    TCHAR buf[500];

    line = 0;
    hdc = GetDC(hWnd);
    // 유니코드 ,,, 멀티바이트
#ifdef _UNICODE         // 문자집합이 유니코이다면
    _tfopen_s(&fPtr, filename, _T("r, ccs=UNICODE"));
#else
    _tfopen_s(&fPtr, filename, _T("r"));
#endif
    // 파일로부터 데이터 읽어.......파일에끝
    while (_fgetts(buf, 100, fPtr) != NULL) {
        if (buf[_tcslen(buf) - 1] == _T('\n') )     /// 줄 바꿈이면
            buf[_tcslen(buf) - 1] = NULL;
        TextOut(hdc, 0, line * 20, buf, _tcslen(buf));
        line++;
    }
    fclose(fPtr);
    ReleaseDC(hWnd, hdc);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CHOOSEFONT FONT;
    static HFONT hFont, oldFont;
    static LOGFONT LogFont;

    OPENFILENAME ofn;
    OPENFILENAME sfn;
    TCHAR filter[] =
        _T("API 소스와 헤더파일 \0*.cpp\0Every File(*.*)\0*.*\0");
    static TCHAR str[100], lpstrFile[100] = _T("");

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_FONT:
                memset(&FONT, 0, sizeof(CHOOSEFONT));
                FONT.lStructSize = sizeof(CHOOSEFONT);
                FONT.hwndOwner = hWnd;
                FONT.lpLogFont = &LogFont;
                FONT.Flags = CF_EFFECTS | CF_SCREENFONTS;
                if (ChooseFont(&FONT) != 0) {
                    hFont = CreateFontIndirect(&LogFont);
                    InvalidateRgn(hWnd, NULL, TRUE);
                }
                break;
            case IDM_FILESAVE:
                memset(&sfn, 0, sizeof(OPENFILENAME));
                sfn.lStructSize = sizeof(OPENFILENAME);
                sfn.hwndOwner = hWnd;
                sfn.lpstrFilter = filter;
                sfn.lpstrFile = lpstrFile;
                sfn.nMaxFile = 256;
                sfn.lpstrInitialDir = _T("d:\\bluebird\\api\\");
                sfn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
                if (GetSaveFileName(&sfn) != 0) {
                    //_stprintf_s(str, _T("%s, 파일을 저장하겠 습니까?"), sfn.lpstrFile);
                    //MessageBox(hWnd, str, _T("저장하기"), MB_OK);
                    //InvalidateRgn(hWnd, NULL, TRUE);
                    SaveFile(sfn.lpstrFile);
                }
                break;
            case IDM_FILEOPEN:
                memset(&ofn,0, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = filter;
                ofn.lpstrFile = lpstrFile;
                ofn.nMaxFile = 100;
                ofn.lpstrInitialDir = _T("d:\\bluebird\\api\\");
                ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
                if (GetOpenFileName(&ofn) != 0) {
                    //_stprintf_s(str, _T("%s, 파일을 열겠습니까?"), ofn.lpstrFile);
                    //MessageBox(hWnd, str, _T("열기 선택"), MB_OK);
                    //InvalidateRgn(hWnd, NULL, TRUE);
                    OutFromFile(ofn.lpstrFile, hWnd);
                }

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
        line = 0;
        count = 0;
        // 캐럿 생성
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;
    case WM_CHAR:
        if (wParam == VK_BACK) count--;
        else if (wParam == VK_RETURN) {
            line++;
            count = 0;
        }
        else {
            str1[line][count++] = wParam;
        }
        str1[line][count] = NULL;
        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            oldFont = (HFONT)SelectObject(hdc, hFont);
            for (i = 0; i < line; i++)
                TextOut(hdc, 0, i * 20, str1[i], _tcslen(str1[i]));
            GetTextExtentPoint(hdc, str1[line], _tcslen(str1[line]), &size);
            TextOut(hdc, 0, line * 20, str1[line], _tcslen(str1[line]));
            SetCaretPos(size.cx, line * 20);
            SelectObject(hdc, oldFont);
            DeleteObject(hFont);
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
