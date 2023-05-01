data class Args(var upper: Int = 100, var showResult: Boolean = false,
                var varTypeLong: Boolean = false)

fun <T> showResult(numbers: Collection<T>, args: Args) {
    println(numbers.size)
    if (args.showResult)
        println(numbers)
}

class PrimeNumberInt {
    val primeNumbers = mutableListOf(2)

    fun isPrimeNumber(n: Int): Boolean {
        for (pn in primeNumbers) {
            if (pn * pn > n)
                return true
            if (n % pn == 0)
                return false
        }
        return true
    }

    fun run(args: Args) {
        var n = 3
        while (n < args.upper) {
            if (isPrimeNumber(n))
                primeNumbers.add(n)
            n += 2
        }
        showResult(primeNumbers, args)
    }
}

class PrimeNumberLong {
    val primeNumbers = mutableListOf(2L)

    fun isPrimeNumber(n: Long): Boolean {
        for (pn in primeNumbers) {
            if (pn * pn > n)
                return true
            if (n % pn == 0L)
                return false
        }
        return true
    }

    fun run(args: Args) {
        var n = 3L
        while (n < args.upper) {
            if (isPrimeNumber(n))
                primeNumbers.add(n)
            n += 2L
        }
        showResult(primeNumbers, args)
    }
}

fun parseArgs(argArr: Array<String>): Args {
    var idx = 0
    val args = Args()

    fun isEnd(): Boolean {
        return idx >= argArr.size
    }

    fun loadNext() {
        idx++
    }

    fun ref(): String {
        return argArr[idx]
    }

    fun refAsInt(): Int {
        return Integer.parseInt(ref())
    }

    while (!isEnd()) {
        when (ref()) {
            "-u", "--upper-limit" -> {
                loadNext()
                args.upper = refAsInt()
            }
            "-s", "--show" -> {
                args.showResult = true
            }
            "--long" -> {
                args.varTypeLong = true
            }
        }
        loadNext()
    }
    return args
}

fun main(argArr: Array<String>) {
    val args = parseArgs(argArr)
    println("Upper bound: " + args.upper)
    if (args.varTypeLong) {
        PrimeNumberLong().run(args)
    } else {
        PrimeNumberInt().run(args)
    }
}
