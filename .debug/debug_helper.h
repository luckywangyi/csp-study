#ifdef DEBUG
#include <cstdio>
namespace {
struct __csp_debug_init {
    __csp_debug_init() {
        FILE* f = fopen(".debug/debug_input.txt", "r");
        if (f) {
            fclose(f);
            freopen(".debug/debug_input.txt", "r", stdin);
        }
    }
} __csp_debug_init_instance;
}
#endif
