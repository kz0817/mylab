package midsummer

class Matrix(m: Int, n: Int) {
  var matrix = Array.ofDim[Double](m, n)

  def this(elements: Array[Array[Double]]) = {
    this(elements.size, elements(0).size)
    setElement { elements(_)(_) }
  }

  def this(m: Int, n: Int, arr: Seq[Double]) = {
    this(m, n)
    val arrIt = arr.toIterator
    setElement { () => arrIt.next() }
  }

  def this(src: Matrix) = {
    this(src.rows, src.columns)
    setElement { src(_, _) }
  }

  def rows = m
  def columns = n
  def apply(m: Int, n: Int) = matrix(m)(n)

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

  def *(that: Matrix): Matrix = {
    var mtx = new Matrix(this.rows, that.columns)
    mtx.setElement { (i: Int, j: Int) =>
      (0 until this.columns).map { a => this(i, a) * that(a, j) }.sum }
    mtx
  }

  def transpose(): Matrix = {
    var mtx = new Matrix(this.columns, this.rows)
    mtx.setElement { (i: Int, j: Int) => this(j, i) }
    mtx
  }
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
    val s = map { v => f"${v}%e" } map {
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
}
