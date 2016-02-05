import org.scalatest._
import midsummer._

class MatrixSpec extends FlatSpec with Matchers {

  "A Matrix" should "be created with the number of rows and columns" in {
    val matrix: Matrix = new Matrix(2, 3)
    matrix.rows should be (2)
    matrix.columns should be (3)
  }

  it should "have elements whose values are all zeros by defalut" in {
    val matrix: Matrix = new Matrix(5, 3)
    for (m <- 0 until 5; n <- 0 until 3)
      matrix(m, n) should be (0)
  }

  it should "be created with 2-D Array" in {
    val mtx: Matrix = new Matrix(Array(Array(1.0, 4.0),
                                       Array(-1.05, 5.2),
                                       Array(3.1415, 46)))
    mtx(0, 0) should be (1.0)
    mtx(0, 1) should be (4.0)
    mtx(1, 0) should be (-1.05)
    mtx(1, 1) should be (5.2)
    mtx(2, 0) should be (3.1415)
    mtx(2, 1) should be (46)
  }

  it should "be created with dimension and an array" in {
    val mtx: Matrix = new Matrix(2, 4, Array(1.0, 4.0, 5, 6,
                                             37, -99, -1.01, 100))
    mtx(0, 0) should be (1.0)
    mtx(0, 1) should be (4.0)
    mtx(0, 2) should be (5)
    mtx(0, 3) should be (6)
    mtx(1, 0) should be (37)
    mtx(1, 1) should be (-99)
    mtx(1, 2) should be (-1.01)
    mtx(1, 3) should be (100)
  }

  it should "be created with dimension and numbers" in {
    val mtx: Matrix = new Matrix(2, 4, 1.0, 4.0, 5, 6, 37, -99, -1.01, 100)
    mtx(0, 0) should be (1.0)
    mtx(0, 1) should be (4.0)
    mtx(0, 2) should be (5)
    mtx(0, 3) should be (6)
    mtx(1, 0) should be (37)
    mtx(1, 1) should be (-99)
    mtx(1, 2) should be (-1.01)
    mtx(1, 3) should be (100)
  }

  it should "be created with an another Matrix" in {
    val src: Matrix = new Matrix(2, 3, Array(1.0, 4.0, 5, -6, -9, -8))
    val mtx: Matrix = new Matrix(src)
    mtx(0, 0) should be (1.0)
    mtx(0, 1) should be (4.0)
    mtx(0, 2) should be (5)
    mtx(1, 0) should be (-6)
    mtx(1, 1) should be (-9)
    mtx(1, 2) should be (-8)
  }

  it should "have the factory pattern from dimensions" in {
    val mtx = Matrix(2, 3)
    mtx.rows should be (2)
    mtx.columns should be (3)
    for (i <- (0 until mtx.rows); j <- (0 until mtx.columns))
      mtx(i, j) should be (0)
  }

  it should "have set()" in {
    var mtx = Matrix(2, 2)
    mtx.set(0, 0, 1)
    mtx.set(1, 1, -1)
    mtx(0, 0) should be (1)
    mtx(0, 1) should be (0)
    mtx(1, 0) should be (0)
    mtx(1, 1) should be (-1)
  }

  it should "have += operator" in {
    val mtx: Matrix = new Matrix(2, 3, Array(1.0,   2, 13, 4,  5,  6))
    val foo: Matrix = new Matrix(2, 3, Array(10.0, -2, -3, 6, 16, 26))
    mtx += foo
    mtx(0, 0) should be (11.0)
    mtx(0, 1) should be (0)
    mtx(0, 2) should be (10)
    mtx(1, 0) should be (10)
    mtx(1, 1) should be (21)
    mtx(1, 2) should be (32)
  }

  it should "have -= operator" in {
    val mtx: Matrix = new Matrix(2, 3, Array(1.0,   2, 13, 4,  5,  6))
    val foo: Matrix = new Matrix(2, 3, Array(10.0, -2, -3, 6, 16, 26))
    mtx -= foo
    mtx(0, 0) should be (-9.0)
    mtx(0, 1) should be (4)
    mtx(0, 2) should be (16)
    mtx(1, 0) should be (-2)
    mtx(1, 1) should be (-11)
    mtx(1, 2) should be (-20)
  }

  it should "have *= operator" in {
    val mtx: Matrix = new Matrix(2, 3, Array(1.0, -2, 13, 4, 5, 6))
    mtx *= 1.5
    mtx(0, 0) should be (1.5)
    mtx(0, 1) should be (-3)
    mtx(0, 2) should be (19.5)
    mtx(1, 0) should be (6)
    mtx(1, 1) should be (7.5)
    mtx(1, 2) should be (9)
  }

  it should "have + operator" in {
    val a: Matrix = new Matrix(2, 3, Array(1.0, -2, 13, 4, 5, 6))
    val b: Matrix = new Matrix(2, 3, Array(10.0, -2, -3, 6, 16, 26))
    val mtx = a + b
    mtx(0, 0) should be (11)
    mtx(0, 1) should be (-4)
    mtx(0, 2) should be (10)
    mtx(1, 0) should be (10)
    mtx(1, 1) should be (21)
    mtx(1, 2) should be (32)
  }

  it should "have - operator" in {
    val a: Matrix = new Matrix(2, 3, Array(1.0, -2, 13, 4, 5, 6))
    val b: Matrix = new Matrix(2, 3, Array(10.0, -2, -3, 6, 16, 26))
    val mtx = a - b
    mtx(0, 0) should be (-9)
    mtx(0, 1) should be (0)
    mtx(0, 2) should be (16)
    mtx(1, 0) should be (-2)
    mtx(1, 1) should be (-11)
    mtx(1, 2) should be (-20)
  }


  it should "have (Matrix * scalar) operator" in {
    val m: Matrix = new Matrix(2, 3, Array(1.0, -2, 13, 4, 5, 6))
    val k = 2.5
    val mtx = m * k
    mtx(0, 0) should be (2.5)
    mtx(0, 1) should be (-5.0)
    mtx(0, 2) should be (32.5)
    mtx(1, 0) should be (10)
    mtx(1, 1) should be (12.5)
    mtx(1, 2) should be (15)
  }

  it should "have (Matrix * Matrix) operator" in {
    val a: Matrix = new Matrix(2, 3, Array(1.0, -2, 13, 4, 5, 6))
    val b: Matrix = new Matrix(3, 4, Array(5.0, -8,  10, 1,
                                           1,    3,   2, 2,
                                           8,    0, -10, 1))
    val mtx = a * b
    mtx.rows should be (2)
    mtx.columns should be (4)
    mtx(0, 0) should be (107)
    mtx(0, 1) should be (-14)
    mtx(0, 2) should be (-124)
    mtx(0, 3) should be (10)
    mtx(1, 0) should be (73)
    mtx(1, 1) should be (-17)
    mtx(1, 2) should be (-10)
    mtx(1, 3) should be (20)
  }

  it should "have transpose()" in {
    val a: Matrix = new Matrix(2, 3, Array(1.0, -2, 13, 4, 5, 6))
    val mtx = a.transpose()
    mtx.rows should be (3)
    mtx.columns should be (2)
    mtx(0, 0) should be (1.0)
    mtx(0, 1) should be (4)
    mtx(1, 0) should be (-2)
    mtx(1, 1) should be (5)
    mtx(2, 0) should be (13)
    mtx(2, 1) should be (6)
  }

  it should "have inverse() for 2x2" in {
    val a: Matrix = new Matrix(2, 2, 1.0, -2, 3, -4)
    val mtx = a.inverse()
    mtx.rows should be (2)
    mtx.columns should be (2)
    mtx(0, 0) should be (-2)
    mtx(0, 1) should be (1)
    mtx(1, 0) should be (-1.5)
    mtx(1, 1) should be (0.5)
  }

  it should "have inverse() for 3x3" in {
    val a: Matrix = new Matrix(3, 3, 3.0, -2, 3, -4, 5, 2, -1, 1, 2)
    val mtx = a.inverse()
    mtx.rows should be (3)
    mtx.columns should be (3)
    mtx(0, 0) should be (+0.5333333333333333)
    mtx(0, 1) should be (+0.4666666666666667)
    mtx(0, 2) should be (-1.2666666666666667)
    mtx(1, 0) should be (+0.4)
    mtx(1, 1) should be (+0.6)
    mtx(1, 2) should be (-1.2)
    mtx(2, 0) should be (+0.06666666666666667)
    mtx(2, 1) should be (-0.06666666666666667)
    mtx(2, 2) should be (+0.4666666666666667)
  }

  it should "have inverse() for 4x4" in {
    val mtx = Matrix(4, 4, 3.0, 1, 1, 2,
                             5, 1, 3, 4,
                             2, 0, 1, 0,
                             1, 3, 2, 1).inverse
    mtx.rows should be (4)
    mtx.columns should be (4)
    mtx(0, 0) should be (+0.5)
    mtx(0, 1) should be (-0.22727272727272727)
    mtx(0, 2) should be (+0.36363636363636365)
    mtx(0, 3) should be (-0.09090909090909091)
    mtx(1, 0) should be (+0.5)
    mtx(1, 1) should be (-0.31818181818181818)
    mtx(1, 2) should be (-0.09090909090909091)
    mtx(1, 3) should be (+0.2727272727272727)
    mtx(2, 0) should be (-1)
    mtx(2, 1) should be (+0.45454545454545453)
    mtx(2, 2) should be (+0.2727272727272727)
    mtx(2, 3) should be (+0.18181818181818182)
    mtx(3, 0) should be (0)
    mtx(3, 1) should be (+0.2727272727272727)
    mtx(3, 2) should be (-0.6363636363636364)
    mtx(3, 3) should be (-0.09090909090909091)
  }

  it should "have isSquare()" in {
     Matrix(2, 2).isSquare should be (true)
     Matrix(2, 3).isSquare should be (false)
  }

  it should "have determinat() for 2x2 matrix" in {
    val d = Matrix(2, 2, 3, 1, 4, 2).determinant
    d should be (2)
  }

  it should "have determinat() for 3x3 matrix" in {
    val d = Matrix(3, 3, 0, -1, -1, 3, 2, 1, 5, -2, 0).determinant
    d should be (11)
  }

  it should "have determinat() for 4x4 matrix" in {
    val d = Matrix(4, 4, 1, -2,  3,  2,
                         0,  2,  0,  2,
                         0,  0, -1, -2,
                         0,  0,  0, -6).determinant
    d should be (12)
  }

  it should "have cofactor() for 2x2 matrix" in {
    val m = Matrix(2, 2, 3, 1, 4, 2)
    m.cofactor(0, 0) should be (2)
    m.cofactor(0, 1) should be (-4)
    m.cofactor(1, 0) should be (-1)
    m.cofactor(1, 1) should be (3)
  }

  it should "have cofactor() for 3x3 matrix" in {
    val m = Matrix(3, 3, 0, -1, -1,
                         3,  2,  1,
                         5, -2,  0)
    m.cofactor(0, 0) should be (2)
    m.cofactor(0, 1) should be (5)
    m.cofactor(0, 2) should be (-16)
    m.cofactor(1, 0) should be (2)
    m.cofactor(1, 1) should be (5)
    m.cofactor(1, 2) should be (-5)
    m.cofactor(2, 0) should be (1)
    m.cofactor(2, 1) should be (-3)
    m.cofactor(2, 2) should be (3)
  }
}
