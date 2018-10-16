#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <jvmti.h>

void  JNICALL callbackClassFileLoadHook(jvmtiEnv *jvmti,
                                  JNIEnv *jni,
                                  jclass class_being_redefined,
                                  jobject loader,
                                  const char *name,
                                  jobject protection_domain,
                                  jint class_data_len,
                                  const unsigned char *class_data,
                                  jint *new_class_data_len,
                                  unsigned char **new_class_data) {


    jvmtiError error;
    if(strcmp(name, "main/java/Foo") == 0){
      printf("loaded class name=%s\n ", name);

      jint size = class_data_len;
      *new_class_data_len = size;

      //为新的class字节码数据区分配JVM内存
      error = (*jvmti)->Allocate(jvmti, size, new_class_data);

      memset(*new_class_data, 0, size);


      if(error != JVMTI_ERROR_NONE) {
        printf(stderr, "ERROR: Unable to AddCapabilities JVMTI");
      }

      int i;
      //遍历旧的字节码字符，将E字符修改为P
      for(i = 0; i < size; i++){
        if(class_data[i] == 'E'){
          (*new_class_data)[i] = 'P';
        } else {
          (*new_class_data)[i] = class_data[i];
        }
      }
    }
}

JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM* vm, char *options, void *reserved){

  jvmtiEnv *jvmti = NULL;
  jvmtiError error;
  //获取JVMTI environment
    error = (*vm)->GetEnv(vm, (void **)&jvmti, JVMTI_VERSION_1_1);
    if (error != JNI_OK) {
        fprintf(stderr, "ERROR: Couldn't get JVMTI environment");
        return JNI_ERR;
    }

    //注册功能
    jvmtiCapabilities capabilities;
    (void)memset(&capabilities, 0, sizeof(jvmtiCapabilities));

    capabilities.can_generate_all_class_hook_events  =  1 ;
    capabilities.can_retransform_classes             =  1 ;
    capabilities.can_retransform_any_class           =  1 ;


    error = (*jvmti)->AddCapabilities(jvmti, &capabilities);

    if(error != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: Unable to AddCapabilities JVMTI");
      return  error;
    }
  //设置JVM事件回调
    jvmtiEventCallbacks callbacks;
    callbacks.ClassFileLoadHook = &callbackClassFileLoadHook;
    error = (*jvmti)->SetEventCallbacks(jvmti, &callbacks, (jint)sizeof(callbacks));
    if(error != JVMTI_ERROR_NONE) {
      fprintf(stderr, "ERROR: Unable to SetEventCallbacks JVMTI!");
      return error;
    }

    //设置事件通知
    error = (*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, (jthread)NULL);
    if(error != JVMTI_ERROR_NONE) {
      fprintf(stderr, " ERROR: Unable to SetEventNotificationMode JVMTI!");
      return  error;
    }

    return JNI_OK;
  }

  JNIEXPORT jint JNICALL
  Agent_OnAttach(JavaVM* vm, char *options, void *reserved){
    //do nothing
  }


  JNIEXPORT void JNICALL
  Agent_OnUnload(JavaVM *vm){
    //do nothing
  }