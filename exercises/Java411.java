class Fib implements Runnable {
    private int num;
    private int[] fib;

    public Fib(int num, int[] fib) {
        this.num = num;
        this.fib = fib;
    }

    public void run() {
        fib[0] = 0;
        fib[1] = 1;
        for ( int i = 2; i < num; ++i ) {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }
}

public class Java411 {
    public static void main(String[] args) {
        if ( args.length == 0 ) {
            System.out.println("Error! Usage: java Java411 <int>");
        } else {
            int num = Integer.parseInt(args[0]);
            if ( num < 0 ) {
                System.out.println("Error! Integer should be >= 0");
            }

            int[] fib = new int[1000];
            Thread thread = new Thread(new Fib(num, fib));
            thread.start();
            try {
                thread.join();
                for ( int i = 0; i < num; ++i ) {
                    System.out.println("" + fib[i]);
                }
            } catch ( Exception e ) {
                e.printStackTrace();
            }
        }
    }
}
