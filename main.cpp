#include <iostream>
#include <sys/mman.h>
#include <cstring>

using namespace std;

uint8_t disassembled[] = {0x55, 0x48, 0x89, 0xe5, 0x48, 0x89, 0x7d, 0xe8, 0x48,
                          0x89, 0x75, 0xe0, 0x48, 0xb8, 0xff, 0xff, 0xff, 0xff,
                          0x7, 0x0, 0x0, 0x0, 0x48, 0x89, 0x45, 0xf8, 0x48, 0xc7,
                          0x45, 0xf0, 0x1, 0x0, 0x0, 0x0, 0x48, 0x83, 0x7d, 0xe0,
                          0x0, 0x7e, 0x38, 0x48, 0x8b, 0x45, 0xe0, 0x83, 0xe0, 0x1,
                          0x48, 0x85, 0xc0, 0x74, 0x13, 0x48, 0x8b, 0x45, 0xf0, 0x48,
                          0xf, 0xaf, 0x45, 0xe8, 0x48, 0x99, 0x48, 0xf7, 0x7d, 0xf8,
                          0x48, 0x89, 0x55, 0xf0, 0x48, 0x8b, 0x45, 0xe8, 0x48, 0xf,
                          0xaf, 0x45, 0xe8, 0x48, 0x99, 0x48, 0xf7, 0x7d, 0xf8, 0x48,
                          0x89, 0x55, 0xe8, 0x48, 0xd1, 0x7d, 0xe0, 0xeb, 0xc1, 0x48,
                          0x8b, 0x45, 0xf0, 0x5d, 0xc3};

void modify(int64_t mod) {
    for (int i = 14; i < 22; i++) {
        disassembled[i] = mod & 255;
        mod >>= 8;
    }
}

void print_error(string message) {
    cerr << message << ": " << strerror(errno) << '\n';
}

int main() {
    int64_t a, b, mod;
    cin >> a >> b >> mod;
    modify(mod);
    auto ptr = mmap(nullptr, sizeof(disassembled), PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        print_error("Couldn't allocate memory");
    } else {
        if (mprotect(ptr, sizeof(disassembled), PROT_EXEC | PROT_WRITE) == -1) {
            print_error("Couldn't set execution permission");
        } else {
            memcpy(ptr, disassembled, sizeof(disassembled));

            typedef int64_t (*function)(int64_t, int64_t);
            auto power = reinterpret_cast<function >(ptr);

            cout << power(a, b);

            if (munmap(ptr, sizeof(disassembled)) == -1) {
                print_error("Couldn't deallocate memory");
            }
        }
    }
    return 0;
}