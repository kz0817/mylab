data class Args(var upper: Int = 100, var showResult: Boolean = false)

fun showResult(numbers: Collection<Int>, args: Args) {
    println(numbers.size)
    if (args.showResult)
        println(numbers)
}

fun isPrimeNumber(n: Int, primeNumbers: List<Int>): Boolean {
    for (primeNumber in primeNumbers) {
        if (n % primeNumber == 0)
            return false
    }
    return true
}

fun parseArgs(argArr: Array<String>): Args {
    var idx = 0
    val args = Args()
    while (idx < argArr.size) {
        when(argArr[idx]) {
            "-u" -> {
                args.upper = Integer.parseInt(argArr[idx+1])
                idx++
            }
            "-s" -> {
                args.showResult = true
            }
        }
        idx++
    }
    return args
}

fun main(args: Array<String>) {
    val primeNumbers = mutableListOf(2, 3)
    val args = parseArgs(args)
    println("Upper bound: " + args.upper)

    var n = 5
    while (n < args.upper) {
        if (isPrimeNumber(n, primeNumbers))
            primeNumbers.add(n)
        n += 2
    }
    showResult(primeNumbers, args)
}
