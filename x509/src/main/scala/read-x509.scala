import java.util.Base64
import scala.io.Source

class Tag(_klass: Int, _structFlag: Boolean, _number: Int)  {
  val klass = _klass
  val structFlag = _structFlag
  val number = _number
}

class LengthDescr(_length: Int, _numTokens: Int = 0) {
  val length = _length
  val lengthPartLength = 1 + _numTokens
}

class Return[+T] (v: T, len: Int) {
  val value = v
  val readLength = len
}

class Subject(_sigAlgo: String, _modulus: String, _exp: Int) {
  val signatureAlgorithm = _sigAlgo
  val modulus = _modulus
  val exponent = _exp
}

object ReadX509 {

  val TAG_ANY  = 0
  val TAG_BOOL = 0x01
  val TAG_INT  = 0x02
  val TAG_BIT_STR = 0x03
  val TAG_OCT_STR = 0x04
  val TAG_NULL = 0x05
  val TAG_OID  = 0x06
  val TAG_SEQ  = 0x10
  val TAG_SET  = 0x11
  val TAG_PRINT_STR = 0x13
  val TAG_UTC  = 0x17

  val CLASS_GENERAL: Byte = 0
  val CLASS_CONTEXT: Byte = 2


  class Module(_it: Iterator[Byte], tagType: Int = TAG_ANY) {
    val it = _it
    val tag = parseTag(it)
    if (tagType != TAG_ANY) {
      assert(tag.number == tagType,
             s"Exepected Tag: $tagType, Actual: ${tag.number}")
    }
    val lenDesc = readValueLength(it)
    val valueLength = lenDesc.length
    val headerLength = 1 /* tag length */ +  lenDesc.lengthPartLength
    //println(f"  - Tag: val: class: 0x${tag.klass}%02x, struct: ${tag.structFlag}%5b, number: 0x${tag.number}%02x, valueLength: $valueLength")

    def readValueLength(it: Iterator[Byte]): LengthDescr = {
      val b = it.next
      val isLong: Boolean = (b & 0x80) != 0
      val length: Int = (b & 0x7f)
      isLong match {
        case false => new LengthDescr(length)
        case true  => new LengthDescr(parseValueLengthLong(it, length), length)
      }
    }

    def parse[T](f: (Module) => Return[T]): Return[T] = {
      val ret = f(this)
      assert(valueLength == ret.readLength,
             s"length: ${valueLength}, actually read: ${ret.readLength}")
      new Return[T](ret.value, headerLength + ret.readLength)
    }

    def read[T](f: (Module, Seq[Byte]) => T): Return[T] = {
      val value = for (i <- 1 to valueLength) yield it.next
      val ret = f(this, value)
      new Return[T](ret, headerLength + valueLength)
    }

    def readAsHexDump(): Return[String] = {
      read { (h, v) => s"${toHexDump(v)}" }
    }
  }

  @scala.annotation.tailrec
  def appendLine(it: Iterator[String], connected: String): String = {
    val line = it.next
    if (line == "-----END CERTIFICATE-----")
      return connected
    else
      return appendLine(it, connected + line)
  }

  def asUnsigned(v: Byte): Int = v & 0xff
  def toString(v: Seq[Any]): String = v map { "%c".format(_) } mkString("")
  def toHexDump(v: Seq[Any]): String = v map { "%02x".format(_) } mkString(":")

  val OIDMap = Map(
    Seq(1, 2, 840, 113549, 1, 1, 1) -> "RSA Encryption",
    Seq(1, 2, 840, 113549, 1, 1, 2) -> "MD2 with RSA",
    Seq(1, 2, 840, 113549, 1, 1, 4) -> "MD5 with RSA",
    Seq(1, 2, 840, 113549, 1, 1, 5) -> "SHA1 with RSA",

    Seq(2, 5, 4, 3)  -> "CN",
    Seq(2, 5, 4, 6)  -> "C",
    Seq(2, 5, 4, 10) -> "O",
    Seq(2, 5, 4, 11) -> "OU",

    Seq(2, 5, 29, 14) -> "Subject Key Identifier",
    Seq(2, 5, 29, 15) -> "Key Usage",
    Seq(2, 5, 29, 19) -> "Basic constraints",
  )

  def totalReadLength(returns: Return[Any]*): Int = {
    returns map { _.readLength } sum
  }

  def decodeOID(binOid: Seq[Byte]): Seq[Int] = {

    // The first two numbers
    require(binOid.size > 0)
    val oid0 = asUnsigned(binOid.head)
    val elem1 = oid0 / 40
    val elem2 = oid0 - 40 * elem1

    // Remaining numbers
    val remaining = binOid.tail
    val breakIdx = remaining.zipWithIndex collect {
      case(v, idx) if (v & 0x80) == 0 => idx
    }

    val ranges = for ((endIdx, i) <- breakIdx.zipWithIndex) yield {
      i match {
        case 0 => (0,                 endIdx + 1)
        case _ => (breakIdx(i-1) + 1, endIdx + 1)
      }
    }

    val elemTail = for (r <- ranges) yield {
      (remaining slice(r._1, r._2) map { _ & 0x7f } reverse).zipWithIndex map {
         case(v, i) => v << (7*i)
      } sum
    }

    Seq(elem1, elem2) ++ elemTail
  }

  def parseTag(it: Iterator[Byte]): Tag = {

    assert(it.hasNext)
    val b = it.next
    val klass = (b & 0xc0) >> 6
    val structFlag  = (((b & 0x20) >> 5) == 1)
    val tagNumber = (b & 0x1f)
    if (klass != CLASS_GENERAL && klass != CLASS_CONTEXT)
      throw new NotImplementedError(s"Not implemneted: class: $klass")
    if (tagNumber >= 0x1f)
      throw new NotImplementedError(s"Not implemneted: tagNumber: $tagNumber")
    new Tag(klass, structFlag, tagNumber)
  }

  @scala.annotation.tailrec
  def parseValueLengthLong(it: Iterator[Byte], numTokens: Int, length: Int = 0): Int = {
    require(numTokens >= 1)
    assert(it.hasNext)
    val renewedLength = (length << 8) + asUnsigned(it.next)
    numTokens match {
      case 1 => renewedLength
      case _ => parseValueLengthLong(it, numTokens-1, renewedLength)
    }
  }

  def parseTBSCertificateVersion(it: Iterator[Byte]): Return[Option[Int]] = {
    new Module(it) parse { header =>
      assert(header.tag.klass == CLASS_CONTEXT)
      // TODO: consider the case version is omitted
      assert(header.tag.number == 0)
      new Module(it, TAG_INT) read { (header, value) =>
        assert(value.length == 1)
        value(0) match {
          case 0 => Some(1)
          case 1 => Some(2)
          case 2 => Some(3)
          case _ => None
        }
      }
    }
  }

  def parseTBSCertificateSerial(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_INT) read { (header, value) =>
      value map asUnsigned map { "%02x".format(_) } mkString(":")
    }
  }

  def parseTBSCertificateSignature(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_SEQ) parse { header =>
      val signature = new Module(it, TAG_OID) read { (header, value) =>
        val oid = decodeOID(value)
        val algorithm = OIDMap.getOrElse(oid, "Unknown")
        s"$algorithm (${oid mkString(".")})"
      }

      val option = new Module(it, TAG_NULL) read { (header, value) =>
        if (header.valueLength != 0) {
          throw new NotImplementedError(
           s"Not implemneted: option for Signature algorithm")
        }
        None
      }
      new Return(signature.value, totalReadLength(signature, option))
    }
  }

  @scala.annotation.tailrec
  def parseIssuerPiece(it: Iterator[Byte], issuerLength: Int,
                       results: Return[Seq[String]]) : Return[Seq[String]] = {

    assert(results.readLength <= issuerLength)

    val ret = new Module(it, TAG_SET) parse { header =>
      new Module(it, TAG_SEQ) parse { header =>
        val label = new Module(it, TAG_OID) read { (header, value) =>
          OIDMap.getOrElse(decodeOID(value), "Unknown OID")
        }
        val data = new Module(it, TAG_PRINT_STR) read { (h, v) => toString(v) }
        val str = s"${label.value}=${data.value}"
        new Return(results.value :+ str,
                   totalReadLength(label, data))
      }
    }

    val totalLength = results.readLength + ret.readLength
    val newResults = new Return(ret.value, totalLength)
    if (newResults.readLength == issuerLength)
      return newResults
    parseIssuerPiece(it, issuerLength, newResults)
  }

  def parseTBSCertificateIssuer(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_SEQ) parse { header =>
      val ret = parseIssuerPiece(it, header.valueLength, new Return(Seq(), 0))
      new Return(ret.value mkString(", "), ret.readLength)
    }
  }

  def parseTBSCertValidity(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_SEQ) parse { header =>

      def conv(s: Seq[Byte]): String = {
        assert(s(12) == 'Z')
        "20%c%c-%c%c-%c%c %c%c:%c%c:%c%c GMT".format(
        s(0), s(1), s(2), s(3), s(4), s(5), s(6), s(7),
        s(8), s(9), s(10), s(11))
      }
      val notBefore = new Module(it, TAG_UTC) read { (h, v) => conv(v) }
      val notAfter  = new Module(it, TAG_UTC) read { (h, v) => conv(v) }

      new Return(notBefore.value + " => " + notAfter.value,
                 totalReadLength(notBefore, notAfter))
    }
  }

  def parseTBSCertSubjectName(it: Iterator[Byte]): Return[String] = {
    parseTBSCertificateIssuer(it)
  }

  def parseTBSCertSubjPubKey(it: Iterator[Byte]): Return[Subject] = {
    new Module(it, TAG_BIT_STR) parse { header =>
      val unusedBits = it.next
      assert(unusedBits == 0, "Not implemented")
      val key = new Module(it, TAG_SEQ) parse { header =>
        val mod = new Module(it, TAG_INT) read { (header, value) =>
          value map { "%02x".format(_) } mkString(":")
        }
        val exp = new Module(it, TAG_INT) read { (header, value) =>
          (value reverse).zipWithIndex map {
             case(v, i) => asUnsigned(v) * (1 << (8*i))
          } sum
        }
        new Return(new Subject("", mod.value, exp.value),
                   totalReadLength(mod, exp))
      }
      new Return(key.value, key.readLength + 1)
    }
  }

  def parseTBSCertSubjectKey(it: Iterator[Byte]): Return[Subject] = {
    new Module(it, TAG_SEQ) parse { header =>
      val sig = parseTBSCertificateSignature(it)
      val key = parseTBSCertSubjPubKey(it)
      val subj = new Subject(sig.value, key.value.modulus, key.value.exponent)
      new Return(subj, totalReadLength(sig, key))
    }
  }

  def parseTBSCertExtBasicConstraints(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_SEQ) parse { header =>
      val ca = new Module(it, TAG_BOOL) read { (h, v) => v != 0}
      val pathFieldLen = header.valueLength - totalReadLength(ca)
      if (pathFieldLen != 0)
        throw new NotImplementedError(s"Not implemneted: path length")
      new Return(s"CA: ${ca.value}", totalReadLength(ca))
    }
  }

  def parseTBSCertExtKeyUsage(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_BIT_STR) read { (h, v) => s"${toHexDump(v)}" }
  }

  def parseTBSCertExtSubjKeyIdent(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_OCT_STR) read { (h, v) => s"${toHexDump(v)}" }
  }

  def parseTBSCertV3ExtValue(
      header: Module, oid: Seq[Int], it: Iterator[Byte]): Return[String] = {
    oid match {
      case Seq(2, 5, 29, 14) => parseTBSCertExtSubjKeyIdent(it)
      case Seq(2, 5, 29, 15) => parseTBSCertExtKeyUsage(it)
      case Seq(2, 5, 29, 19) => parseTBSCertExtBasicConstraints(it)
      case _ =>
        val data = for (i <- 1 to header.valueLength) yield it.next
        new Return(toHexDump(data), data.length)
    }
  }

  @scala.annotation.tailrec
  def parseTBSCertV3ExtEach(
      it: Iterator[Byte], totalExtValLength: Int,
      results: Return[String] = new Return("", 0)): Return[String] = {

    val m = new Module(it, TAG_SEQ) parse { header =>
      val oid = new Module(it, TAG_OID) read { (h, v) => decodeOID(v) }

      val secondMod = new Module(it, TAG_ANY) parse { header =>
        header.tag.number match {
          case TAG_OCT_STR =>
            val extVal = parseTBSCertV3ExtValue(header, oid.value, it)
            new Return((false, Some(extVal)), totalReadLength(extVal))

          case TAG_BOOL =>
            val critical = (it.next != 0)
            assert(header.valueLength == 1,
                   s"Unexpected length: ${header.valueLength}")
            new Return((critical, None), header.valueLength)

          case _ =>
            assert(false, s"Unexpected tag: ${header.tag.number}")
            new Return((false, None), 0)
        }
      }
      val (critical, tentativeExtVal) = secondMod.value

      val extVal = tentativeExtVal match {
        case Some(v) => new Return(v.value, 0)
        case None =>
          new Module(it, TAG_OCT_STR) parse { h =>
            parseTBSCertV3ExtValue(h, oid.value, it)
          }
      }
      val title = OIDMap.getOrElse(oid.value, oid.value.mkString("."))
      val critMsg = if (critical) "[critical]" else ""
      new Return(s"X509v3 Ext. ${title}: ${critMsg} ${extVal.value}",
                 totalReadLength(oid, secondMod, extVal))
    }

    val newTotalLen = totalReadLength(results, m)
    assert(newTotalLen <= totalExtValLength)
    val newMsg = results.value match {
      case "" => m.value
      case _ => results.value + "\n" + m.value
    }
    val newResults = new Return(newMsg, newTotalLen)
    if (newTotalLen == totalExtValLength)
      newResults
    else
      parseTBSCertV3ExtEach(it, totalExtValLength, newResults)
  }

  def parseTBSCertV3Ext(
      it: Iterator[Byte], remainingLen: Int,
      message: String = ""): Return[String] = {
    new Module(it, TAG_SEQ) parse { header =>
      parseTBSCertV3ExtEach(it, header.valueLength)
    }
  }

  def parseTBSCertificate(it: Iterator[Byte]): Return[String] = {
    new Module(it, TAG_SEQ) parse { header =>
      //println(s"TBSCertificate: Length: ${header.valueLength}")

      val version = parseTBSCertificateVersion(it)
      version.value match {
        case Some(v) => println(s"Version: $v")
        case None    => println("Version: Unknown")
      }

      val serial = parseTBSCertificateSerial(it)
      println(s"Serial: ${serial.value}")

      val signature = parseTBSCertificateSignature(it)
      println(s"Signature Algorithm: ${signature.value}")

      val issuer = parseTBSCertificateIssuer(it)
      println(s"Issuer: ${issuer.value}")

      val validity = parseTBSCertValidity(it)
      println(s"Validity: ${validity.value}")

      val subject = parseTBSCertSubjectName(it)
      println(s"Subject Name: ${subject.value}")

      val subjectKey = parseTBSCertSubjectKey(it)
      println(s"Subject Algorithm: ${subjectKey.value.signatureAlgorithm}")
      println(s"Modulus: ${subjectKey.value.modulus}")
      println(f"exponent: ${subjectKey.value.exponent}%x")

      val extensions = new Module(it, TAG_ANY) parse { header =>
        assert(header.tag.klass == CLASS_CONTEXT)
        // TODO: consider the case version is omitted or other versions
        assert(header.tag.number == 3)
        parseTBSCertV3Ext(it, header.valueLength)
      }
      println(extensions.value)

      val totalLen = totalReadLength(
          version, serial, signature, issuer, validity,
          subject, subjectKey, extensions)
      new Return("", totalLen)
    }
  }

  def parseSignature(it: Iterator[Byte]): Return[String] = {
    val algo = parseTBSCertificateSignature(it)
    val value = new Module(it, TAG_BIT_STR).readAsHexDump
    println(s"Subject Signature Algorithm: ${algo.value}")
    println(s"${value.value}")
    new Return("", totalReadLength(algo, value))
  }

  def parseCertificate(it: Iterator[Byte]): Unit = {
    val cert = new Module(it, TAG_SEQ) parse { header =>
      //println(s"Certificate: Length: ${header.valueLength}")
      val cert = parseTBSCertificate(it)
      val sigAlgo = parseSignature(it)
      new Return("", totalReadLength(cert, sigAlgo))
    }
  }

  def start(fileName: String): Unit = {
    val source = Source.fromFile(fileName)
    val it = source.getLines
    if (it.next != "-----BEGIN CERTIFICATE-----") {
      println("Not found: begin line")
      return
    }
    val encBody = appendLine(it, "")
    //println(encBody)
    val body = Base64.getDecoder().decode(encBody)
    parseCertificate(body.iterator)
  }

  def main(args: Array[String]): Unit = {
    if (args.length < 1) {
      println("Give me a certificate file name as first option.")
      return
    }
    start(args(0))
  }
}
