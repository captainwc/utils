#include <string_view>
#include <vector>

#include "graphic.h"

using namespace sk::graphic::terminal;  // NOLINT

int main() {
    Str_Frame_t xixi   = {"##     ## #### ##     ## ####", " ##   ##   ##   ##   ##   ## ",
                          "  ## ##    ##    ## ##    ## ", "   ###     ##     ###     ## ",
                          "  ## ##    ##    ## ##    ## ", " ##   ##   ##   ##   ##   ## ",
                          "##     ## #### ##     ## ####"};
    Str_Frame_t kaikai = {
        "##    ##    ###    #### ##    ##    ###    #### ", "##   ##    ## ##    ##  ##   ##    ## ##    ##  ",
        "##  ##    ##   ##   ##  ##  ##    ##   ##   ##  ", "#####    ##     ##  ##  #####    ##     ##  ##  ",
        "##  ##   #########  ##  ##  ##   #########  ##  ", "##   ##  ##     ##  ##  ##   ##  ##     ##  ##  ",
        "##    ## ##     ## #### ##    ## ##     ## #### "};

    Frame   win({8, 50}, 6);
    Frame_t f1 = makeFrame(win.shape(), kaikai);
    Frame_t f2 = makeFrame(win.shape(), xixi);

    for (int i = 0; i < 1000; i++) {
        if (i % 2 == 0) {
            win.refresh(f1);
        } else {
            win.refresh(f2);
        };
    }
}
