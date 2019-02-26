import scala.io.Source

object FileReadExample extends App {
  Source.fromFile("data.txt")
    .getLines
    .flatMap { "^[A-Z].*".r.findFirstIn }
    .foreach(println)
}
