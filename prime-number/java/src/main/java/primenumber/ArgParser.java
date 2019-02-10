import java.util.Iterator;
import java.util.Arrays;

class ArgParser {
  int upperLimit = 0;

  ArgParser(String[] args) {
    var it = Arrays.asList(args).iterator();
    while (it.hasNext()) {
      final var option = it.next();
      switch (option) {
        case "-u":
          upperLimit = Integer.parseInt(it.next());
          break;
        default:
          System.out.println("Unknown option: " + option);
      }
    }
  }
};
