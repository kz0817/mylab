
class SomeFunc {
    int plus(int a, int b)
    {
        return a + b;
    }

    public static void main(String[] args) {
        int a = 5;
        int b = 8;
        SomeFunc sf = new SomeFunc();
        int x = sf.plus(a, b);
        System.out.printf("Func(%d, %d) => %d%n", x);
    }
}
