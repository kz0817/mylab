object Main {

  def func1 = println("func1")

  def func2() = println("func2()")

  def main(args: Array[String]) {
    println("Hello");
    func1
    func2
    // func1() // compile error
    func2()
  }
}
