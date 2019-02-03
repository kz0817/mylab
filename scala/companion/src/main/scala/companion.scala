class Foo(val id: Int, val name: String) {

  val getLongName = f"id: $id, $name"
}

object Foo {
  def apply(name: String ) = {
    new Foo(1, name)
  }

  def apply() = {
    new Foo(2, "Gogo")
  }
}

object Main {
  def main(args: Array[String]) {
    val foo = Foo("Nanami")
    println(f"name: ${foo.name}")
    println(foo.getLongName)

    val goo = Foo()
    println(goo.getLongName)
  }
}
