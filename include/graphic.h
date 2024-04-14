#ifndef SK_UTILS_GRAPHIC_H
#define SK_UTILS_GRAPHIC_H

/// Mark: Basic Graphic Interface
#include <string_view>
#include <vector>

namespace sk::graphic::terminal {
struct Position {
    int x;
    int y;
};

struct Shape {
    int row;
    int column;
};

using Pos_t               = Position;
using Shape_t             = Shape;
using Default_Frame_Row_t = std::vector<char>;
using Frame_t             = std::vector<Default_Frame_Row_t>;
using Str_Frame_t         = std::vector<std::string_view>;
using Digit_Frame_t       = std::vector<std::vector<int>>;

using Const_Pos_t         = const Pos_t &;
using Const_Shape_t       = const Shape_t &;
using Const_Frame_t       = const Frame_t &;
using Const_Str_Frame_t   = const Str_Frame_t &;
using Const_Digit_Frame_t = const Digit_Frame_t &;

void setCursorPos(Const_Pos_t pos);
void printCharAtPos(Const_Pos_t pos, char c);
void printStringAtPos(Const_Pos_t pos, const std::string_view &str);
void clearScreen();
void resetCursorPos(int skippedLine = 0);

void registerCtrlC();

Frame_t makeFrame(Const_Shape_t shape, char c);
Frame_t makeFrame(Const_Shape_t shape, std::string_view str);
Frame_t makeFrame(Const_Digit_Frame_t digit_frame);
Frame_t makeFrame(Const_Shape_t shape, const std::vector<std::string_view> &frame);

class Frame {
private:
    Shape_t shape_;
    Frame_t currImg_;
    Frame_t nextImg_;
    double  fps_;
    char    default_char_;

    void sleep() const;

public:
    explicit Frame(Shape_t shape, double fps = 60, char default_char = ' ');
    ~Frame();
    void show() const;
    void refresh(Frame_t newFrame);

    [[nodiscard]] Const_Shape_t shape() const;

    [[nodiscard]] Frame_t makeFrame(char c) const;
    [[nodiscard]] Frame_t makeFrame(Const_Str_Frame_t frame) const;
    [[nodiscard]] Frame_t makeFrame(std::string_view str) const;
    [[nodiscard]] Frame_t makeFrame(Const_Digit_Frame_t digit_frame) const;
};
}  // namespace sk::graphic::terminal

#endif  // SK_UTILS_GRAPHIC_H