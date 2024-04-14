#include "graphic.h"

using namespace sk::graphic::terminal;  // NOLINT

int main() {
    auto xixi = "##     ## #### ##     ## ####"  // NOLINT
                " ##   ##   ##   ##   ##   ## "
                "  ## ##    ##    ## ##    ## "
                "   ###     ##     ###     ## "
                "  ## ##    ##    ## ##    ## "
                " ##   ##   ##   ##   ##   ## "
                "##     ## #### ##     ## ####";

    Frame   win({7, 29}, 10, '@');
    Frame_t f1 = makeFrame(win.shape(), "xixi");
    Frame_t f2 = makeFrame(win.shape(), "%");
    Frame_t f3 = win.makeFrame(xixi);
    Frame_t f4 = win.makeFrame("xixi");

    for (int i = 0; i < 10; i++) {
        if (i % 4 == 0) {
            win.refresh(f1);
        } else if (i % 4 == 1) {
            win.refresh(f2);
        } else if (i % 4 == 2) {
            win.refresh(f3);
        } else {
            win.refresh(f4);
        };
    }
}
