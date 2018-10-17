#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <jvmti.h>

void JNICALL callbackThreadStart(jvmtiEnv
*jvmti,
JNIEnv *env, jthread
thr){
jvmtiThreadInfo info;
jlong cpu_time;
jvmtiFrameInfo frames[5];
jint count;
jvmtiError err;
//获取启动线程信息
(*jvmti)->
GetThreadInfo(jvmti, thr,
&info);
//获取启动线程CPU-Time
(*jvmti)->
GetThreadCpuTime(jvmti, thr,
&cpu_time);

printf("thread-%s start...\n", info.name);
printf("thread-%s cpu-time : %ld nanos\n", info.name, cpu_time);


}

void JNICALL
callbackThreadEnd(jvmtiEnv
*jvmti,
JNIEnv *env,
        jthread
thr){
jvmtiThreadInfo info;
(*jvmti)->
GetThreadInfo(jvmti,
        thr,
&info);
printf("thread-%s end...\n", info.name);
}

JNIEXPORT jint

JNICALL
Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {

    jvmtiEnv * jvmti = NULL;
    jvmtiError error;

    //获取JVMTI environment
    error = (*vm)->GetEnv(vm, (void **) &jvmti, JVMTI_VERSION_1_1);
    if (error != JNI_OK) {
        fprintf(stderr, "ERROR: Couldn't get JVMTI environment");
        return JNI_ERR;
    }

    //注册功能
    jvmtiCapabilities capabilities;
    (void) memset(&capabilities, 0, sizeof(jvmtiCapabilities));


    capabilities.can_get_current_thread_cpu_time = 1;
    capabilities.can_get_thread_cpu_time = 1;

    error = (*jvmti)->AddCapabilities(jvmti, &capabilities);

    if (error != JVMTI_ERROR_NONE) {
        fprintf(stderr, "ERROR: Unable to AddCapabilities JVMTI");
        return error;
    }


    //jvmtiEventThreadStart ThreadStart;
    //jvmtiEventThreadEnd ThreadEnd;
    //设置JVM事件回调
    jvmtiEventCallbacks callbacks;
    callbacks.ThreadStart = &callbackThreadStart;
    callbacks.ThreadEnd = &callbackThreadEnd;
    error = (*jvmti)->SetEventCallbacks(jvmti, &callbacks, (jint)
    sizeof(callbacks));
    if (error != JVMTI_ERROR_NONE) {
        fprintf(stderr, "ERROR: Unable to SetEventCallbacks JVMTI!");
        return error;
    }

    //设置事件通知
    error = (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_THREAD_START, (jthread)NULL);
    if (error != JVMTI_ERROR_NONE) {
        fprintf(stderr, " ERROR: Unable to SetEventNotificationMode JVMTI!");
        return error;
    }

    error = (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_THREAD_END, (jthread)NULL);
    if (error != JVMTI_ERROR_NONE) {
        fprintf(stderr, " ERROR: Unable to SetEventNotificationMode JVMTI!");
        return error;
    }

    return JNI_OK;
}

JNIEXPORT jint

JNICALL
Agent_OnAttach(JavaVM *vm, char *options, void *reserved) {
    //do nothing
}


JNIEXPORT void JNICALL
Agent_OnUnload(JavaVM
*vm){
//do nothing
}