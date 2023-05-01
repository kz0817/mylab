package main

import (
    "fmt"
    "flag"
)

type Number interface {
    int32 | int64
}

func isPrimeNumber[T Number](primeNumbers []T, n T) bool {
    for _, primeNum := range primeNumbers {
        if primeNum * primeNum > n {
            break
        }
        if (n % primeNum) == 0 {
            return false
        }
    }
    return true
}

func run[T Number](upperLimit int, showResult bool) {
    primeNumbers := []T{2}

    for n := 3; n < upperLimit; n+=2 {
        _n := T(n)
        if isPrimeNumber(primeNumbers, _n) {
            primeNumbers = append(primeNumbers, _n)
        }
    }

    fmt.Printf("Count: %d\n",  len(primeNumbers))
    if (showResult) {
        for _, primeNum := range primeNumbers {
            fmt.Printf("%d\n", primeNum)
        }
    }
}

func main() {
    var upperLimit int
    var varTypeInt64 bool
    var showResult bool
    flag.IntVar(&upperLimit, "u", 100, "Upper Limit")
    flag.BoolVar(&varTypeInt64, "int64", false, "use long")
    flag.BoolVar(&showResult, "s", false, "Show prime numbers")
    flag.Parse()

    fmt.Printf("Upper Limit: %d\n", upperLimit)
    if (varTypeInt64) {
        fmt.Println("VarType: int64")
        run[int64](upperLimit, showResult)
    } else {
        fmt.Println("VarType: int32")
        run[int32](upperLimit, showResult)
    }
}
