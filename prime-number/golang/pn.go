package main

import (
    "fmt"
    "flag"
)

func isPrimeNumber(primeNumbers []int, n int) bool {
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

func main() {
    upperLimit := flag.Int("u", 100, "Upper Limit")
    showResult := flag.Bool("s", false, "Show prime numbers")
    flag.Parse()

    fmt.Printf("Upper Limit: %d\n", *upperLimit)

    primeNumbers := []int{2, 3}

    for n := 5; n < *upperLimit; n+=2 {
        if isPrimeNumber(primeNumbers, n) {
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
