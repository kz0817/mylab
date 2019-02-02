import scala.concurrent._
import scala.concurrent.ExecutionContext.Implicits.global
import scala.concurrent.duration.Duration

object Main {

  def main(args: Array[String]) {
    val f: Future[Int] = Future {
      Thread.sleep(1000)
      1
    }

    println("Start")

    val a: Int = Await.result(f, Duration.Inf)
    println(s"Hello: $a")
  }
}
