#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <unordered_set>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int Base = 256;
const int BFSIZE = 10000; // Size of the Bloom filter

int hashh(const string& str, int start, int end) {
    int h = 0;
    for (int i = start; i <= end; ++i) {
        h = (h * Base + str[i]) % BFSIZE;
    }
    return h;
}

bitset<BFSIZE> BuildBloomFilter(const vector<string>& patterns, steady_clock::time_point& start, steady_clock::time_point& end) {
    start = steady_clock::now(); 
    bitset<BFSIZE> bloomFilter;
    for (const auto& pat : patterns) {
       int h = hashh(pat, 0, pat.size() - 1);
       bloomFilter.set(h);
    }
    end = steady_clock::now(); 
    return bloomFilter;
}

void rabinKarpWithBloomFilter(const string& txt, const vector<string>& pats, steady_clock::time_point& start, steady_clock::time_point& end) {
    bitset<BFSIZE> bloomFilter = BuildBloomFilter(pats, start, end);

    for (const auto& pat : pats) {
        int h = hashh(pat, 0, pat.size() - 1);
        if (!bloomFilter[h]) {
            continue;
        }

        for (int i = 0; i <= txt.size() - pat.size(); ++i) {
            if (hashh(txt, i, i + pat.size() - 1) == h) {
                int j;
                for (j = 0; j < pat.size(); ++j) {
                    if (txt[i + j] != pat[j]) {
                        break;
                    }
                }
                if (j == pat.size()) {
                    cout << "Pattern '" << pat << "' found at index " << i << endl;
                }
            }
        }
    }
}

string randstr(int len) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string str;
    for (int i = 0; i < len; ++i) {
        str += chars[rand() % chars.length()];
    }
    return str;
}

int main() {
    srand(time(nullptr));
    steady_clock::time_point start, end;
    start = steady_clock::now(); 
    string txt = randstr(12*32);
    vector<string> pats;
    for (int i = 0; i < 32; ++i) {
        string pat = randstr(rand()%6+1); 
        pats.push_back(pat);
        pats.push_back(pat); 
    }
    rabinKarpWithBloomFilter(txt, pats, start, end);
    end = steady_clock::now();
    auto totalduration = duration_cast<milliseconds>(end - start);
    cout << "Total real execution time: " << totalduration.count() << " milliseconds" << endl;
    return 0;
}
