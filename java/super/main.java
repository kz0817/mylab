public class App {

    class A {
        void hello() {
            System.out.println("Class A: Hello");
        }
    }

    class B extends A {
        @Override
        void hello() {
            super.hello();
        }
    }

    class C extends B {
        @Override
        void hello() {
            System.out.println("Class C: Hello");
        }
    }

    void run() {
        var b = new B();
        var c = new C();
        b.hello();
        c.hello();
    }

    public static void main(String[] args) {
        (new App()).run();
    }
}
