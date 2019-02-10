package kz;
import java.util.stream.Stream;
import java.util.Optional;

public class Triangle {
    private String name;

    public Triangle(String _name) {
        name = _name;
    }

    public double sum(double a, double b) {
        return a * b / 2;
    }

    public double streamSum(Stream<Double> data) {
        Optional<Double> sumOpt = data.reduce((a, b) -> a + b);
        return sumOpt.get();
    }
}
