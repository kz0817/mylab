import org.scalatest._
import midsummer._

class PatternsSpec extends FlatSpec with Matchers {

  val data = Seq(Vect(1.0, 5, 3), Vect(-1.0, -1.0, 0), Vect(100.0, -5050, 802))

  "A Patterns" should "be created with Seq[Vect]" in {
    val s = Patterns(data)
  }

  it should "have size" in {
    Patterns(data).size should be (3)
  }

  it should "have numberOfFeature" in {
    Patterns(data).numberOfFeature should be (3)
  }

  it should "have argumentedFeatureVectors" in {
    Patterns(data).argumentedFeatureVectors should be
      (Vect(1.0, 1.0, 2, 3), Vect(1.0, -1.0, -1.0, 0),
       Vect(1.0, 100.0, -5050, 802))
  }
}

class PrototypesSpec extends FlatSpec with Matchers {

  val data = Seq((Vect(1.0, 5, 3), 0), (Vect(-1.0, -1.0, 0), 1),
                 (Vect(100.0, -5050, 80002), 0))

  "A Prototypes" should "be created with Seq[(Vect, Int)]" in {
    val s = Prototypes(data)
  }

  it should "have size" in {
    Prototypes(data).size should be (3)
  }

  it should "have classSeq" in {
    Prototypes(data).classSeq should be
      (Vect(1.0, 2, 3), Vect(-1.0, -1.0, 0), Vect(100.0, -5050, 80002))
  }

  it should "have numberOfClass" in {
    Prototypes(data).numberOfClass should be (2)
  }
}
