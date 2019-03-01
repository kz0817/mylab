import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.sql.SQLException;
import java.sql.ResultSet;

object Main {

  case class Option(args: Array[String]) {

    val argIdxMap = args.zipWithIndex.map {
       case(arg, idx) => arg -> idx.toInt
    }.toMap

    val numLoops = argIdxMap.get("-n") match {
      case Some(idx) => args(idx+1).toInt
      case None => 1
    }

    val commitAll = argIdxMap contains "-c"
  }

  def run(opt: Option) {
    val conn = DriverManager.getConnection("jdbc:sqlite:try2.db");
    val stmt = conn.createStatement();
    val cols = Array(
      "id integer primary key",
      "name text",
      "age integer"
    ).mkString(",")
    stmt.executeUpdate(s"create table if not exists table1($cols)")

    val sql = "insert into table1(name, age) values(?, ?)"
    val pstmt = conn.prepareStatement(sql);

    def insert() {
      val data = Array(("memi", 17), ("rei", 15))
      data foreach { case (name: String, age: Int) =>
        pstmt.setString(1, name)
        pstmt.setInt(2, age)
        pstmt.execute();
      }
    }

    def beginIfCommitAll() {
      if (opt.commitAll)
        stmt.executeUpdate("begin")
    }

    def commitIfCommitAll() {
      if (opt.commitAll)
        stmt.executeUpdate("commit")
    }

    println(s"Num Loops: ${opt.numLoops}")
    println(s"Commit all: ${opt.commitAll}")

    if (!opt.commitAll)
      stmt.executeUpdate("begin")
    for (i <- 0 to opt.numLoops) {
      beginIfCommitAll()
      insert()
      commitIfCommitAll()
    }
    if (!opt.commitAll)
      stmt.executeUpdate("commit")
  }

  def main(args: Array[String]) {
    Class.forName("org.sqlite.JDBC")
    run(new Option(args))
  }
}
