package main
import (
    "fmt"
)

func func1(ch chan string) {
    ch <- "OK"
}

func main() {
    ch := make(chan string)
    go func1(ch)
    fmt.Printf("Reply: %s\n", <-ch)
}
