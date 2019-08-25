#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>

using namespace std;

static const size_t NUM_TOKENS_OF_PROC_MAPS = 6;

struct Section {
    unsigned long start;
    unsigned long size;
};

static vector<string>  decompose(const string &line, const char separator=' ')
{
    vector<string> v;
    stringstream stream(line);
    string token;
    while (getline(stream, token, separator)) {
        if (token.empty())
            continue;
        v.emplace_back(token);
    }
    return v;
}

static string getLastComponent(const string &s, const char separator)
{
    const auto words = decompose(s, separator);
    return *words.rbegin();
}


static Section getTextSection()
{
    const auto filename = "/proc/self/maps";
    ifstream ifs(filename);
    if (ifs.fail()) {
        cerr << "Failed to open: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    // Get the first line
    string line;
    getline(ifs, line);

    const auto tokens = decompose(line);
    if (tokens.size() != NUM_TOKENS_OF_PROC_MAPS) {
        cerr << "Unexpected number of tokens (/proc/self/maps): " << tokens.size()
             << " :" << line << endl;
        exit(EXIT_FAILURE);
    }

    const auto programName = getLastComponent(tokens[NUM_TOKENS_OF_PROC_MAPS-1], '/');
    if (programName != "pages") {
        cerr << "Unexpected program name: " << programName << endl;
        exit(EXIT_FAILURE);
    }

    const auto &addressString = tokens[0];
    const auto addressRange = decompose(addressString, '-');
    if (addressRange.size() != 2) {
        cerr << "Unexpected token (address range): " << addressRange.size() << " :"
             << addressString << endl;
        exit(EXIT_FAILURE);
    }

    const auto &permission = tokens[1];
    if (permission != "r-xp") {
        cerr << "Unexpected permission: " << permission  << endl;
        exit(EXIT_FAILURE);
    }

    Section section;
    cout << "start: " << addressRange[0] << endl;
    cout << "end  : " << addressRange[1] << endl;
    section.start = strtol(addressRange[0].c_str(), NULL, 16);
    section.size = strtol(addressRange[1].c_str(), NULL, 16) - section.start;

    return section;
}

static unsigned long sum(const Section section)
{
    unsigned long a = 0;
    unsigned char *p = reinterpret_cast<unsigned char *>(section.start);
    for (unsigned long i = 0; i < section.size; i++)
        a += p[i];
    return a;
}

int main(void)
{
    const auto textSection = getTextSection();
    cout << "Text section: addr.: " << hex << textSection.start << ", size: "
         << textSection.size << endl;
    cout << "Call function after 10 sec." << endl;
    sleep(10);

    cout << "sum: " << sum(textSection) << endl;

    cout << "Sleep 10 sec." << endl;
    sleep(10);
    return EXIT_SUCCESS;
}
