import java.util.List;
import java.util.ArrayList;

public class App {

  List<Integer> primeNumbers = new ArrayList<>(){
    { add(2); }
  };

  private int upperLimit = 0;

  App(ArgParser parser) {
    upperLimit = parser.upperLimit;
  }

  boolean isPrimeNumber(int n) {
    for (var pn : primeNumbers) {
      if (pn * pn > n) {
        return true;
      }
      if (n % pn == 0) {
        return false;
      }
    }
    return true;
  }

  void run() {
    System.out.println("Upper limit: " + upperLimit);
    for (int n = 3; n <= upperLimit; n += 2) {
      if (isPrimeNumber(n)) {
        primeNumbers.add(n);
      }
    }
    System.out.println("Count: " + primeNumbers.size());
  }

  public static void main(String[] args) {
    var parser = new ArgParser(args);
    var app = new App(parser);
    app.run();
  }
}
