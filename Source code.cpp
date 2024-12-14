#include <windows.h>
#include <windowsx.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#define _USE_MATH_DEFINES 1
#include <cmath>
#define M_PI 3.14159265358979323846264338327950288
#pragma comment(lib, "msimg32.lib")

INT sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);

COLORREF RndRGB() {
    int clr = rand() % 5;
    if (clr == 0) return RGB(255, 0, 0); if (clr == 1) return RGB(0, 255, 0); if (clr == 2) return RGB(0, 0, 255); if (clr == 3) return RGB(255, 0, 255); if (clr == 4) return RGB(255, 255, 0);
}

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE unused;
    };

}_RGBQUAD, * PRGBQUAD;

DWORD WINAPI textdraw(LPVOID lpParam)
{ //textdraw from gergfgfggv
    HDC hdc = GetDC(0);
    POINT cur;
    int sx = GetCursorPos(&cur);
    int sy = GetCursorPos(&cur);
    LPCWSTR text = L"Zygosporangium";
    while (true)
    {
        hdc = GetDC(0);
        GetCursorPos(&cur);
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        TextOutW(hdc, cur.x, cur.y, text, wcslen(text));
        ReleaseDC(GetDesktopWindow(), hdc);
        DeleteDC(hdc);
        Sleep(10);
    }
}

DWORD WINAPI thing(LPVOID lpParam)
{ //mythlasathome
    srand(time(0) + GetCurrentThreadId()); // Added thread ID to seed generation
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);
        POINT pt[3];
        int inc3 = (rand() % 700) + (GetCurrentThreadId() % 100); // Added thread ID to inc3 calculation
        int v = (rand() % 2) + (GetCurrentThreadId() % 3); // Added thread ID to v calculation
        if (v == 1) inc3 = -inc3;
        pt[0].x = rect.left + inc3;
        pt[0].y = rect.top + inc3;
        pt[1].x = rect.right + inc3;
        pt[1].y = rect.top * inc3;
        pt[2].x = rect.left + inc3;
        pt[2].y = rect.bottom + inc3;
        PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        SelectObject(hdc, CreateSolidBrush(RndRGB()));
        BitBlt(hdc, rand() % 20, rand() % 20, sw, sh, hdcMem, rand() % 20, rand() % 20, 0x123456);
        DeleteObject(hdcMem);
        DeleteObject(bm);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}

DWORD WINAPI mash(LPVOID lpParam)
{ //masher but its SRCAND & SRCPAINT combined
    srand((unsigned int)time(NULL));
    while (1)
    {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, w, h);
        HDC hdcTemp = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcTemp, hbm);
        BitBlt(hdcTemp, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        int numShifts = 600;
        for (int i = 0; i < numShifts; i++)
        {
            int x = rand() % w;
            int y = rand() % h;
            int dx = (rand() % 3) - 1;
            int dy = (rand() % 3) - 1;
            BitBlt(hdcTemp, x + dx, y + dy, w - x, h - y, hdcTemp, x, y, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hdcTemp, 0, 0, !(rand() % 2) ? SRCAND : SRCPAINT);
        SelectObject(hdcTemp, hbmOld);
        DeleteDC(hdcTemp);
        DeleteObject(hbm);
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI shader1(LPVOID lpParam) { //Prgbquint's 3rd effect but its green
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; w * h > i; i++) {
            data[i].rgb = (data[i].rgb * 2) % (RGB(0, 255, 0));
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader2(LPVOID lpParam)
{ //structdestr shader but its going down
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, -30, 0, SRCCOPY);
        BitBlt(hdcdc, 0, 0, w, h, desk, w - 30, 0, SRCCOPY);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, -30, SRCCOPY);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, h - 30, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - time) > 60000)
            byte = rand() % 0xff;
        for (int i = 0; w * h > i; i++) {
            if (i % h == 0 && rand() % 100 == 0)
                v = rand() % 2;
            *((BYTE*)data + 4 * i + v) = ((BYTE*)(data + i + v))[v];
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI transparent(LPVOID lpParam)
{ //transparentblt
    HWND wnd = GetDesktopWindow();
    HDC hdcScreen = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    COLORREF TransparentBltColor = (RGB(rand() % 255, rand() % 255, rand() % 255));
    while (1) {
        for (;;) {
            TransparentBlt(hdcScreen, rand() % 255, rand() % 255, sw, sh, hdcScreen, rand() % 255, rand() % 255, rand() % 255, rand() % 255, TransparentBltColor);
        }
        ReleaseDC(wnd, hdcScreen);
        DeleteDC(hdcScreen); DeleteObject(&sw); DeleteObject(&sh);
    }
}

DWORD WINAPI mash1(LPVOID lpParam)
{ //colormasher
    while (1)
    {
        HDC hdc = GetDC(HWND_DESKTOP);
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % 5, rand() % 5, rand() % sw, rand() % sh, hdc, 0, 1, 0x1900ac010e);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI gtriangle(LPVOID lpParam)
{ //my new gtriangle
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);


    while (1)
    {
        hdc = GetDC(NULL);
        // Create an array of TRIVERTEX structures that describe
// positional and color values for each vertex.
        TRIVERTEX vertex[3];
        vertex[0].x = rand() % w;
        vertex[0].y = rand() % h;
        vertex[0].Red = 0xbb00;
        vertex[0].Green = 0xff00;
        vertex[0].Blue = 0xd000;
        vertex[0].Alpha = 0x0000;

        vertex[1].x = rand() % w;
        vertex[1].y = rand() % h;
        vertex[1].Red = 0;
        vertex[1].Green = 0;
        vertex[1].Blue = 0;
        vertex[1].Alpha = 0xff00;

        vertex[2].x = rand() % w;
        vertex[2].y = rand() % h;
        vertex[2].Red = 0xd000;
        vertex[2].Green = 0x3000;
        vertex[2].Blue = 0x9000;
        vertex[2].Alpha = 0xFFF0;

        // Create a GRADIENT_TRIANGLE structure that
        // references the TRIVERTEX vertices.
        GRADIENT_TRIANGLE gTriangle;
        gTriangle.Vertex1 = 0;
        gTriangle.Vertex2 = 1;
        gTriangle.Vertex3 = 2;

        // Draw a shaded triangle.
        GradientFill(hdc, vertex, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
        ReleaseDC(0, hdc);
        Sleep(1);
    }

    return 0x00;
}

DWORD WINAPI invert(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    while (true)
    {
        hdc = GetDC(0);
        PatBlt(hdc, 0, 0, rand() % x, rand() % y, PATINVERT);
        ReleaseDC(GetDesktopWindow(), hdc);
        DeleteDC(hdc);
        Sleep(10);
    }
}

DWORD WINAPI shaking(LPVOID lpParam)
{
    HDC hdc;
    int sw, sh;
    double angle = 0.0;
    int xadd, yadd; // x angle and y angle
    while (1) {
        hdc = GetDC(NULL);
        sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
        xadd = cos(angle) * 10, yadd = sin(angle) * 10;
        BitBlt(hdc, xadd, yadd, sw, sh, hdc, 0, 0, SRCCOPY);
        angle += M_PI / 8;
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

DWORD WINAPI gtriangle1(LPVOID lpParam)
{
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);


    while (1)
    {
        hdc = GetDC(NULL);
        // Create an array of TRIVERTEX structures that describe
// positional and color values for each vertex.
        TRIVERTEX vertex[3];
        vertex[0].x = rand() % w;
        vertex[0].y = rand() % h;
        vertex[0].Red = 0x9000;
        vertex[0].Green = 0;
        vertex[0].Blue = 0x9000;
        vertex[0].Alpha = 0;

        vertex[1].x = rand() % w;
        vertex[1].y = rand() % h;
        vertex[1].Red = 0xFFF0;
        vertex[1].Green = 0xFFF0;
        vertex[1].Blue = 0xFFF0;
        vertex[1].Alpha = 0x0000;

        vertex[2].x = rand() % w;
        vertex[2].y = rand() % h;
        vertex[2].Red = 0xa000;
        vertex[2].Green = 0xb000;
        vertex[2].Blue = 0xc000;
        vertex[2].Alpha = 0xFFFF;

        // Create a GRADIENT_TRIANGLE structure that
        // references the TRIVERTEX vertices.
        GRADIENT_TRIANGLE gTriangle;
        gTriangle.Vertex1 = 0;
        gTriangle.Vertex2 = 1;
        gTriangle.Vertex3 = 2;

        // Draw a shaded triangle.
        GradientFill(hdc, vertex, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
        ReleaseDC(0, hdc);
        Sleep(1);
    }

    return 0x00;
}

DWORD WINAPI rndblt(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH brush = CreateSolidBrush(RndRGB());
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI pie(LPVOID lpParam)
{ //credits to VennyTech
    HDC hdc;
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    HBRUSH hbsh;
    int sx = 1, sy = 1, sx1 = 1, sy1 = 1;
    int incrementation = 10;
    int x = 10, y = 10;
    while (1) {
        hdc = GetDC(0);
        x += incrementation * sx;
        y += incrementation * sy;
        hbsh = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, hbsh);
        Pie(hdc, x, y, x + 100, y + 100, rand() % w, rand() % h, 30, 30);
        if (y >= GetSystemMetrics(1)) {
            sy = -1;
        }
        if (x >= GetSystemMetrics(0)) {
            sx = -1;
        }
        if (y == 0) {
            sy = 1;
        }
        if (x == 0) {
            sx = 1;
        }
        DeleteObject(hbsh);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI icons(LPVOID lpParam)
{ //error from trihydridoarsenic but it has a lot of icons
    HDC hDc = GetWindowDC(GetDesktopWindow());
    int x = rand() % GetSystemMetrics(SM_CXSCREEN);
    int y = rand() % GetSystemMetrics(SM_CYSCREEN);
    while (true) {
        HDC hDc = GetWindowDC(GetDesktopWindow());
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hDc, x, y, LoadIcon(NULL, MAKEINTRESOURCE(32512 + (rand() % 7))));
        ReleaseDC(0, hDc);
    }
}

DWORD WINAPI swirl(LPVOID lpParam)
{ //swirl from trihydridoarsenic but faster
    HDC hdc = GetDC(0);
    int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh / 10, ySize = 9;
    while (1) {
        hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, screenshot);
        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < sh * 2; i++) {
            int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
            BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
        }
        for (int i = 0; i < sw * 2; i++) {
            int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
            BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
    }
}

DWORD WINAPI beep(LPVOID lpParam)
{ //beep sound from gadolinium
    while (1) {
        Beep(rand() % 2555, rand() % 25);
    }
}


DWORD WINAPI staticscreen(LPVOID lpParam)
{ //staticscreen from salinewin
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    PRGBQUAD rgbScreen = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
    SelectObject(hdcMem, hbmTemp);
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            rgbScreen[i].rgb = rand();
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI blue(LPVOID lpParam)
{
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    HDC hdc, hdcMem; HBITMAP hbm;
    for (int i = 0;; i++, i %= 6) {
        hdc = GetDC(0); hdcMem = CreateCompatibleDC(hdc); hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY); GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; w * h > i; i++) {
            int x = i * w + i, y = i * h + i, f = (y | y + y + -1 + x + w / h);
            ((BYTE*)(data + i))[4] = (f / 1);
        }
        SetBitmapBits(hbm, w * h * 4, data); BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCERASE);
        DeleteObject(hbm); DeleteObject(hdcMem);
        DeleteObject(hdc);
        Sleep(10);
    }
}

DWORD WINAPI gtriangle2(LPVOID lpParam)
{
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);


    while (1)
    {
        hdc = GetDC(NULL);
        // Create an array of TRIVERTEX structures that describe
// positional and color values for each vertex.
        TRIVERTEX vertex[3];
        vertex[0].x = rand() % w;
        vertex[0].y = rand() % h;
        vertex[0].Red = 0xBBB0;
        vertex[0].Green = 0xFFF0;
        vertex[0].Blue = 0x0000;
        vertex[0].Alpha = 0;

        vertex[1].x = rand() % w;
        vertex[1].y = rand() % h;
        vertex[1].Red = 0;
        vertex[1].Green = 0;
        vertex[1].Blue = 0;
        vertex[1].Alpha = 0;

        vertex[2].x = rand() % w;
        vertex[2].y = rand() % h;
        vertex[2].Red = 0x0000;
        vertex[2].Green = 0xBBB0;
        vertex[2].Blue = 0xd000;
        vertex[2].Alpha = 0xff00;

        // Create a GRADIENT_TRIANGLE structure that
        // references the TRIVERTEX vertices.
        GRADIENT_TRIANGLE gTriangle;
        gTriangle.Vertex1 = 0;
        gTriangle.Vertex2 = 1;
        gTriangle.Vertex3 = 2;

        // Draw a shaded triangle.
        GradientFill(hdc, vertex, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
        ReleaseDC(0, hdc);
        Sleep(1);
    }

    return 0x00;
}

VOID WINAPI SplitBlt(HDC hcdc, HDC hcdc2, int s, int w, int h, DWORD SRC)
{
    BitBlt(hcdc, w / 2 + s, 0, w / 5, h / 2, hcdc2, w / 5, s, SRC);
    BitBlt(hcdc, w / 2 + s, h / 2 + s, w / 2, h / 2, hcdc2, w / 2, h / 2, SRC);
    BitBlt(hcdc, 0, 0, w / 2, h / 5, hcdc2, s, s, SRC);
    BitBlt(hcdc, 0, h / 2 + s, w / 2, h / 2, hcdc2, s, h / 2, SRC);
}

DWORD WINAPI splitblt(LPVOID lpParam)
{ //splitblt from dyporssions
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1)
    {
        HDC hdc = GetDC(0);
        SplitBlt(hdc, hdc, 1, w, h, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI afx1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI afx2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t << t | t >> 6 | t & 5 & 8 >> 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI afx3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 5 | (t << 3) + 12 * t * (t >> 13 | (t >> 1 | t >> 10 | t >> 2) & t >> 8));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}


VOID WINAPI afx4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 8 | t >> 9) & 46 & t >> 8 ^ (t & t >> 13 | t >> 6));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI afx5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 12) * 64 + (t >> 1) * (t >> 10) * (t >> 11) * 48 >> ((t >> 16 | t >> 17) & 1));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI afx6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & 4096) ? ((t * (t ^ t % 255) | (t >> 4)) >> 1) : (t >> 3) | ((t & 8192) ? t << 2 : t));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI afx7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((~t >> 2) * ((127 & t * (7 & t >> 10)) < (245 & t * (2 + (5 & t >> 14)))));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI afx8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> (t % 32 ? 4 : 3) | (t % 128 ? t >> 3 : t >> 3 | t >> 9));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI afx9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (0xCA98 >> (t >> 9 & 14) & 15) | t >> 8);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}




int main() {
    if (MessageBoxW(NULL, L"this malware is safety, do you want to run?", L"Zygosporangium", MB_YESNO | MB_ICONWARNING) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        // this is skidded
        if (MessageBoxW(NULL, L"are you sure, it contains flashing lights", L"Zygosporangium - Last Warning", MB_YESNO | MB_ICONWARNING) == IDNO)
        {
            ExitProcess(0);
        }
        else
        {
            Sleep(5000);
            HANDLE a = CreateThread(0, 0, thing, 0, 0, 0);
            HANDLE aa = CreateThread(0, 0, textdraw, 0, 0, 0);
            afx1();
            Sleep(30000);
            TerminateThread(a, 0);
            CloseHandle(a);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE b = CreateThread(0, 0, mash, 0, 0, 0);
            afx2();
            Sleep(30000);
            TerminateThread(b, 0);
            CloseHandle(b);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE c = CreateThread(0, 0, shader1, 0, 0, 0);
            afx3();
            Sleep(30000);
            TerminateThread(c, 0);
            CloseHandle(c);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE d = CreateThread(0, 0, shader2, 0, 0, 0);
            afx4();
            Sleep(30000);
            TerminateThread(d, 0);
            CloseHandle(d);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE e = CreateThread(0, 0, transparent, 0, 0, 0);
            HANDLE ee = CreateThread(0, 0, mash1, 0, 0, 0);
            afx5();
            Sleep(30000);
            TerminateThread(e, 0);
            TerminateThread(ee, 0);
            CloseHandle(e);
            InvalidateRect(0, 0, 0);
            CloseHandle(ee);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE f = CreateThread(0, 0, gtriangle, 0, 0, 0);
            HANDLE ff = CreateThread(0, 0, invert, 0, 0, 0);
            afx6();
            Sleep(30000);
            TerminateThread(f, 0);
            TerminateThread(ff, 0);
            CloseHandle(f);
            InvalidateRect(0, 0, 0);
            CloseHandle(ff);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE g = CreateThread(0, 0, shaking, 0, 0, 0);
            HANDLE gg = CreateThread(0, 0, gtriangle1, 0, 0, 0);
            afx7();
            Sleep(30000);
            TerminateThread(g, 0);
            TerminateThread(gg, 0);
            CloseHandle(g);
            InvalidateRect(0, 0, 0);
            CloseHandle(gg);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE h = CreateThread(0, 0, rndblt, 0, 0, 0);
            HANDLE hh = CreateThread(0, 0, pie, 0, 0, 0);
            afx8();
            Sleep(30000);
            TerminateThread(h, 0);
            TerminateThread(hh, 0);
            CloseHandle(h);
            InvalidateRect(0, 0, 0);
            CloseHandle(hh);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE i = CreateThread(0, 0, icons, 0, 0, 0);
            HANDLE ii = CreateThread(0, 0, swirl, 0, 0, 0);
            afx9();
            Sleep(30000);
            TerminateThread(i, 0);
            TerminateThread(ii, 0);
            CloseHandle(i);
            InvalidateRect(0, 0, 0);
            CloseHandle(ii);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE ini = CreateThread(0, 0, beep, 0, 0, 0);
            HANDLE j = CreateThread(0, 0, staticscreen, 0, 0, 0);
            Sleep(30000);
            TerminateThread(j, 0);
            CloseHandle(j);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE k = CreateThread(0, 0, blue, 0, 0, 0);
            HANDLE kk = CreateThread(0, 0, gtriangle2, 0, 0, 0);
            Sleep(30000);
            TerminateThread(k, 0);
            TerminateThread(kk, 0);
            CloseHandle(k);
            InvalidateRect(0, 0, 0);
            CloseHandle(kk);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            HANDLE l = CreateThread(0, 0, splitblt, 0, 0, 0);
            Sleep(30000);
            TerminateThread(l, 0);
            TerminateThread(ini, 0);
            CloseHandle(l);
            InvalidateRect(0, 0, 0);
            CloseHandle(ini);
            InvalidateRect(0, 0, 0);
            ExitProcess(0);
        }
    }
}