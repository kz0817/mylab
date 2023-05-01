import java.util.Iterator;
import java.util.Arrays;

enum VarType {
  Integer,
  Long;
}

class ArgParser {
  int upperLimit = 0;
  VarType varType = VarType.Integer;

  ArgParser(String[] args) {
    var it = Arrays.asList(args).iterator();
    while (it.hasNext()) {
      final var option = it.next();
      switch (option) {
        case "-u":
          upperLimit = Integer.parseInt(it.next());
          break;
        case "--long":
          varType = VarType.Long;
          break;
        default:
          System.out.println("Unknown option: " + option);
      }
    }
  }
};
