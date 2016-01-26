class Classifier(numClass: Int, numFeature: Int) {

  class Func(klass: Int, numFeature: Int) {
    val w = -0.5 * Math.pow(prototype(0), 2) +:
            ((1 to numFeature) map { d => prototype(d) })
    println(s"c: $klass, $w")

    def prototype(d: Int): Double = {
      (klass + d) % 2 - 0.5
    }

    def hook(v: Double): Double = {
      //println(v)
      return v
    }

    def apply(x: Seq[Double]): Double = {
      (w zip x map { z => z._1 * z._2 } map hook).sum
    }
  }

  val funcs = (1 to numClass) map { i => new Func(i, numFeature) }

  def hook(v: (Double, Int)) = {
    //println(v)
    v;
  }

  def apply(x: Seq[Double]): Int = {
    ((funcs map { _(1.0 +: x) }).zipWithIndex map hook).max._2
  }
}

object Recog1 {

  def main(args: Array[String]) = {
    val numClass = 2
    val numFeature = 1
    val classifier = new Classifier(numClass, numFeature)
    val xs = Seq(Seq(3.0), Seq(-1.0), Seq(5.0), Seq(-0.1), Seq(0.05))
    xs map { classifier(_) } foreach { c => println(s"c: $c") }
  }
}
