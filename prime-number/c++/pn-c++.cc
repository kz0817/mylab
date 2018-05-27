#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <map>

using namespace std;

template<typename T>
class ArgParser {
public:
	struct Context;

private:
	using Parser = function<void(const string &, Context &)>;

	const vector<string> args;

	vector<string> initArgs(int argc, char *argv[]) {
		vector<string> v;
		for (int i = 1; i < argc; i++)
			v.push_back(argv[i]);
		return v;
	}

	map<string, Parser> parserMap;

public:
	ArgParser(int argc, char *argv[])
	: args(initArgs(argc, argv)) {
	}

	struct Context {
		Parser nextParser;
		T      priv;

		Context()
		: nextParser(NULL)
		{}

		void reset(void)
		{
			nextParser = NULL;
		}
	};

	T parse(void) const {
		Context ctx;
		for (const auto &arg: args) {
			if (ctx.nextParser) {
				ctx.nextParser(arg, ctx);
				ctx.reset();
				continue;
			}
			auto parserIt = parserMap.find(arg);
			if (parserIt == parserMap.end()) {
				cout << "Warning: unprocessed option: " << arg << endl;
				continue;
			}
			auto parser = parserIt->second;
			ctx.reset();
			parser(arg, ctx);
		}
		return ctx.priv;
	}

	void add(const vector<string> &options, Parser parser) {
		for (const auto &opt: options)
			parserMap.insert(pair<string, Parser>(opt, parser));
	}
};

class PrimeNumber {
	vector<long> primeNumbers;

	bool isPrimeNumber(const long n)
	{
		for (const auto &primeNum: primeNumbers) {
			if (primeNum > n/2)
				return true;
			if (n % primeNum == 0)
				return false;
		}
		return true;
	}

public:
	void calc(const long upperLimit) {
		for (auto n = 2; n <= upperLimit; n++) {
			if (isPrimeNumber(n))
				primeNumbers.push_back(n);
		}
	}

	long count(void) const { return primeNumbers.size(); }
	void show(void) const {
		for (const auto pn: primeNumbers) { cout << pn << endl; }
	}

	void reserve(const long n) { primeNumbers.reserve(n); }
};

struct Param {
	long upperLimit;
	long reserve;
	bool show;

	Param()
	: upperLimit(0),
	  reserve(0),
	  show(false)
	{}
};

Param parseArg(int argc, char *argv[])
{
	using ParserContext = ArgParser<Param>::Context;
	ArgParser<Param> parser(argc, argv);

	parser.add({"-u", "--upper-limit"},
	  [](const string &arg, ParserContext &ctx) {
		ctx.nextParser = [](const string &arg, ParserContext &ctx) {
			ctx.priv.upperLimit = atol(arg.c_str());
		};
	});

	parser.add({"-r", "--reserve"},
	  [](const string &arg, ParserContext &ctx) {
		ctx.nextParser = [](const string &arg, ParserContext &ctx) {
			ctx.priv.reserve = atol(arg.c_str());
		};
	});

	parser.add({"-s", "--show"},
	  [](const string &arg, ParserContext &ctx) {
		 ctx.priv.show = true;
	});
	return parser.parse();
}

int main(int argc, char *argv[])
{
	PrimeNumber pn;

	const auto param = parseArg(argc, argv);
	cout << "Upper limit: " << param.upperLimit << endl;

	if (param.reserve > 0) {
		cout << "Reserve: " << param.reserve << endl;
		pn.reserve(param.reserve);
	}

	pn.calc(param.upperLimit);
	cout << "Count: " << pn.count() << endl;
	if (param.show)
		pn.show();

	return EXIT_SUCCESS;
}
