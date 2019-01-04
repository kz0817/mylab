import scala.math

class Ex1(_x: Double, _y: Double) {
  val x = _x
  val y = _y

  def this() {
    this(1, 1)
  }

  def length(): Double = math.sqrt(x*x + y*y)
}

object Main {
  def main(args: Array[String]) {
    val x0 = new Ex1()
    println(f"x0: -> ${x0.length()}%.1f")
    val x1 = new Ex1(2.1, 5.3)
    println(f"x1: x: 2.1, y: 5.3 -> ${x1.length()}%.1f")
  }
}
