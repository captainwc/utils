#include <chrono>
#include <thread>

#include "graphic.h"

// FIXME: 多次引用此文件，会和静态库中，定义的全局变量冲突。重复定义
// #include "macro.h"

void change_frame(sk::graphic::terminal::Frame_t& frame, int pos, char new_char) {
    auto size = frame.size();
    // ASSERT_MSG(size == frame[0].size(), "必须是方阵");
    if (pos < frame.size()) {
        for (auto j = pos; j < size - pos; j++) {
            frame[pos][j]            = new_char;
            frame[size - pos - 1][j] = new_char;
            frame[j][pos]            = new_char;
            frame[j][size - pos - 1] = new_char;
        }
    }
}

int main() {
    sk::graphic::terminal::Frame win({22, 22}, 24, '-');
    while (1) {
        auto f1 = win.makeFrame('-');
        for (int i = 8; i >= 0; i--) {
            if (i % 2 == 0) {
                change_frame(f1, i, '#');
                win.refresh(f1);
            } else {
                change_frame(f1, i, '$');
                win.refresh(f1);
            }
        }
        f1 = win.makeFrame('+');
        for (int i = 0; i <= 8; i++) {
            if (i % 2 != 0) {
                change_frame(f1, i, '#');
                win.refresh(f1);
            } else {
                change_frame(f1, i, '$');
                win.refresh(f1);
            }
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}