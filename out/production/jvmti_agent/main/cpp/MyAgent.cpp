
#include

#include

#include

#include "jvmti.h"



static jvmtiEnv *gb_jvmti = NULL;

static jvmtiCapabilities gb_capa;

static jrawMonitorID gb_lock;



static void          enter_critical_section(jvmtiEnv *jvmti)

{

    jvmtiError error;

    error = jvmti->RawMonitorEnter(gb_lock);

}



static void          exit_critical_section(jvmtiEnv *jvmti)

{

    jvmtiError error;

    error = jvmti->RawMonitorExit(gb_lock);

}



static void JNICALL callbackException(jvmtiEnv *jvmti_env, JNIEnv* env, jthread thr, jmethodID method, jlocation location, jobject exception, jmethodID catch_method, jlocation catch_location)

{

            enter_critical_section(gb_jvmti);

            {

                        char *name,*sig,*gsig;

                        jvmtiError  error  = gb_jvmti->GetMethodName(method, &name, &sig, &gsig);

                        if (error  != JVMTI_ERROR_NONE)

                        {

          printf("ERROR:GetMethodName!\n");

          return;

                        }

                        if(strcmp(name,"main")==0)

                        {

                                    printf("In Agent: Got an exception from Method: %s\n" ,name );

                        }

            }

            exit_critical_section(gb_jvmti);

}



JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved)

{

            jvmtiError error;

            jvmtiEventCallbacks callbacks;



            jint result = jvm->GetEnv((void **) &gb_jvmti, JVMTI_VERSION_1_0);

            if(result != JNI_OK || gb_jvmti==NULL)

            {

                        printf("ERROR: Unable to access JVMTI!");

                        return JNI_ERR;

            }



            memset(&gb_capa,0,sizeof(jvmtiCapabilities));

    gb_capa.can_signal_thread = 1;

    gb_capa.can_get_owned_monitor_info = 1;

    gb_capa.can_generate_method_entry_events = 1;

    gb_capa.can_generate_exception_events = 1;

    gb_capa.can_generate_vm_object_alloc_events = 1;

    gb_capa.can_tag_objects = 1;



    error = gb_jvmti->AddCapabilities(&gb_capa);

            if(error != JVMTI_ERROR_NONE)

            {

                        printf("ERROR: Can't get JVMTI capabilities");

                        return JNI_ERR;

            }



            memset(&callbacks,0,sizeof(jvmtiEventCallbacks));

            callbacks.Exception = &callbackException;



            error = gb_jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));

            if(error != JVMTI_ERROR_NONE)

            {

                        printf("ERROR: Can't set jvmti callback!");

                        return JNI_ERR;

            }



            error = gb_jvmti->CreateRawMonitor("agent data", &gb_lock);

            if(error != JVMTI_ERROR_NONE)

            {

                        printf("ERROR: Can't create raw monitor!");

                        return JNI_ERR;

            }



 //   error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE,JVMTI_EVENT_VM_INIT, (jthread)NULL);

    error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE,JVMTI_EVENT_EXCEPTION, (jthread)NULL);



            return JNI_OK;

}



JNIEXPORT void JNICALL        Agent_OnUnload(JavaVM *vm)

{

            //

}