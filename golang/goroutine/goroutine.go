package main
import (
    "fmt"
    "time"
)

func func1(msg string) {
    fmt.Printf("Your input: %s\n", msg)
}

func main() {
    go func1("foo")
    time.Sleep(time.Second)
}
