#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int up = uptime();
    printf(1, "Uptime: %d ticks\n", up);
    exit();
}