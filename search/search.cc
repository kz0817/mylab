#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

class Args {
    string path;
    string pattern;

public:
    Args(int argc, char **argv)
    {
        for (int i = 1; i < argc; i++) {
            const string arg = argv[i];
            if (i == 1)
                path = arg;
            else if (i == 2)
                pattern = arg;
        }
    }

    bool hasError(void) const
    {
        if (path.empty())
            return true;
        if (pattern.empty())
            return true;
        return false;
    }

    const string &getPath(void) const
    {
        return path;
    }

    const string &getPattern(void) const
    {
        return pattern;
    }
};

class ShiftBuffer {
    size_t size;
    vector<uint8_t> buf;
public:
    ShiftBuffer(const size_t len)
    : size(len),
      buf(len)
    {
        for (size_t i = 0; i < len; i++)
            buf[i] = 0;
    }

    void push(const int v) {
        for (size_t i = 0; i < size-1; i++)
            buf[i] = buf[i+1];
        buf[size-1] = v;
    }

    bool operator ==(const string &rhs) const
    {
        return memcmp(buf.data(), rhs.data(), size) == 0;
    }
};

static bool search(const Args &args)
{
    ifstream ifs(args.getPath(), ios::binary);
    if (!ifs) {
        cerr << "Failed to open: " << args.getPath() << endl;
        return false;
    }

    const auto &pattern = args.getPattern();
    const auto patternSize = pattern.size();
    ShiftBuffer buf(patternSize);

    int64_t offset = -patternSize + 1;
    while (true) {
        const auto ch = ifs.get();
        if (ifs.eof())
            break;
        buf.push(ch);
        if (buf == pattern) {
            cout << "Match: " << setw(16) << right << offset << "  "
                 << hex << setw(12) << right << offset << endl;
        }
        offset++;
    }

    return true;
}


int main(int argc, char *argv[])
{
    const Args args(argc, argv);
    if (args.hasError()) {
        cerr << "Failed to parse arguments." << endl;
        return EXIT_FAILURE;
    }

    cout << "Path: " << args.getPath() << endl;
    cout << "Pattern: " << args.getPattern() << endl;
    cout << "" << endl;
    cout << "* Started" << endl;
    search(args);
    cout << "* Finished" << endl;
    return EXIT_SUCCESS;
}
