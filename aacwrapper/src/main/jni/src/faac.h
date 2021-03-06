#ifndef __H_FAAC_H__
#define __H_FAAC_H__

#include<iostream>
extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/samplefmt.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/log.h"
#include "libavutil/mathematics.h"
}

using namespace std;
namespace panda
{
class aac
{
public:
	aac();
	~aac();
public:
	int static flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index);
	static int pcm2aac(const std::string& pcmfile, const std::string& aacfile);

	static int pcm2aacdetail(const std::string& pcmfile, const std::string& aacfile);

};
}


#endif //__H_FAAC_H__