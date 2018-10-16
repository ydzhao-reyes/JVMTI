package main.callback;

public class A  implements  CallBack{

    B b = new B();

    /**
     * 响应回调函数
     */
    @Override
    public void solve() {
        System.out.println("problem is solved!");
    }

    /**
     * 登记回调函数
     */
    public void askQuestion(){
        System.out.println("ask b solve the problem");
        new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("A want to do another things");
            }
        });
        this.b.call(this);
    }

    public static  void  main(String[] args){
        A a  = new A();
        a.askQuestion();
    }



}
