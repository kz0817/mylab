import java.lang.Number;
import java.util.List;
import java.util.ArrayList;

class PrimeNumberInteger {
  List<Integer> primeNumbers = new ArrayList<>(){
    { add(2); }
  };

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

  int run(int upperLimit) {
    for (int n = 3; n <= upperLimit; n += 2) {
      if (isPrimeNumber(n)) {
        primeNumbers.add(n);
      }
    }
    return primeNumbers.size();
  }
}

class PrimeNumberLong {
  List<Long> primeNumbers = new ArrayList<>(){
    { add(2L); }
  };

  boolean isPrimeNumber(long n) {
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

  int run(int upperLimit) {
    for (long n = 3; n <= upperLimit; n += 2) {
      if (isPrimeNumber(n)) {
        primeNumbers.add(n);
      }
    }
    return primeNumbers.size();
  }
}

public class App {

  int count = 0;

  void run(ArgParser parser) {
    System.out.println("Var Type: " + parser.varType);
    System.out.println("Upper limit: " + parser.upperLimit);

    switch(parser.varType) {
    case Integer:
      count = (new PrimeNumberInteger()).run(parser.upperLimit);
      break;
    case Long:
      count = (new PrimeNumberLong()).run(parser.upperLimit);
      break;
    default:
      System.out.println("Error: Unknonwn type: " + parser.varType);
      break;
    }

    System.out.println("Count: " + count);
  }

  public static void main(String[] args) {
    var parser = new ArgParser(args);
    var app = new App();
    app.run(parser);
  }
}
