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

template <typename T>
class PrimeNumber {
	vector<T> primeNumbers;

	bool isPrimeNumber(const T n)
	{
		for (const auto &primeNum: primeNumbers) {
			if (primeNum * primeNum > n)
				return true;
			if (n % primeNum == 0)
				return false;
		}
		return true;
	}

public:
	void calc(const T upperLimit) {
		for (auto n = 2; n <= upperLimit; n++) {
			if (isPrimeNumber(n))
				primeNumbers.push_back(n);
		}
	}

	T count(void) const { return primeNumbers.size(); }
	void show(void) const {
		for (const auto pn: primeNumbers) { cout << pn << endl; }
	}

	void reserve(const T n) { primeNumbers.reserve(n); }
};

template <typename T>
class PrimeNumberOdd {
	vector<T> primeNumbers;

	bool isPrimeNumber(const T n)
	{
		for (const auto &primeNum: primeNumbers) {
			if (primeNum * primeNum > n)
				return true;
			if (n % primeNum == 0)
				return false;
		}
		return true;
	}

public:
	PrimeNumberOdd()
	{
		primeNumbers.push_back(2);
	}

	void calc(const T upperLimit) {
		for (auto n = 3; n <= upperLimit; n += 2) {
			if (isPrimeNumber(n))
				primeNumbers.push_back(n);
		}
	}

	T count(void) const { return primeNumbers.size(); }
	void show(void) const {
		for (const auto pn: primeNumbers) { cout << pn << endl; }
	}

	void reserve(const T n) { primeNumbers.reserve(n); }
};

template<typename T>
class PrimeNumberOddSmart {
	vector<T> primeNumbers;
	size_t lastIdx;

	bool isPrimeNumber(const T n)
	{
		T pn;
		for (size_t i = 0; i <= lastIdx; i++) {
			pn = primeNumbers[i];
			if (n % pn == 0)
				return false;
		}
		if (n > pn * pn)
			lastIdx++;
		return true;
	}

public:
	PrimeNumberOddSmart()
	: lastIdx(0)
	{
		primeNumbers.push_back(2);
	}

	void calc(const T upperLimit) {
		for (auto n = 3; n <= upperLimit; n+= 2) {
			if (isPrimeNumber(n))
				primeNumbers.push_back(n);
		}
	}

	T count(void) const { return primeNumbers.size(); }
	void show(void) const {
		for (const auto pn: primeNumbers) { cout << pn << endl; }
	}

	void reserve(const T n) { primeNumbers.reserve(n); }
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
	const auto param = parseArg(argc, argv);

#ifdef TYPE_LONG
	PrimeNumber<long> pn;
#else

#ifdef STD
	PrimeNumber<int> pn;
#endif
#ifdef ODD
	PrimeNumberOdd<int> pn;
#endif
#ifdef ODD_SMART
	PrimeNumberOddSmart<int> pn;
#endif

#endif

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
