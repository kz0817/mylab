import scala.collection.mutable.ListBuffer

object Factorization {

  val firstPrimeNumber = 2
  var primeNumberSet = Set(firstPrimeNumber)

  def factorize(_num: Int): ListBuffer[Int] = {
    var primeList = ListBuffer[Int]()
    var num = _num
    for (primeNum <- primeNumberSet) {
      while (num % primeNum == 0) {
        primeList += primeNum
        num = num / primeNum;
        if (num == 1)
          return primeList
      }
    }
    primeList += num
    primeNumberSet += num
    return primeList
  }

  def main(args: Array[String]) = {
    var maxNum = 10
    if (args.length >= 1)
      maxNum = args(0).toInt
    for (num <- firstPrimeNumber to maxNum)
      println(num + ": " + factorize(num) + ", " + primeNumberSet)
  }
};
