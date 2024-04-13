#include "graphic.h"

using namespace sk::graphic::terminal;  // NOLINT

int main() {
    auto xixi = "##     ## #### ##     ## ####"
                " ##   ##   ##   ##   ##   ## "
                "  ## ##    ##    ## ##    ## "
                "   ###     ##     ###     ## "
                "  ## ##    ##    ## ##    ## "
                " ##   ##   ##   ##   ##   ## "
                "##     ## #### ##     ## ####";

    Frame   win({7, 29}, 2);
    Frame_t f1 = makeFrame(win.shape(), "=");
    Frame_t f2 = makeFrame(win.shape(), xixi);
    Frame_t f3 = win.makeFrame('$');
    Frame_t f4 = win.makeFrame('^');

    for (int i = 0; i < 10001; i++) {
        if (i % 4 == 0) {
            win.refresh(f2);
        } else if (i % 4 == 1) {
            win.refresh(f2);
        } else if (i % 4 == 2) {
            win.refresh(f2);
        } else {
            win.refresh(f2);
        };
    }
}
