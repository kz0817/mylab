import scala.collection.mutable.ListBuffer

object FindDiffDiff {

  var plusLines: ListBuffer[String]  = ListBuffer()
  var minusLines: ListBuffer[String] = ListBuffer()

  def detectPlus(line: String) = {
  }

  def detectMinus(line: String) = {
  }

  def detectNonDiffLine(): Unit = {
    if (plusLines.isEmpty && minusLines.isEmpty)
      return
    if (plusLines.length != minusLines.length) {
      println("====== (+) " + plusLines.length + ", (-) " + minusLines.length + " ======")
      for (line <- plusLines)
        println(line)
      for (line <- minusLines)
        println(line)
    }
    plusLines.clear()
    minusLines.clear()
  }

  def main(args: Array[String]) = {
    val reStartsPlus  = """^\+.*""".r
    val reStartsMinus = """^\-.*""".r
    val sc = new java.util.Scanner(System.in)
    while (sc.hasNext()) {
      val line = sc.nextLine()
      line match {
        case reStartsPlus() => plusLines += line
        case reStartsMinus() => minusLines += line
        case _ => detectNonDiffLine()
      }
    }
  }

}
