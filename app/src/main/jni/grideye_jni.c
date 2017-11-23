//
// Created by HowardJHLi on 2017/4/26.
//
/*******************************************************************************
	include file
*******************************************************************************/

#include "com_fihtdc_grideye_MainActivity.h"
#include <stdio.h>
#include <pthread.h>
#include "grideye/header/common.h"
#include "grideye/header/human_detection.h"

/*******************************************************************************
 * program of JNI
 ******************************************************************************/

// Test Hello JNI
JNIEXPORT jstring JNICALL
Java_com_evenwell_grideye_MainActivity_getMsgFromJni(JNIEnv *env, jobject instance) {
    return (*env)->NewStringUTF(env, "HELLO JNI");
}

// Test bool
JNIEXPORT jboolean JNICALL Java_com_evenwell_grideye_MainActivity_getStatusFromJni
        (JNIEnv *env, jobject instance) {
//	if(bAMG_PUB_SMP_InitializeHumanDetectLv3Sample()){
//		LOGD("start detect human");
//		//return bAMG_PUB_SMP_ExecuteHumanDetectLv3Sample();
//	}
    return (FALSE);
}

JNIEXPORT void JNICALL
Java_com_evenwell_grideye_MainActivity_sendByteArrayToJNI(JNIEnv *env, jobject instance,
                                                        jbyteArray data_) {
    jbyte *data = (*env)->GetByteArrayElements(env, data_, NULL);
    jsize arrayLength = (*env)->GetArrayLength(env, data_);
    int i;
    for (i = 0; i < arrayLength; i++) {
        LOGD("data %d: %d", i, data[i]);
    }

    (*env)->ReleaseByteArrayElements(env, data_, data, 0);
}


JNIEXPORT void JNICALL
Java_com_evenwell_grideye_Grideye_1NDK_initializeHumanDetect(JNIEnv *env, jobject instance) {

    HumanDetect_Initialize();
    setConfigParameter(0);
}



JNIEXPORT void JNICALL
Java_com_evenwell_grideye_Grideye_1NDK_runHumanDetectionThresh(JNIEnv *env, jobject instance,
                                                            jfloat diffTemp_thresh,
                                                            jint labeling_thresh,
                                                            jint human_thresh,
                                                            jbyteArray data_) {
    jbyte *data = (*env)->GetByteArrayElements(env, data_, NULL);

    // TODO
    int i;
    UCHAR *ausWork[128];

    /* get 128 bytes raw data */
    for (i = 5; i < 133; i++) {
        ausWork[i - 5] = data[i];
        LOGD("[howard] ausWork %d: %d", i - 5, ausWork[i - 5]);
    }

    //HumanDetect_Measure_forDll(data_);

    (*env)->ReleaseByteArrayElements(env, data_, data, 0);

}

JNIEXPORT void JNICALL
Java_com_evenwell_grideye_Grideye_1NDK_startHumanDetection(JNIEnv *env, jobject instance,
                                                        jshortArray jsDataArray) {
    jshort *data = (*env)->GetShortArrayElements(env, jsDataArray, NULL);

// TODO
    int i;

    short ausWork[64];

    /* get 128 bytes raw data */
    for (i = 0; i < 64; i++) {
        ausWork[i] = data[i];
    }

    LOGD("[howard] Java_com_evenwell_grideye_BluetoothLeService_startHumanDetection");

    HumanDetect_Measure(ausWork);

    (*env)->ReleaseShortArrayElements(env, jsDataArray, data, 0);

}


JNIEXPORT void JNICALL
Java_com_evenwell_grideye_Grideye_1NDK_setDetectionConfig(JNIEnv *env, jobject instance,
                                                         jbyteArray jbDataArray) {

    jbyte *data = (*env)->GetByteArrayElements(env, jbDataArray, NULL);

 // TODO
    int i;

    u_short usData[10];
    float fData[1];

    fData[0]  =  (float)(data[0]&0xff) +  ((data[1]&0xff) * 0.01f);

    usData[0] = (u_short) data[2]&0xff;
    usData[1] = (u_short) data[3]&0xff;
    usData[2] = (data[4] << 8 | data[5]);
    usData[3] = (u_short) data[6]&0xff;
    usData[4] = (u_short) data[7]&0xff;
    usData[5] = (u_short) data[8]&0xff;
    usData[6] = (u_short) data[9]&0xff;
    usData[7] = (u_short) data[10]&0xff;
    usData[8] = (u_short) data[11]&0xff;
    usData[9] = (u_short) data[12]&0xff;
//
//
//    LOGD("[[ble setting]] Java_com_evenwell_grideye_Grideye_1NDK_setDetectionConfig: %d", sizeof(usData)/sizeof(short));
//    LOGD("[[ble setting]] float data %d: %f", i, fData[i]);
//
    for (i = 0; i < sizeof(usData)/sizeof(short); i++) {
        LOGD("[[ble setting]] ushort data %d: %d", i, usData[i]);
    }

    setConfigParameter_InputUShort(KEY_LABELING_THRESH, usData[0]);
    setConfigParameter_InputUShort(KEY_FNMV_THRESH, usData[1]);
    setConfigParameter_InputUShort(KEY_EDGE_THRESH, usData[2]);
    setConfigParameter_InputUShort(KEY_HUMAN_FRAME_THRESH, usData[3]);
    setConfigParameter_InputUShort(KEY_BED_LEFT, usData[4]);
    setConfigParameter_InputUShort(KEY_BED_RIGHT, usData[5]);
    setConfigParameter_InputUShort(KEY_LEFT_RANGE, usData[6]);
    setConfigParameter_InputUShort(KEY_RIGHT_RANGE, usData[7]);
    setConfigParameter_InputUShort(KEY_TOP_RANGE, usData[8]);
    setConfigParameter_InputUShort(KEY_BOTTOM_RANGE, usData[9]);
    setConfigParameter_InputFloat(KEY_DIFF_TEMP_THRESH, fData[0]);
//
    (*env)->ReleaseByteArrayElements(env, jbDataArray, data, 0);

    LOGD("[[ble setting]] ushort data");

}


/* define call back Global variable */
JavaVM *g_jvm = NULL;  //Get g_jvm from jni main thread use env->GetJavaVM(&g_jvm);
jobject g_obj = NULL;  //Where the java function exist (some Activity).



JNIEXPORT void JNICALL
Java_com_evenwell_grideye_Grideye_1NDK_setJNIEnv(JNIEnv *env, jobject instance) {
// TODO
    (*env)->GetJavaVM(env,&g_jvm);
    g_obj = (*env)->NewGlobalRef(env,instance);
}



/*******************************************************************************/
/* program of JNI call back function */
/*******************************************************************************/


void *doCallback_isHuman(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_isHuman", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_humanArea(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_humanArea", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_humanNum(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_humanNum", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_detectNum(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_detectNum", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_backTempState(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_backTempState", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_centerX(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_centerX", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_centerY(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_centerY", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}

void *doCallback_maxX(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_maxX", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_maxY(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_maxY", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_minX(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_minX", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_minY(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_minY", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_alertStatus(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_alertStatus", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}

void *doCallback_detailStatus(int arg){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_detailStatus", "(I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}

void *doCallback_filterData(short arg[IMG_SZ]){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_filterData", "([I)V");
    if (mid == NULL) {
        LOGD("GetMethodID() Error.....");
        goto error;
    }
    LOGD("GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arg);
    LOGD("CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}
void *doCallback_detailStatusCode(int arg[4]){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;
    pthread_detach(pthread_self());
    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: [doCallback]AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("[doCallback]AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("[doCallback]FindClass() Error.....");
        goto error;
    }
    LOGD("[doCallback]FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_detailStatusCode", "([I)V");
    if (mid == NULL) {
        LOGD("[doCallback]GetMethodID() Error.....");
        goto error;
    }
    LOGD("[doCallback]GetMethodID() pass ,arg[0] = %d, arg[1] = %d, arg[2] = %d", arg[0], arg[1], arg[2]);


    jintArray arr = (*env)->NewIntArray(env, 4);


    (*env)->SetIntArrayRegion(env, arr, 0, 4, arg);
    //(*env)->SetByteArrayRegion(env, arr, 0, len, (jbyte*)buf);

    LOGD("[doCallback]CallVoidMethod() ready");

    (*env)->CallVoidMethod(env, g_obj, mid , arr);
    LOGD("[doCallback]CallVoidMethod() pass");

    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("DetachCurrentThread() pass");
    }

    pthread_exit(0);
}

void *doCallback_humanOutputImg(short arg[IMG_SZ]){
    jclass cls;
    jmethodID mid;
    JNIEnv *env;

    pthread_detach(pthread_self());

    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK){
        LOGD("%s: Howard AttachCurrentThread() failed", __FUNCTION__);
        return NULL;
    }
    LOGD("Howard AttachCurrentThread() pass");
    cls = (*env)->GetObjectClass(env, g_obj);
    if(cls == NULL){
        LOGD("Howard FindClass() Error.....");
        goto error;
    }
    LOGD("FindClass() pass");
    mid = (*env)->GetMethodID(env, cls, "callbackFromLib_humanOutputImg", "([S)V");
    if (mid == NULL) {
        LOGD("Howard GetMethodID() Error.....");
        goto error;
    }


    jshortArray arr = (*env)->NewShortArray(env, IMG_SZ);
    (*env)->SetShortArrayRegion(env, arr, 0, IMG_SZ, arg);


    LOGD("Howard GetMethodID() pass");
    (*env)->CallVoidMethod(env, g_obj, mid , arr);
    LOGD("Howard CallVoidMethod() pass");


    error:
    if((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGD("%s: Howard DetachCurrentThread() failed", __FUNCTION__);
    }else{
        LOGD("Howard DetachCurrentThread() pass");
    }

    pthread_exit(0);
}

void call_back_to_java(int code, int dataType){
    LOGD("call_back_to_java");
    pthread_t pt;

//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_attr_setstacksize(&attr, 32768);


    if (dataType == HUMAN_IN_BED) {
        pthread_create(&pt, NULL, &doCallback_isHuman, code);
    } else if (dataType == HUMAN_AREA){
        pthread_create(&pt, NULL, &doCallback_humanArea, code);
    } else if (dataType == HUMAN_NUM){
        pthread_create(&pt, NULL, &doCallback_humanNum, code);
    } else if (dataType == DETECT_NUM){
        pthread_create(&pt, NULL, &doCallback_detectNum, code);
    } else if (dataType == BACK_TEMP_STATE){
        pthread_create(&pt, NULL, &doCallback_backTempState, code);
    } else if (dataType == CENTER_X){
        pthread_create(&pt, NULL, &doCallback_centerX, code);
    } else if (dataType == CENTER_Y){
        pthread_create(&pt, NULL, &doCallback_centerY, code);
    } else if (dataType == ALERT_STATUS){
        pthread_create(&pt, NULL, &doCallback_alertStatus, code);
    } else if (dataType == DETAIL_STATUS){
        pthread_create(&pt, NULL, &doCallback_detailStatus, code);
    } else if (dataType == MAX_X){
        pthread_create(&pt, NULL, &doCallback_maxX, code);
    } else if (dataType == MAX_Y){
        pthread_create(&pt, NULL, &doCallback_maxY, code);
    } else if (dataType == MIN_X){
        pthread_create(&pt, NULL, &doCallback_minX, code);
    } else if (dataType == MIN_Y){
        pthread_create(&pt, NULL, &doCallback_minY, code);
    }
    //pthread_attr_destroy(&attr);
}

void call_back_to_java2(int code[4], int dataType){
    LOGD("call_back_to_java2");
    pthread_t pt;
    if (dataType == DETAIL_STATUS_CODE){
        pthread_create(&pt, NULL, &doCallback_detailStatusCode, code);
    }
}

//pthread_t pt_h;
void call_back_to_java_humanOutputImg(short code[IMG_SZ]){
    LOGD("call_back_to_java_humanOutputImg");
    pthread_t pt;

    //pthread_attr_t attr;
    //pthread_attr_init(&attr);
    //pthread_attr_setstacksize(&attr, 32768);

    pthread_create(&pt, NULL, &doCallback_humanOutputImg, code);

    //pthread_attr_destroy(&attr);


}

