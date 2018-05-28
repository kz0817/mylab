const calc = (args) => {

    const primeNumbers = []

    const isPrimeNumber = (n) => {
        for (let i = 0; i < primeNumbers.length; i++) {
            const pn = primeNumbers[i]
            if (pn * pn > n)
                return true
            if (n % pn  == 0)
                return false
        }
        return true
    }

    for (let n = 2; n <= args.upperLimit; n++) {
        if (isPrimeNumber(n))
            primeNumbers.push(n)
    }

    console.log("Count: " + primeNumbers.length)
    if (args.show) {
        for (let i = 0; i < primeNumbers.length; i++)
            console.log(primeNumbers[i])
    }
}

const parseArgs = (argv) => {
    const args = {
        upperLimit: 0,
        show: false,
    }

    let nextParser = null;
    for (let i = 0; i < argv.length; i++) {
        const arg = argv[i]
        if (nextParser) {
            nextParser = nextParser(arg)
            continue
        }
        if (arg == "-u" || arg == "--upper-limit") {
            nextParser = (arg) => {
                args.upperLimit = Number(arg)
                return null
            }
        }
        else if (arg == "-s" || arg ==  "--show") {
            args.show = true
        }
    }
    return args
}

const start = () => {
    const args = parseArgs(process.argv)
    console.log("UpperLimit: " + args.upperLimit)
    calc(args)
}

start()
