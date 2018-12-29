package main

import (
    "fmt"
    "flag"
)

func isPrimeNumber(primeNumbers []int, n int, upperLimit int) bool {
    for _, primeNum := range primeNumbers {
        if (n % primeNum) == 0 {
            return false
        }
        if (primeNum * primeNum) >= upperLimit {
            break
        }
    }
    return true
}

func main() {
    upperLimit := flag.Int("u", 100, "Upper Limit")
    showResult := flag.Bool("s", false, "Show prime numbers")
    flag.Parse()

    fmt.Printf("Upper Limit: %d\n", *upperLimit)

    primeNumbers := []int{2}

    for n := 2; n < *upperLimit; n++ {
        if isPrimeNumber(primeNumbers, n, *upperLimit) {
            primeNumbers = append(primeNumbers, n)
        }
    }

    fmt.Printf("Count: %d\n",  len(primeNumbers))
    if (*showResult) {
        for _, primeNum := range primeNumbers {
            fmt.Printf("%d\n", primeNum)
        }
    }
}
