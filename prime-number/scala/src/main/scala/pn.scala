class PrimeNumber {
  val primeNumbers = new scala.collection.mutable.ArrayBuffer[Int]()

  def isPrimeNumber(n: Int): Boolean = {
    for (pn <- primeNumbers) {
      if (pn * pn > n)
        return true
      if (n % pn == 0)
        return false
    }
    return true
  }

  def calc(upperLimit: Int): Unit = {
    println("upper limit: " + upperLimit)
    for (n <- 2 to upperLimit) {
      if (isPrimeNumber(n))
        primeNumbers += n
    }
    println(s"Count: ${primeNumbers.length}")
  }
}

class ArgParser(args: Array[String]) {
  var upperLimit = 0;
  val it = args.toIterator
  while (it hasNext) {
    it next match {
      case "-u" => upperLimit = it.next toInt
    }
  }
}

object Main {
  def main(args: Array[String]) {
    val parser = new ArgParser(args)
    val pn = new PrimeNumber()
    pn.calc(parser.upperLimit)
  }
}
