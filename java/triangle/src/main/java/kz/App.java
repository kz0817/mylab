package kz;

import java.util.stream.Stream;
import java.util.Arrays;

public class App {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("You should give two parameters.");
            return;
        }

        Triangle triangle = new Triangle("Mr.Tri");
        double a = Double.parseDouble(args[0]);
        double h = Double.parseDouble(args[1]);
        System.out.format("a: %.3f, b: %.3f => Sum of a triangle: %.3f\n",
                          a, h, triangle.sum(a, h));

        System.out.println("");
        Double data[] = {2.5, 1.2, -1.4};
        System.out.println("Raw data: ");
        Arrays.stream(data).forEach(d -> System.out.println(d));
        System.out.format("sum(data): %.3f\n",
            triangle.streamSum(Arrays.stream(data)));
    }
}
