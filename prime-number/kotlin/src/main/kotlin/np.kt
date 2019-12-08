data class Args(var upper: Int = 100, var showResult: Boolean = false)

fun showResult(numbers: Collection<Int>, args: Args) {
    println(numbers.size)
    if (args.showResult)
        println(numbers)
}

fun isPrimeNumber(n: Int, primeNumbers: List<Int>): Boolean {
    for (primeNumber in primeNumbers) {
        if (primeNumber * primeNumber > n)
            return true
        if (n % primeNumber == 0)
            return false
    }
    return true
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
        }
        loadNext()
    }
    return args
}

fun main(argArr: Array<String>) {
    val primeNumbers = mutableListOf(2, 3)
    val args = parseArgs(argArr)
    println("Upper bound: " + args.upper)

    var n = 5
    while (n < args.upper) {
        if (isPrimeNumber(n, primeNumbers))
            primeNumbers.add(n)
        n += 2
    }
    showResult(primeNumbers, args)
}
