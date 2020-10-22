#include "iobench.hh"
bool quiet = false;
double start_tstamp;

int main(int argc, char* argv[]) {
    FILE* f = stdout;
    if (isatty(fileno(f))) {
        f = fopen(DATAFILE, "w");
    }
    if (!f) {
        perror("fopen");
        exit(1);
    }

    size_t size = 51200000;
    size_t nwrite = size;
    parse_arguments(argc, argv, &nwrite, nullptr);

    const char* buf = "6";

    size_t pos = size;
    size_t n = 0;
    while (n < nwrite) {
        pos -= 1;
        if (fseek(f, pos, SEEK_SET) == -1) {
            perror("fseek");
            exit(1);
        }
        size_t r = fwrite(buf, 1, 1, f);
        if (r != 1) {
            perror("fwrite");
            exit(1);
        }
        n += r;
        if (n % PRINT_FREQUENCY == 0) {
            report(n);
        }
    }

    fclose(f);
    report(n, true);
}
