#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int Base = 256;

int hashh(const string& str, int start, int end) {
    int h = 0;
    for (int i = start; i <= end; ++i) {
        h = (h * Base + str[i]);
    }
    return h;
}

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

void rabinKarp(const string& txt, const vector<string>& p, steady_clock::time_point& s, steady_clock::time_point& e) {
    int n = txt.length();
    for (const auto& pattern : p) {
        int m = pattern.length();
        int pattHash = hashh(pattern, 0, m - 1);
        int txtHash = hashh(txt, 0, m - 1);

        for (int i = 0; i <= n - m; ++i) {
            if (txtHash == pattHash) {
                int j;
                for (j = 0; j < m; ++j) {
                    if (txt[i + j] != pattern[j]) {
                        break;
                    }
                }
                if (j == m) {
                    cout << "Pattern '" << pattern << "' found at index " << i << endl;
                }
            }
            if (i < n - m) {
                txtHash = (Base * (txtHash - txt[i] * power(Base, m - 1)) + txt[i + m]);
            }
        }
    }
}

string randstring(int len) {
    string c = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string r;
    for (int i = 0; i < len; ++i) {
        r += c[rand() % c.length()];
    }
    return r;
}

int main() {
    srand(time(nullptr));
    string t = randstring(12*32);

    vector<string> p;
    for (int i = 0; i < 32; ++i) {
        string pa = randstring(rand()%6+1);
        p.push_back(pa);
        p.push_back(pa);
    }
    steady_clock::time_point s, e;

    s = steady_clock::now();
    rabinKarp(t, p, s, e);
    e = steady_clock::now();

    auto d = duration_cast<milliseconds>(e - s);

    cout << "Total real execution time: " << d.count() << " milliseconds" << endl;

    return 0;
}
