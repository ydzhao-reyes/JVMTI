package main.java;

public class ThreadMain {
    public static void main(String[] args) throws InterruptedException {
        System.out.println("JVMTI agent Test start");
        int i = 0;
        while (true) {
            Thread.sleep(1000);
            i++;
        }
    }
}