package ann;

public class Ann {
    @MyAnnotation
    private void run(final String name, final int birthYear) {
        System.out.format("%s, born in %d\n", name, birthYear);
    }

    public static void main(String[] args) {
        final var ann = new Ann();
        ann.run("Mao", 2005);
    }
}
