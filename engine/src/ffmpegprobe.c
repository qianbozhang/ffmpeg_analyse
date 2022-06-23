#include "ffmpegprobe.h"
#include "tool/convertRgbaToWebp.h"

#include <stdio.h>
#include <string.h>

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"


#define PSECOND         1000000000
#define PMSECOND        1000000
#define URI_BUFF_MAX    1024

typedef uint64_t                    ClockTime;
#define CLOCK_TIME_NONE             ((ClockTime)-1)
#define CLOCK_TIME_IS_VALID(time)   (((ClockTime)(time)) != CLOCK_TIME_NONE)
#define TIME_FORMAT                 "u:%02u:%02u.%09u"


#define TIME_ARGS(t)                                                \
        CLOCK_TIME_IS_VALID (t) ?                                   \
        (uint) (((ClockTime)(t)) / (PSECOND * 60 * 60)) : 99,       \
        CLOCK_TIME_IS_VALID (t) ?                                   \
        (uint) ((((ClockTime)(t)) / (PSECOND * 60)) % 60) : 99,     \
        CLOCK_TIME_IS_VALID (t) ?                                   \
        (uint) ((((ClockTime)(t)) / PSECOND) % 60) : 99,            \
        CLOCK_TIME_IS_VALID (t) ?                                   \
        (uint) (((ClockTime)(t)) % PSECOND) : 999999999

typedef struct _ProbeContext
{
    /* ffmpeg */
    AVFormatContext* fmtCtx;

    /* key information */
    int64_t  start_time;    //the file start time
    int64_t  duration;      //file duration
    int      probetrack;    //the probe track

    /* webp tool */
    WHandle whandle;

    /* user space */
    char uri[URI_BUFF_MAX];
}ProbeContext;


ProbeHnalde createHandle(void)
{
    ProbeContext* handle = (ProbeContext*) malloc(sizeof(ProbeContext));
    if(handle) {
        memset(handle, 0, sizeof(ProbeContext));
        handle->fmtCtx = NULL;
        handle->start_time = 0;
        handle->duration = 0;
        handle->probetrack = -1;

        memset(handle->uri, 0, URI_BUFF_MAX);
    }

    return handle;
}

int destroyHandle(ProbeHnalde* handle)
{
    if(*handle)
    {
        free(*handle);
        *handle = NULL;
    }

    return 0;
}

int probeInit(ProbeHnalde handle, const char* uri)
{
    int  ret = 0;
    char errMsg[512] = {0};

    if(handle == NULL || uri == NULL)
    {
        return -1;
    }

    strncpy(handle->uri, uri, strlen(uri));
    printf("setting uri:%s \n", handle->uri);

    ret = avformat_open_input(&handle->fmtCtx, handle->uri, NULL, NULL);
    if (ret < 0)
    {
        av_strerror(ret, errMsg, 512);
        printf("open file fail(%d):%s.\n", ret, errMsg);
        return -1;
    }

    ret = avformat_find_stream_info(handle->fmtCtx, NULL);
    if (ret < 0)
    {
        av_strerror(ret, errMsg, 512);
        printf("find stream info fail(%d):%s.\n", ret, errMsg);
        return -1;
    }

    //find first video track
    for (int i = 0; i < handle->fmtCtx->nb_streams; i++)
    {
        if (handle->fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            handle->probetrack = i;
            break;
        }
    }

    //duration
    AVRational bq = {1, PMSECOND};
    AVStream *ps = handle->fmtCtx->streams[handle->probetrack];
    if(ps->duration > 0) {
        handle->duration = av_rescale_q(ps->duration, ps->time_base, bq);
    }
    //start time
    if(ps->start_time > 0) {
        handle->start_time = av_rescale_q(ps->start_time, ps->time_base, bq);
    }

    printf("probe index:%d, start time:%" TIME_FORMAT " duration:%" TIME_FORMAT " \n",
        handle->probetrack, TIME_ARGS(handle->start_time), TIME_ARGS(handle->duration));

    return 0;
}

int probeDeInit(ProbeHnalde handle)
{
    if(handle == NULL)
    {
        return -1;
    }

    if(handle->fmtCtx) {
        avformat_close_input(&handle->fmtCtx);
        avformat_free_context(handle->fmtCtx);
    }

    return 0;
}

