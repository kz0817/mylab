package midsummer

class Matrix(m: Int, n: Int) {
  var matrix = Array.ofDim[Double](m, n)

  //def this(elements: Array[Array[Double]]) = {
  def this(elements: Seq[Array[Double]]) = {
    this(elements.size, elements(0).size)
    setElement { elements(_)(_) }
  }

  def this(m: Int, n: Int, arr: Iterable[Double]) = {
    this(m, n)
    val arrIt = arr.toIterator
    setElement { () => arrIt.next() }
  }

  def this(m: Int, n: Int, xs: Double*) = this(m, n, xs.toIterable)

  def this(m: Int, n: Int, f:(Int, Int) => Double) = {
    this(m, n)
    setElement { (i: Int, j: Int) => f(i, j) }
  }

  def this(src: Matrix) = {
    this(src.rows, src.columns)
    setElement { src(_, _) }
  }

  def rows = m
  def columns = n
  def column(idx: Int) = matrix(idx)
  def apply(m: Int, n: Int) = matrix(m)(n)
  def set(m: Int, n: Int, v: Double) = matrix(m)(n) = v

  def iter(f:(Int, Int) => Any) = {
    for (i <- 0 until this.rows; j <- 0 until this.columns)
      yield f(i, j)
  }

  def setElement(f:() => Double) = {
    iter { (i: Int, j: Int) => matrix(i)(j) = f() }
  }

  def setElement(f:(Int, Int) => Double) = {
    iter { (i: Int, j: Int) => matrix(i)(j) = f(i, j) }
  }

  def setElement(f:(Int, Int, Double) => Double) = {
    iter { (i: Int, j: Int) =>
      matrix(i)(j) = f(i, j, matrix(i)(j)) }
  }

  def copy(): Matrix = new Matrix(this)

  def +=(that: Matrix) = setElement { that(_, _) + _}
  def -=(that: Matrix) = setElement { -that(_, _) + _}
  def *=(k: Double) = setElement { k * matrix(_)(_) }

  def +(that: Matrix): Matrix = {
    var mtx = copy()
    mtx += that
    mtx
  }

  def -(that: Matrix): Matrix = {
    var mtx = copy()
    mtx -= that
    mtx
  }

  def *(k: Double): Matrix = {
    var mtx = copy()
    mtx *= k
    mtx
  }

  def *(that: Matrix): Matrix =
    new Matrix(this.rows, that.columns, (i: Int, j: Int) =>
      (0 until this.columns).map { a => this(i, a) * that(a, j) }.sum)

  def transpose(): Matrix =
    new Matrix(this.columns, this.rows, (i: Int, j: Int) => this(j, i))

  def inverse(): Matrix = {
    (rows, columns) match {
      case (2, 2) => inverse2x2()
      case (3, 3) => inverse3x3()
      case _ => inverseGeneric()
    }
  }

  def determinant(): Double = determinantGeneric(this)

  def isSquare(): Boolean = rows == columns

  def cofactor(i: Int, j: Int): Double = {
    def sign(row: Int, col: Int) = {
      (row + col) % 2 match {
        case 0 => +1.0
        case 1 => -1.0
      }
    }
    val m = new Matrix(rows-1, columns-1,
      for (row <- 0 until rows if row != i;
           col <- 0 until columns if col != j)
        yield matrix(row)(col)
    )
    sign(i, j) * m.determinant
  }

  def determinantGeneric(m: Matrix): Double = {
    assert(m.isSquare, println(s"Not a square matrix ${m.rows}x${m.columns}"))
    m.rows match {
      case 1 => m(0, 0)
      case 2 => m(0, 0)*m(1, 1) - m(0, 1)*m(1, 0)
      case _ => (0 until m.rows).map { i => m(i, 0) * m.cofactor(i, 0) }.sum
    }
  }

  def inverse2x2(): Matrix = {
    val m = matrix
    var newMtx = new Matrix(2, 2, m(1)(1), -m(0)(1), -m(1)(0), m(0)(0))
    newMtx *= 1.0 / (m(0)(0)*m(1)(1) - m(0)(1)*m(1)(0))
    newMtx
  }

  def inverse3x3(): Matrix = {
    val m = matrix
    var newMtx = new Matrix(3, 3,
      m(1)(1)*m(2)(2) - m(1)(2)*m(2)(1), m(0)(2)*m(2)(1) - m(0)(1)*m(2)(2),
      m(0)(1)*m(1)(2) - m(0)(2)*m(1)(1),
      m(1)(2)*m(2)(0) - m(1)(0)*m(2)(2), m(0)(0)*m(2)(2) - m(0)(2)*m(2)(0),
      m(0)(2)*m(1)(0) - m(0)(0)*m(1)(2),
      m(1)(0)*m(2)(1) - m(1)(1)*m(2)(0), m(0)(1)*m(2)(0) - m(0)(0)*m(2)(1),
      m(0)(0)*m(1)(1) - m(0)(1)*m(1)(0))
    newMtx *= 1.0 / (m(0)(0)*m(1)(1)*m(2)(2) + m(1)(0)*m(2)(1)*m(0)(2)
                     + m(2)(0)*m(0)(1)*m(1)(2) - m(0)(0)*m(2)(1)*m(1)(2)
                     - m(2)(0)*m(1)(1)*m(0)(2) - m(1)(0)*m(0)(1)*m(2)(2))
    newMtx
  }

  def inverseGeneric(): Matrix = {
    val detM = determinant
    new Matrix(rows, columns, (i: Int, j:Int) => cofactor(j, i) / detM)
  }

  override def toString = {
    var s = s"Matrix (${rows}x${columns}) "
    for (i <- 0 until rows; j <- 0 until columns)
       s += f"${matrix(i)(j)}%+e "
    s
  }
}

object Matrix {
  def apply(m: Int, n: Int) = new Matrix(m, n)
  def apply(m: Int, n: Int, xs: Double*) = new Matrix(m, n, xs)
}

class Vect(n: Int) extends Matrix(1, n) {

  def this(xs: Iterable[Double]) {
    this(xs.size)
    val xsIt = xs.toIterator
    setElement { () => xsIt.next() }
  }

  def this(xs: Double*) {
    this(xs.toIterable)
  }

  def this(vec: Vect) {
    this(vec.size)
    setElement { (_: Int, i: Int) => vec(i) }
  }

  override
  def copy(): Matrix = new Vect(this)

  def size = columns
  def apply(i: Int): Double = this(0, i)
  def map (f:(Double) => Any) = iter { (i: Int, j: Int) => f(apply(i, j)) }
  def toArray: Array[Double] = matrix(0)
  override def toString = {
    val s = map { v => f"${v}%+e" } map {
              a => a.asInstanceOf[String] } reduce { (a, b) => s"$a, $b" }
    "(" + s + ")"
  }

  def +(that: Vect): Vect = (super.+(that)).asInstanceOf[Vect]
  def -(that: Vect): Vect = (super.-(that)).asInstanceOf[Vect]
  override def *(k: Double): Vect = (super.*(k)).asInstanceOf[Vect]
  def *(vec: Vect): Double = (this * vec.transpose())(0, 0)
  def ++(that: Vect): Vect = new Vect(this.toArray ++ that.toArray)
}

object Vect {
  def apply(n: Int) = new Vect(n)
  def apply(xs: Iterable[Double]) = new Vect(xs)
  def apply(xs: Double*) = new Vect(xs)
  def apply(vec: Vect) = new Vect(vec)
  def apply(m: Matrix) = new Vect(for (i <- (0 until m.rows)) yield m(i, 0))
}
