import scala.io.Source
import midsummer._

class Recog3Classifier(numClass: Int, numFeature: Int) extends Classifier {

  class SubUnit(klass: Int, numFeature: Int) {
    var w: Vect = null

    def setWeight(weight: Vect) = {
      w = weight
      println(s"learned: class: ${klass}, ${w}")
    }

    def apply(x: Vect): Double = w * x
  }

  val subUnits = (0 until numClass) map {
    i => new SubUnit(Prototypes.indexToClass(i), numFeature) }

  private
  def supervisorVector(classSeq: Seq[Int], correctClass: Int): Vect = {
    Vect(classSeq map {
      _ == correctClass match {
        case true  => 1.0
        case false => 0.0
      }
    })
  }

  def learn(prototypes: Prototypes) = {
    val X = new Matrix(prototypes.argumentedFeatureVectors.map(_.toArray))
    for (i <- (0 until numClass)) {
      val correctClass = Prototypes.indexToClass(i)
      val bi = supervisorVector(prototypes.classSeq, correctClass).transpose
      subUnits(i).setWeight(Vect((X.transpose * X).inverse * X.transpose * bi))
    }
  }

  def apply(x: Vect): Int = {
    val idx = subUnits.map(_(x)).zipWithIndex.max._2
    Prototypes.indexToClass(idx)
  }
}

object Recog3 {
  def main(args: Array[String]) = {
    val lb = new LearningBase(args, new Recog3Classifier(_, _))
    if (!lb.run())
      println("Failed to invoke.")
  }
}
