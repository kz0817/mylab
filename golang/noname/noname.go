package main
import "fmt"

func main() {
    a := func(x int) int {
        return x * 2
    }(5)
    fmt.Println(a)
}
