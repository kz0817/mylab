import scala.io.Source
import scala.collection.mutable
import java.util.Date

object Main {
  private val numHeaders = 2
  private val path = "/proc/net/dev"
  private var interfaceMap = scala.collection.mutable.Map[String, InterfaceInfo]()

  class Counter(_recvBytes:Long, _recvPkts:Long, _sendBytes:Long, _sendPkts:Long) {
    val recvBytes = _recvBytes
    val recvPkts  = _recvPkts
    val sendBytes = _sendBytes
    val sendPkts  = _sendPkts
    val date = new Date()

    def formatRate(rate: Double): (Double, String) = {
      val units = List("  B", "KiB", "MiB", "GiB", "TiB", "EiB")
      var r = rate
      for (u <- units) {
         if (r < 1000 || u == units.last)
           return (r, u)
         r /= 1024
      }
      (-1, "ERR")
    }

    def diffInfo(counter:Counter): String = {
      val diffSec = (counter.date.getTime() - date.getTime()) / 1000.0
      val diffRecvBytes = counter.recvBytes - recvBytes
      val recvBytesPerSec = diffRecvBytes / diffSec
      val diffSendBytes = counter.sendBytes - sendBytes
      val sendBytesPerSec = diffSendBytes / diffSec
      val recvRate = formatRate(recvBytesPerSec)
      val sendRate = formatRate(sendBytesPerSec)
      return "Recv: %5.1f %3s/s, Send: %5.1f %3s/s".format(recvRate._1, recvRate._2, sendRate._1, sendRate._2)
    }
  }

  class InterfaceInfo {
    var counterStart:Counter = null;
    var counterPrev:Counter  = null;
  }


  def parseLine(dev: String): Unit = {
    val words = dev.trim.split(" +")
    val name = words(0).init
    val recvBytes = words(1)
    val recvPkts  = words(2)
    val sendBytes = words(9)
    val sendPkts  = words(10)

    val counter = new Counter(recvBytes.toLong, recvPkts.toLong,
                              sendBytes.toLong, sendPkts.toLong)
    var intfInfo = interfaceMap.getOrElse(name, null)
    if (intfInfo == null) {
      intfInfo = new InterfaceInfo()
      intfInfo.counterStart = counter
      intfInfo.counterPrev  = counter
      interfaceMap(name) = intfInfo
      return
    }

    println("[%16s] <Short> %s, <Long> %s".format(name, intfInfo.counterPrev.diffInfo(counter),
                                                    intfInfo.counterStart.diffInfo(counter)))
    intfInfo.counterPrev = counter
  }

  def main(args: Array[String]): Unit = {
    while (true) {
      val devs = Source.fromFile(path).getLines().toList.drop(numHeaders)
      println("")
      devs.foreach(parseLine)
      Thread.sleep(5000)
    }
  }
}
