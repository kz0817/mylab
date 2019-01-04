class Foo {
  def func3() = println("func3")
  def func4(x: Integer) = println(s"func4: ${x}")

  val func5 = (x: Int) => println(s"func5: ${x}")
}

object Main {

  def func1 = println("func1")

  def func2() = println("func2()")

  def four = (x: Int) => 4 * x

  def main(args: Array[String]) {
    println("Hello");
    func1
    func2
    // func1() // compile error
    func2()

    val foo = new Foo()
    foo.func3()
    // foo.func3{}
    foo.func4(5)
    foo.func4{6}
    //foo.func4 7
    foo func4 8

    foo.func5(10)

    println(four(5))
  }
}
