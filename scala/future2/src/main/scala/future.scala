import scala.concurrent._
import scala.concurrent.ExecutionContext.Implicits.global
import scala.util.{Success, Failure}

object Main {

  def main(args: Array[String]) {
    val f: Future[Int] = Future {
      Thread.sleep(1000)
      1
    }

    println("Start")
    f onComplete {
      case Success(a) => print(s"Hello $a")
      case Failure(t) => print("Error: " + t.getMessage)
    }
  }
}
