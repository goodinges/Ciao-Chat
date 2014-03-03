
package com.skype.api;

import java.util.HashMap;

import com.skype.api.Skype;

public abstract class SkypeObject {

	protected int mObjectId;
	protected HashMap<Integer, Object> mPropCache;
	protected Skype skype;
	
	protected SkypeObject(int oid, Skype skype) {
		mObjectId = oid;
		mPropCache = new HashMap<Integer, Object>();
		this.skype = skype;
		skype.object_list.put(oid, this);
	}
	
	public int getOid() {
		return mObjectId;
	}
	
	public void close() {
		flush_cache();
		skype.object_list.remove(mObjectId);
	}
	
	public abstract Object GetPropertyAsEnum(int propid);
	
	void flush_cache()
	{
		mPropCache.clear();
	}
}

