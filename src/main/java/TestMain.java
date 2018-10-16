package main.java;

public class TestMain {
    public static void main(String args[]) throws InterruptedException {
        Thread[] threads = new Thread[5];
        Foo foo = new Foo();
        foo.bar();
        foo.baz();
        for (int i = 0; i < threads.length; i++) {
            threads[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    System.out.println(Thread.currentThread().getName());
                }
            });
        }

        for(Thread thread: threads){
            thread.start();
            thread.join();
        }
    }

}
