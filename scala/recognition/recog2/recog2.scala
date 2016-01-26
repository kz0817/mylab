import scala.io.Source

object Ctx {
  var showDotResult = false
}

class Classifier(numClass: Int, numFeature: Int) {

  class ClsfFunc(klass: Int, numFeature: Int) {
    val rho = 0.1
    var w = (0 to numFeature) map { d => 0.0 }
    println(s"[${this.getClass.getName}] class: $klass, $w, rho: $rho")

    def hook(v: Double): Double = {
      if (Ctx.showDotResult)
        println(v)
      return v
    }

    def delta_weight(x: Seq[Double]) = {
      x map { _ * rho }
    }

    def inc_weight(x: Seq[Double]) = {
      w = w zip delta_weight(x) map { v => v._1 + v._2 }
    }

    def dec_weight(x: Seq[Double]) = {
      w = w zip delta_weight(x) map { v => v._1 - v._2 }
    }

    def apply(x: Seq[Double]): Double = {
      (w zip x map { z => z._1 * z._2 } map hook).sum
    }

    def showWeights() {
      val s = w map { a => f"$a%.3f" } reduce { (a, b) => s"$a, $b" }
      println(s"c: $klass => $s")
    }
  }

  val cfuncs = (1 to numClass) map { i => new ClsfFunc(i, numFeature) }

  def hook(v: (Double, Int)) = {
    v;
  }

  def learn(data: Seq[(Seq[Double], Int)]) = {
    showWeights()
    var numRepeat = 0
    while (tryLearn(data) >= 1) {
      numRepeat += 1
    }
    println(s"====> numRepeat: $numRepeat")
  }

  private def tryLearn(data: Seq[(Seq[Double], Int)]): Int = {
    val numFixed = (data map { d =>
      val x = 1.0 +: d._1
      val trueClass = d._2
      println(s">>> data: $d")
      fixWeightIfNeeded(trueClass, x)
    }).sum
    println(s"# of fixed: ${numFixed}")
    numFixed
  }

  def fixWeightIfNeeded(trueClass: Int, x: Seq[Double]): Int = {
    val calcClass = apply(x)
    if (calcClass != trueClass)
      fixWeightRecursively(trueClass, calcClass, x)
    else
      0
  }

  protected def toIndex(klass: Int) = { klass - 1 }
  protected def toClass(idx: Int) = { idx + 1 }

  def fixWeightRecursively(trueClass: Int, calcClass: Int, x: Seq[Double]): Int = {
      cfuncs(toIndex(trueClass)).inc_weight(x);
      cfuncs(toIndex(calcClass)).dec_weight(x);
      val numFixed = fixWeightIfNeeded(trueClass, x)
      showWeights()
      numFixed + 1
  }

  protected def showWeights() = {
    println("===== Weights =====")
    cfuncs foreach { _.showWeights() }
    println("===")
  }

  def apply(x: Seq[Double]): Int = {
    val idx = ((cfuncs map { _(x) }).zipWithIndex map hook).max._2
    toClass(idx)
  }
}

object Recog2 {

  private def usage() {
    println("Usage: ")
    println(" learning_file [target_file]")
  }

  private def parseLearningDataLine(line: String): (Seq[Double], Int) = {
    val elems = line.split(" +")
    val klass = elems.head.toInt
    val featureVect = elems.tail.map { _.toDouble }
    (featureVect, klass)
  }

  private def parseDataLine(line: String): Seq[Double] = {
    line.split(" +") map { _.toDouble }
  }

  private
  def getLearningDataOutline(data: Seq[(Seq[Double], Int)]): (Int, Int) = {
    val numClass = data.map { _._2 }.toSet.size
    val numFeature = data.head._1.size
    data.tail.map { _._1 } foreach { s => assert(s.size == numFeature) }
    (numClass, numFeature)
  }

  private def isValidDataLine(line: String): Boolean  = {
    val trimmed = line.trim
    !(trimmed.isEmpty || trimmed(0) == '#')
  }

  def main(args: Array[String]) = {

    if (args.length == 0) {
      usage()
      sys.exit(1)
    }
    val learningFile = args(0)
    val lines = Source.fromFile(learningFile).getLines.toSeq
    val data = lines filter isValidDataLine map parseLearningDataLine
    val (numClass, numFeature) = getLearningDataOutline(data)
    println(s"Learning : $learningFile")
    println(s"  # data: ${data.size}")
    println(s"  # of class: $numClass")
    println(s"  # of feature: $numFeature")
    println("-----")

    val classifier = new Classifier(numClass, numFeature)
    classifier.learn(data)

    if  (args.length > 1) {
      val targetFile = args(1)
      println(s"")
      println(s"******* discrimination *******")
      println(s"file: ${targetFile}")
      val lines = Source.fromFile(targetFile).getLines.toSeq
      lines filter isValidDataLine map parseDataLine map {
        x => (x, classifier(1.0 +: x))
      } foreach {
        t => println(s"*** input: ${t._1},\tclass: ${t._2}")
      }
    }
  }
}
