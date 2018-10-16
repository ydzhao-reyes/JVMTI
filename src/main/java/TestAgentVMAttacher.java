package main.java;

import com.sun.tools.attach.VirtualMachine;

public class TestAgentVMAttacher {

    public static void main(String[] args) throws Exception {
        String pid = "2886"; // 12345改成你想attach的java进程id
        String agentPath = "/Users/zhaoyuandong/Desktop/JVMTI/jvmti_agent/src/main/cpp/libtestagent.so"; // path_to_agent为你编译的agent的路径
        VirtualMachine virtualMachine = com.sun.tools.attach.VirtualMachine.attach(pid);
        virtualMachine.loadAgentPath(agentPath, null);
        virtualMachine.detach();
    }
}
