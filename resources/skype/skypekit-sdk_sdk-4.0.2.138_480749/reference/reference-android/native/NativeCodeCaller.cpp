/**
 * Copyright (C) 2011, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

#define LOG_TAG "NativeCodeCaller"

#include "jni.h"
#include "nativehelper/JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
#include <surfaceflinger/Surface.h>

#include "utils/Log.h"
#include "utility.h"

#include "VideohostMain.h"
#include "PCMHostMain.h"

// ----------------------------------------------------------------------------
static const char* const kClassPathName = "com/skype/ref/NativeCodeCaller";

using namespace android;

struct fields_t {
	jfieldID	context; // Refers to this.mContext
	jfieldID	surface; // Refers to android.view.Surface
};

static fields_t fields;
static Mutex sLock;

class JNINativeContext : public RefBase {
public:
	JNINativeContext(JNIEnv* env, jobject weak_this, jclass clazz);
	~JNINativeContext();

	bool init(int mode) {
		videohost->Init((VideohostMain::runMode)mode);
		pcmhost->Init();
		return true;
	}
	bool start() {
		videohost->Start();
		pcmhost->Start();
		return true;
	}
	bool stop() {
		if (videohost->IsRunning()) {
			videohost->Stop();
		}
		if (pcmhost->IsRunning()) {
			pcmhost->Stop();
		}
		return true;
	}
	bool setVideoSurface(sp<Surface> surface) {
		return videohost->setVideoSurface(surface);
	}
	bool setPreviewSurface(sp<Surface> surface) {
		return videohost->setPreviewSurface(surface);;
	}
private:
	jobject	 mNativeJObjectWeak;	// Weak reference to java object
	jclass mNativeJClass;		// Strong reference to java class

	VideohostMain * videohost;
	PCMHostMain * pcmhost;
};

JNINativeContext::JNINativeContext(JNIEnv* env, jobject weak_this, jclass clazz) :
	videohost(NULL),
	pcmhost(NULL)
{
	mNativeJObjectWeak = env->NewGlobalRef(weak_this);
	mNativeJClass = (jclass)env->NewGlobalRef(clazz);

	videohost = new VideohostMain();
	pcmhost = new PCMHostMain();
}

JNINativeContext::~JNINativeContext() {
	stop();
	delete videohost;
	videohost = NULL;
	delete pcmhost;
	pcmhost = NULL;
}

static void JNICALL native_setup(JNIEnv *env, jobject thiz, jobject weak_this) {
	F_LOG;

	jclass clazz = env->GetObjectClass(thiz);
	if (clazz == NULL) {
		jniThrowException(env, "java/lang/RuntimeException", "Can't find com/skype/ref/NativeCodeCaller");
		return;
	}

	sp<JNINativeContext> context = new JNINativeContext(env, weak_this, clazz);
	context->incStrong(thiz);

	// Save context in opaque field
	env->SetIntField(thiz, fields.context, (int)context.get());
}

static void JNICALL native_release(JNIEnv *env, jobject thiz) {
	F_LOG;

	JNINativeContext* context = NULL;
	{
		Mutex::Autolock _l(sLock);
		context = reinterpret_cast<JNINativeContext*>(env->GetIntField(thiz, fields.context));

		// Make sure we do not attempt to callback on a deleted Java object.
		env->SetIntField(thiz, fields.context, 0);
	}
	// Clean up if release has not been called before
	if (context != NULL) {
		// Remove context to prevent further Java access
		context->decStrong(thiz);
	}
}

JNINativeContext * getContext(JNIEnv *env, jobject & thiz) {
	return reinterpret_cast<JNINativeContext*>(env->GetIntField(thiz, fields.context));
}

static jboolean JNICALL initAVHosts(JNIEnv *env, jobject thiz, jint mode) {
	F_LOG;
	Mutex::Autolock _m(sLock);
	return getContext(env, thiz)->init(mode) ? JNI_TRUE : JNI_FALSE;
}

static jboolean JNICALL startAVHosts(JNIEnv *env, jobject thiz) {
	F_LOG;
	Mutex::Autolock _m(sLock);

	return getContext(env, thiz)->start() ? JNI_TRUE : JNI_FALSE;
}

static jboolean JNICALL stopAVHosts(JNIEnv *env, jobject thiz) {
	F_LOG;
	Mutex::Autolock _m(sLock);
	return getContext(env, thiz)->stop() ? JNI_TRUE : JNI_FALSE;
}

static void JNICALL setVideoDisplay(JNIEnv *env, jobject thiz, jobject jSurface) {
	F_LOG;
	Mutex::Autolock _m(sLock);
	sp<Surface> surface = NULL;
	if (jSurface != NULL) {
		surface = reinterpret_cast<Surface*>(env->GetIntField(jSurface, fields.surface));
		getContext(env, thiz)->setVideoSurface(surface);
	}
}

static void JNICALL setPreviewDisplay(JNIEnv *env, jobject thiz, jobject jSurface) {
	F_LOG;
	Mutex::Autolock _m(sLock);
	sp<Surface> surface = NULL;
	if (jSurface != NULL) {
		surface = reinterpret_cast<Surface*>(env->GetIntField(jSurface, fields.surface));
		if (surface->isValid()) {
			Log("valid surface");
		} else {
			Log("invalid surface");
		}
		getContext(env, thiz)->setPreviewSurface(surface);
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static JNINativeMethod methods[] = {
	// Name			,Signature,		,funcPtr
	{"native_setup"		,"(Ljava/lang/Object;)V",(void*)native_setup},
	{"native_release"	,"()V"					,(void*)native_release},
	{"initAVHosts"	,"(I)Z"					,(void*)initAVHosts},
	{"startAVHosts"	,"()Z"					,(void*)startAVHosts},
	{"stopAVHosts"	,"()Z"					,(void*)stopAVHosts},
	{"setVideoDisplay"	,"(Landroid/view/Surface;)V",(void*)setVideoDisplay},
	{"setPreviewDisplay","(Landroid/view/Surface;)V",(void*)setPreviewDisplay},
};

struct field {
	const char *class_name;
	const char *field_name;
	const char *field_type;
	jfieldID *jfield;
};

// Lifted from android_hardware_Camera
static int find_fields(JNIEnv *env, field *fields, int count) {
	for (int i = 0; i < count; i++) {
		field *f = &fields[i];
		jclass clazz = env->FindClass(f->class_name);
		if (clazz == NULL) {
			Log("Can't find %s", f->class_name);
			return -1;
		}

		jfieldID field = env->GetFieldID(clazz, f->field_name, f->field_type);
		if (field == NULL) {
			Log("Can't find %s.%s", f->class_name, f->field_name);
			return -1;
		}

		*(f->jfield) = field;
	}

	return 0;
}

#ifndef ANDROID_VIEW_SURFACE_JNI_ID
#define ANDROID_VIEW_SURFACE_JNI_ID "mSurface"
#endif

int register_the_class(JNIEnv *_env) {
	// Cache the fieldIDs
	field fields_to_find[] = {
		{ kClassPathName		, "mNativeContext", "I", &fields.context },
		{ "android/view/Surface", ANDROID_VIEW_SURFACE_JNI_ID, "I", &fields.surface }
	};

	Log("find_fields: %d fields", lenof_x(fields_to_find));
	if (find_fields(_env, fields_to_find, NELEM(fields_to_find)) < 0) {
		return -1;
	}

	Log("registerNativeMethods: %d functions", lenof_x(methods));
	return android::AndroidRuntime::registerNativeMethods(
		_env, kClassPathName, methods,
		lenof_x(methods)
		);
}

// Boilerplate OnLoad
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	F_LOG;

	JNIEnv* env = 0;
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		goto bail;
	}
	assert(env != 0);

	if (register_the_class(env) < 0) {
		goto bail;
	}

	result = JNI_VERSION_1_4;
bail:
	return result;
}
