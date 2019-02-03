case class Foo(f: (Int) => Int ) {
  val run: (Int => Int) = f(_)
  val run2: (Int => Int) = { n => f(n+3) }
}

object Main {
  def main(args: Array[String]) {
    val foo = Foo(2 * _)
    println(f"Hello: ${foo}")
    println(f"foo.run(2): ${foo.run(2)}")
    println(f"foo.run2(4): ${foo.run2(4)}")

    val goo = Foo { 3 * _ + 1 }
    println(f"goo.run2(2): ${goo.run2(2)}")
    println(f"goo.run2(3): ${goo.run2(3)}")
  }
}
