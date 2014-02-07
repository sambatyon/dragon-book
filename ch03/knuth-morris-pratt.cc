#include <iostream>
#include <string>
#include <vector>

int Find(std::string where, std::string what) {
    std::vector<int> table(what.size(), 0);
    auto generate_table = [&]() {
        table[0] = -1;
        int pos = 2;
        int cnd = 0;
        while (pos < static_cast<int>(what.size())) {
            if (what[pos - 1] == what[cnd]) {
                ++cnd;
                table[pos] = cnd;
                ++pos;
            } else if (cnd > 0) {
                cnd = table[cnd];
            } else {
                table[pos] = 0;
                ++pos;
            }
        }
    };
    generate_table();
    int m = 0;
    int i = 0;
    while (m + i < static_cast<int>(where.size())) {
        if (what[i] == where[m + i]) {
            if (i == static_cast<int>(what.size()) - 1)
                return m;
            ++i;
        } else {
            m += i - table[i];
            if (table[i] > -1)
                i = table[i];
            else
                i = 0;
        }
    }
    return std::string::npos;
}

int main(int argc, char **argv) {
    std::cout << Find("ABC ABCDAB ABCDABCDABDE", "ABCDABD") << '\n';
    return 0;
}
