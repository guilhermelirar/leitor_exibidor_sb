public class Teste {
    private final static int ANSWER = 42; 
    private final static float PI = 3.14f;
    private final static long LONG = -42L;

    private int somaBoba(int a, int b) {
      return a + b;
    }

    private static void hello() {
      System.out.println("Hello world!");
    }

    public static void main(String[] args) {
        hello();
        System.out.println(ANSWER);
        System.out.println(PI);
        System.out.println(LONG);
    }
}
