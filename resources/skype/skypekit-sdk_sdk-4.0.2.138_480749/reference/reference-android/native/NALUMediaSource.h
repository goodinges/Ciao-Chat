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

struct NALUMediaSource : public MediaSource {

	struct observer {
		virtual void onBufferAvailable() = 0;
	} & mObserver ;

	NALUMediaSource(int w, int h, observer & obs);
	virtual ~NALUMediaSource();
	const char * dumpFile;
	FILE * mFh;
	const char * dumpRenderFile;
	FILE * mFRendered;

	// MediaSource interface
	android::status_t start(MetaData * params );
	android::status_t stop();
	sp<MetaData> getFormat();
	status_t read(MediaBuffer **, const ReadOptions * options = NULL);

	// RTP unwrapping
	struct AnxBCallback: public H264RtpToAnxb::AnxbFrameCallback {
		NALUMediaSource &ref;
		AnxBCallback(NALUMediaSource &_ref): ref(_ref) {}
#ifdef SKYPEKIT31
		void TransmitAnxbFrame(uint8_t *data, uint32_t len);
#else
		void TransmitAnxbFrame(const uint8_t *data, uint32_t len, int timestamp);
#endif
	};
	void SubmitPacket(const uint8_t *data, uint32_t len);
private:
	H264RtpToAnxb	mRtpToAnxb;
	AnxBCallback	mAnxBCallback;
	void addNAL(uint8_t *data, uint32_t len);

	bool mStarted;
	android::MediaBufferGroup *mBufferGroup;
	uint8_t *mSrcBuffer;
	uint32_t mSrcBufferLen;
	uint32_t NALLen;

	android::sp<MetaData> mMeta;
	mutable android::Mutex mLock;
	mutable android::Condition mSignalBuffer;
};
