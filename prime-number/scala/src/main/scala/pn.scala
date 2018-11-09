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
}

object Main {
  def main(args: Array[String]) {
    val pn = new PrimeNumber()
    pn.calc(100000000)
  }
}
