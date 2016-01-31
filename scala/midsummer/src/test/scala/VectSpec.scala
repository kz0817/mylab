import org.scalatest._
import midsummer._

class VectSpec extends FlatSpec with Matchers {

  "A Vect" should "be created with the number of dimension" in {
    val vec = new Vect(3)
    vec should have size 3
  }

  it should "have elements whose value are all zero by default" in {
    val vec = new Vect(3)
    for (i <- 0 until 3)
      vec(i) should be (0)
  }

  it should "be created with an array" in {
    val vec = new Vect(Array(5.0, -2, 8))
    vec should have size 3
    vec(0) should be (5.0)
    vec(1) should be (-2)
    vec(2) should be (8)
  }

  it should "be created with numbers" in {
    val vec = new Vect(5.0, -2.0, 8.0)
    vec should have size 3
    vec(0) should be (5.0)
    vec(1) should be (-2)
    vec(2) should be (8)
  }

  it should "have the factory pattern from Int" in {
    val vec = Vect(2)
    vec should have size 2
    for (i <- 0 until 2)
      vec(i) should be (0)
  }

  it should "have the factory pattern from Iterable[Double]" in {
    val vec = Vect(Seq(3.0, 2))
    vec should have size 2
    vec(0) should be (3)
    vec(1) should be (2)
  }

  it should "have the factory pattern from Double*" in {
    val vec = Vect(3.0, 2)
    vec should have size 2
    vec(0) should be (3)
    vec(1) should be (2)
  }

  it should "have the factory pattern from Vect" in {
    val vec = Vect(new Vect(1, -5))
    vec should have size 2
    vec(0) should be (1)
    vec(1) should be (-5)
  }

  it should "have += operator" in {
    var vec = Vect(2.0, 3)
    vec += Vect(-1.0, 4)
    vec(0) should be (1)
    vec(1) should be (7)
  }

  it should "have -= operator" in {
    var vec = Vect(2.0, 3)
    vec -= Vect(-1.0, 4)
    vec(0) should be (3)
    vec(1) should be (-1)
  }

  it should "have + operator" in {
    val a = Vect(2.0, 3)
    val b = Vect(-1.0, 4)
    val vec = a + b
    vec(0) should be (1)
    vec(1) should be (7)
  }

  it should "have - operator" in {
    val a = Vect(2.0, 3)
    val b = Vect(-1.0, 4)
    val vec = a - b
    vec(0) should be (3)
    vec(1) should be (-1)
  }

  it should "have (* scalar) operator" in {
    val a = Vect(2.0, 3)
    val vec = a * 1.5
    vec(0) should be (3)
    vec(1) should be (4.5)
  }

  it should "have inner product (*) operator" in {
    val a = Vect(2.0, 3)
    val b = Vect(5.0, -9)
    val x = a * b
    x should be (-17)
  }

  it should "have map method" in {
    val vec = Vect(2.0, 3, -8)
    val mapped = vec map { +_ }
    mapped should have size 3
    mapped(0) should be (2)
    mapped(1) should be (3)
    mapped(2) should be (-8)
  }

  it should "have toArray method" in {
    val vec = Vect(2.0, 3, -8)
    val arr = vec.toArray
    arr should have size 3
    arr(0) should be (2)
    arr(1) should be (3)
    arr(2) should be (-8)
  }

  it should "have toString method" in {
    val vec = Vect(253.0, 3.5e3, -8.123)
    vec.toString should be ("(+2.530000e+02, +3.500000e+03, -8.123000e+00)")
  }

  it should "have ++ operator" in {
    val a = Vect(2.0, 3)
    val b = Vect(5.0, -9)
    val x = a ++ b
    x should have size 4
    x(0) should be (2)
    x(1) should be (3)
    x(2) should be (5)
    x(3) should be (-9)
  }
}
