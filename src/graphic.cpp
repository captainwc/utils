#include "graphic.h"

#include <atomic>
#include <iostream>
#include <thread>

namespace sk::graphic::terminal {
struct GlobalCtx {
    static std::atomic_int16_t maxRowPos;
};

std::atomic_int16_t GlobalCtx::maxRowPos(0);

#ifdef _WIN32
#include <windows.h>

void setCursorPos(Const_Pos_t pos) {
    if (pos.x > sk::graphic::terminal::GlobalCtx::maxRowPos.load()) {
        sk::graphic::terminal::GlobalCtx::maxRowPos.store(static_cast<SHORT>(pos.x));
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD  position = {static_cast<SHORT>(pos.y), static_cast<SHORT>(pos.x)};
    SetConsoleCursorPosition(hConsole, position);
}

void clearScreen() {
    HANDLE                     hConsole      = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD                      coordScreen   = {0, 0};
    DWORD                      cCharsWritten = 0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      dwConSize = 0;

    // 获取控制台缓冲区信息
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // 用空格填充整个控制台缓冲区
    FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

    // 将光标移动到控制台左上角
    SetConsoleCursorPosition(hConsole, coordScreen);
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        // 处理Ctrl+C信号
        case CTRL_C_EVENT:
            sk::graphic::terminal::resetCursorPos();
            exit(-1);
            return TRUE;

        // 其他信号
        default:
            return FALSE;
    }
}

void registerCtrlC() {
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
}

#else
void setCursorPos(Const_Pos_t pos) {
    TODO("Missing Linux Version Function setCursorPos(Pos)");
}

void clearScreen() {
    TODO("Missing Linux Version Function clearScreen()");
}

void registerCtrlC() {
    TODO("Missing Linux Version Function registerCtrlC()");
    exit(-1);
}
#endif

void resetCursorPos(int skippedLine) {
    sk::graphic::terminal::setCursorPos({GlobalCtx::maxRowPos.load() + skippedLine + 1, 0});
}

void printCharAtPos(Const_Pos_t pos, char c) {
    sk::graphic::terminal::setCursorPos(pos);
    putchar(c);
}

void printStringAtPos(Const_Pos_t pos, const std::string_view &str) {
    sk::graphic::terminal::setCursorPos(pos);
    std::cout << str;
}

Pos_t calPosition(Const_Shape_t shape, int x) {
    return {x / shape.column, x % shape.column};
}

Frame_t makeFrame(Const_Shape_t shape, char c) {
    return Frame_t(shape.row, Frame_Row_t(shape.column, c));
}

Frame_t makeFrame(Const_Shape_t shape, std::string_view str) {
    auto ret     = sk::graphic::terminal::makeFrame(shape, ' ');
    int  maxSize = shape.row * shape.column;
    for (int i = 0, j = 0; i < maxSize; i++, j++) {
        if (j == str.length()) {
            j = 0;
        }
        auto [x, y] = sk::graphic::terminal::calPosition(shape, i);
        ret[x][y]   = str[j];
    }
    return ret;
}

Frame::Frame(Shape_t shape, int fps) : shape_(shape), fps_(fps) {
    sk::graphic::terminal::registerCtrlC();
    currImg_ = makeFrame(' ');
    nextImg_ = makeFrame(' ');
    sk::graphic::terminal::clearScreen();
    this->show();
}

Frame::~Frame() {
    resetCursorPos();
}

Frame_t Frame::makeFrame(char c) const {
    return sk::graphic::terminal::makeFrame(this->shape_, c);
}

Frame_t Frame::makeFrame(std::string_view str) const {
    return sk::graphic::terminal::makeFrame(this->shape_, str);
}

void Frame::sleep() const {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / this->fps_));
}

Const_Shape_t Frame::shape() const {
    return this->shape_;
}

void Frame::show() const {
    for (int i = 0; i < shape_.row; i++) {
        sk::graphic::terminal::printStringAtPos({i, 0}, std::string(currImg_[i].cbegin(), currImg_[i].cend()));
    }
    sk::graphic::terminal::setCursorPos({0, 0});
    this->sleep();
}

void Frame::refresh(Frame_t newFrame) {
    currImg_ = nextImg_;
    nextImg_ = newFrame;
    show();
}

}  // namespace sk::graphic::terminal