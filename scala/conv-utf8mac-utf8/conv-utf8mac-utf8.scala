import scala.collection.mutable.ListBuffer
import java.io.InputStreamReader

object ConvUtf8MacUtf8 {

  private val CodeDakuOn    = 0x3099
  private val CodeHanDakuOn = 0x309a
  private val dakuonMap = Map(
    'か' -> 'が', 'き' -> 'ぎ', 'く' -> 'ぐ', 'け' -> 'げ', 'こ' -> 'ご',
    'さ' -> 'ざ', 'し' -> 'じ', 'す' -> 'ず', 'せ' -> 'ぜ', 'そ' -> 'ぞ',
    'た' -> 'だ', 'ち' -> 'ぢ', 'つ' -> 'づ', 'て' -> 'で', 'と' -> 'ど',
    'は' -> 'ば', 'ひ' -> 'び', 'ふ' -> 'ぶ', 'へ' -> 'べ', 'ほ' -> 'ぼ',
    'カ' -> 'ガ', 'キ' -> 'ギ', 'ク' -> 'グ', 'ケ' -> 'ゲ', 'コ' -> 'ゴ',
    'サ' -> 'ザ', 'シ' -> 'ジ', 'ス' -> 'ズ', 'セ' -> 'ゼ', 'ソ' -> 'ゾ',
    'タ' -> 'ダ', 'チ' -> 'ヂ', 'ツ' -> 'ヅ', 'テ' -> 'デ', 'ト' -> 'ド',
    'ハ' -> 'バ', 'ヒ' -> 'ビ', 'フ' -> 'ブ', 'ヘ' -> 'ベ', 'ホ' -> 'ボ',
    'ウ' -> 'ヴ')
  private val hanDakuonMap = Map(
    'は' -> 'ぱ', 'ひ' -> 'ぴ', 'ふ' -> 'ぷ', 'へ' -> 'ぺ', 'ほ' -> 'ぽ',
    'ハ' -> 'パ', 'ヒ' -> 'ピ', 'フ' -> 'プ', 'ヘ' -> 'ペ', 'ホ' -> 'ポ')

  private def printChar(charCode: Char): Char = {
    print(charCode)
    return '\0'
  }

  def main(args: Array[String]): Unit = {
    val inStream = new InputStreamReader(System.in, "UTF-8")
    var prevCode = '\0'
    while (true) {
      val charCode = inStream.read() match {
        case -1 => '\0'
        case code: Int => code.asInstanceOf[Char]
      }
      prevCode = charCode match {
        case CodeDakuOn    => printChar(dakuonMap(prevCode))
        case CodeHanDakuOn => printChar(hanDakuonMap(prevCode))
        case _ =>
          if (prevCode != '\0')
            printChar(prevCode)
          charCode
      }
      if (charCode == '\0')
        return
    }
  }

}
