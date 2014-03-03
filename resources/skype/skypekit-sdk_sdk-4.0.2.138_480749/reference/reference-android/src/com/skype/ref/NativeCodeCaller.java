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

package com.skype.ref;

import java.lang.ref.WeakReference;

import android.view.Surface;

public class NativeCodeCaller {

	private int mNativeContext; // Accessed by native methods

	static {
		System.loadLibrary("SkypeKitVideoDemo");
	}

	protected void finalize() {
		native_release();
	}

	NativeCodeCaller() {
		native_setup(new WeakReference<NativeCodeCaller>(this));
	}

	private native final void native_setup(Object caller_this);

	private native final void native_release();

	public native final boolean startAVHosts();

	public native final boolean stopAVHosts();

	public native final void setVideoDisplay(Surface surface);

	public native final void setPreviewDisplay(Surface surface);

	public native final boolean initAVHosts(int mode);
}
