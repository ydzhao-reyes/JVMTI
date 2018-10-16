package main.java;

public class Foo {
    public  void bar() throws InterruptedException {
        Thread.sleep(5000);
        System.out.println("Excuting Foo.bar()");
    }

    public  void baz(){
        System.out.println("Excuting Foo.baz()");
    }
}
