#include "hexdump.hh"
#include <ctime>

union example {
    int x;
    char y;
    int z;
    char w;
};

int main() {
    example e;

    e.x = 61;
    e.y = 62;
    e.z = 63;
    e.w = 64;

    hexdump_object(e);
}
