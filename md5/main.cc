#include <iostream>
#include <fstream>
#include <vector>
#include "ArgParser.h"
#include "md5.h"
using namespace Leaf;
using namespace std;

constexpr size_t DEFAULT_CHUNK_SIZE = 0x10000;

struct Chunk {
    uint8_t *data;
    size_t  length;

    operator bool() const
    {
        return length;
    }
};

struct Args {
    string msg;
    string filename;
    ifstream *ifs;
    bool   verbose;
    size_t chunkSize;
    size_t msgReadIdx;

    vector<uint8_t> content;

    Args()
    : ifs(nullptr),
      verbose(false),
      chunkSize(DEFAULT_CHUNK_SIZE),
      msgReadIdx(0)
    {
    }

    virtual ~Args()
    {
        delete ifs;
    }

    bool setFile(const string &filename)
    {
        ifs = new ifstream(filename, ios::binary);
        if (!ifs)
            return false;
        if (!(*ifs))
            return false;
        return true;
    }

    Chunk readFile(void)
    {
        Chunk chunk;
        ifs->read((char *)content.data(), chunkSize);
        chunk.length = ifs->gcount();
        chunk.data = content.data();
        return chunk;
    }

    Chunk readMsg(void)
    {
        Chunk chunk;
        const size_t remainingSize = msg.size() - msgReadIdx;
        chunk.length = (remainingSize < chunkSize) ? remainingSize : chunkSize;
        chunk.data = (uint8_t *)&msg.data()[msgReadIdx];
        msgReadIdx += chunk.length;
        return chunk;
    }

    Chunk readChunk(void)
    {
        if (ifs)
            return readFile();
        else
            return readMsg();
    }

    void fixup(void)
    {
        if (ifs) {
            content.resize(chunkSize);
        }
    }
};

using Parser = ArgParser<Args>;

static void setupArgumentParser(Parser &parser)
{
    parser.add("-m", [](Args &priv, Parser &parser) {
        if (!parser.hasNext()) {
            parser.error("-m needs a string to be calculated");
            return;
        }
        priv.msg = parser.getNext();
    });

    parser.add("-i", [](Args &priv, Parser &parser) {
        if (!parser.hasNext()) {
            parser.error("-i needs a file name");
            return;
        }
        const string filename = parser.getNext();
        if (!priv.setFile(filename)) {
            parser.error("Failed to load: " + filename);
            return;
        }
    });

    parser.add("-c", [](Args &priv, Parser &parser) {
        if (!parser.hasNext()) {
            parser.error("-c needs a size");
            return;
        }
        priv.chunkSize = atoi(parser.getNext());
    });

    parser.add("-v", [](Args &priv, Parser &parser) {
        priv.verbose = true;
    });

    parser.setCompletionHook([](Args &priv, Parser &parser) {
        priv.fixup();
    });
}

int main(int argc, char *argv[])
{
    Parser parser;
    setupArgumentParser(parser);
    if (!parser.parse(argc, argv)) {
        cerr << parser.getErrorMessage() << endl;
        return EXIT_FAILURE;
    }
    Args &args = parser.getPrivateData();

    if (args.verbose) {
        cout << "Chunk size: " << args.chunkSize << endl;
        if (!args.filename.empty())
            cout << "File: " << args.filename << endl;
        else
            cout << "Message: " << args.msg << endl;
    }

    MD5Context ctx;
    MD5init(&ctx);
    if (args.verbose) {
        ctx.verboseMessage = [](const string &msg) {
            cout << msg;
        };
    } else {
        ctx.verboseMessage = nullptr;
    }

    while (true) {
        const Chunk chunk = args.readChunk();
        if (!chunk)
            break;
        MD5update(chunk.data, chunk.length, &ctx);
    }
    MD5finish(&ctx);
    cout << getDigest(&ctx);
    return EXIT_SUCCESS;
}
