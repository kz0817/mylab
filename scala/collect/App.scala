package example

object App extends App {
  val reg1 = "(^\\w*i)".r
  List("Memi", "Ayaka", "Rei", "Konno") collect {
    case reg1(name) => s"* ${name}"
    case (name: String) if name != "Konno" => s"  ${name}"
  } foreach println
}
