#include <bits/stdc++.h>
#include <cmath>

struct Array {
    Array(char i, long double d) {
        ch = i;
        p = d;
    }
    char ch;
    long double p;
};

void sortP(std::vector<Array> &arr, int L, int R) {
    int i = L;
    int j = R;
    long double X = arr.at(L).p;
    while (i <= j) {
        while (arr.at(i).p > X) {
            ++i;
        }
        while (arr.at(j).p < X) {
            --j;
        }
        if (i <= j) {
            std::swap(arr.at(i), arr.at(j));
            ++i;
            --j;
        }
    }
    if (L < j) {
        sortP(arr, L, j);
    }
    if (i < R) {
        sortP(arr, i, R);
    }
}

void Shannon(int n, std::vector<Array> &arr, long double *Q, int *L, int **C) {
    Q[0] = 0, L[0] = std::ceil((-std::log2(arr.at(0).p)));
    std::cout << "Q" << 0 << ") " << Q[0] << "\n";
    for (int i = 1; i < n; ++i) {
        Q[i] = Q[i - 1] + arr.at(i - 1).p;
        std::cout << "Q" << i << ") " << Q[i] << "\n";
        L[i] = std::ceil((-std::log2(arr.at(i).p)));
    }
    std::cout << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < L[i]; ++j) {
            Q[i] = Q[i] * 2;
            C[i][j] = (int) (Q[i]);
            if (Q[i] > 1)
                Q[i] -= 1;
        }
    }
}

void printArray(std::vector<Array> &arr) {
    for (auto i : arr)
        if (i.ch == ' ')
            std::cout << i.ch << "space[" << i.p << "], ";
        else
            std::cout << i.ch << "[" << i.p << "], ";
    std::cout << "\n\n";
}

void printCharacters(int L[], int **C, std::vector<Array> &arr) {
    for (int i = 0; i < arr.size(); ++i) {
        if (arr.at(i).ch == ' ')
            std::cout << "space"
                      << "[" << arr.at(i).p << "], ";
        else
            std::cout << arr.at(i).ch << "[" << arr.at(i).p << "], ";
        std::cout << "L: [" << L[i] << "], ";
        std::cout << std::setw(5);
        for (int j = 0; j < arr.size(); ++j)
            if (C[i][j] != -1)
                std::cout << C[i][j] << " ";
            else
                std::cout << " "
                          << " ";
        std::cout << std::endl;
    }
    std::cout << "\n\n";
}

std::vector<Array> init(std::string &full) {
    std::vector<long double> P;
    std::string alphabet;
    float count = 0;
    FILE *fin = fopen("testBase4.dat", "rb");
    int sym;
    long position = 0;
    if (fin == nullptr)
        std::cout << "Error\n";
    while (1) {
        sym = getc(fin);
        full.insert(position, 1, (char) sym);
        position++;
        if (sym == EOF) {
            if (feof(fin) != 0) {
                std::cout << "End of file\n";
                break;
            } else {
                std::cout << "Error of file\n";
                break;
            }
        }
    }
    fclose(fin);
    std::cout << "All: " << full.size() << "\n";
    alphabet.append(full);
    std::set<char> chars;
    alphabet.erase(
            std::remove_if(alphabet.begin(), alphabet.end(),
                           [&chars](char i) {
                               if (chars.count(i))
                                   return true;
                               chars.insert(i);
                               return false;
                           }),
            alphabet.end());
    for (char i : alphabet) {
        for (char j : full) {
            if (i == j)
                count++;
        }
        long double result = (long double) std::round(count / (long double) full.size() * 10000000000) / 10000000000;
        P.emplace_back(result);
        count = 0;
    }
    long double res = 0;
    for (auto i : P) {
        res += i;
    }
    std::cout << "Size of alphabet: " << alphabet.size() << "\n";
    std::cout << "Veroyat: " << res << "\n";
    std::vector<Array> vec;
    vec.reserve(alphabet.length());
    for (int i = 0; i < alphabet.length(); ++i)
        vec.emplace_back(alphabet[i], P[i]);
    return vec;
}

void encodingFile(std::vector<Array> &alphabet, int **C, std::string &full) {
    std::map<char, std::string> map;
    for (int i = 0; i < alphabet.size(); ++i) {
        std::string s;
        for (int v = 0; v < alphabet.size(); ++v)
            if (C[i][v] != -1)
                s.append(std::to_string(C[i][v]));
        map[alphabet.at(i).ch] = s;
        s.clear();
    }
    std::ofstream file;
    file.open("encoded.txt");
    if (!file.is_open()) {
        std::cout << "Couldn't open file";
    } else {
        std::string str;
        for (int i = 0; i < full.size(); ++i) {
            str.insert(i, 1, full.at(i));
        }
        for (char ch : str)
            for (auto &it : map)
                if (ch == it.first)
                    file << it.second;
        file.close();
    }
}

long filesize(FILE *fp) {
    long save_pos = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    long size_of_file = ftell(fp);
    fseek(fp, save_pos, SEEK_SET);
    return size_of_file;
}

long koef() {
    std::ifstream file("encoded.txt");
    std::stringstream ss;
    ss << file.rdbuf();
    std::string bin = ss.str();
    file.close();
    long count = 0;
    for (long i = 0; i < bin.size(); ++i) {
        if (i % 8 == 0)
            count++;
        if (i == bin.size() - 1 and count < 8)
            count += 8 - count;
    }
    return count;
}

int main() {
    std::string full;
    std::vector<Array> alphabet = init(full);
    sortP(alphabet, 0, (int) alphabet.size() - 1);
    printArray(alphabet);
    long double Q[alphabet.size()];
    int L[alphabet.size()];
    int **C = new int *[alphabet.size()];
    for (int i = 0; i < alphabet.size(); ++i)
        C[i] = new int[alphabet.size()];
    for (int i = 0; i < alphabet.size(); ++i)
        for (int j = 0; j < alphabet.size(); ++j)
            C[i][j] = -1;
    Shannon(alphabet.size(), alphabet, Q, L, C);
    printCharacters(L, C, alphabet);
    long double sd = 0, enter = 0;
    for (int i = 0; i < alphabet.size(); ++i) {
        sd += alphabet.at(i).p * (double) L[i];
        enter += alphabet.at(i).p * (-std::log2(alphabet.at(i).p));
    }
    std::cout << "Teor. Shennon's koef: " << sd / 8 * 100 << std::endl;
    std::cout << "Middle height: " << sd << "\n";
    std::cout << "Entropia: " << enter << "\n";
    encodingFile(alphabet, C, full);
    for (int i = 0; i < alphabet.size(); ++i)
        delete[] C[i];
    delete[] C;
    FILE *fin = fopen("testBase4.dat", "rb");
    long file_size = filesize(fin);
    fclose(fin);
    std::cout << "Koef: " << std::round((double) koef() / file_size * 100) << "%";
}