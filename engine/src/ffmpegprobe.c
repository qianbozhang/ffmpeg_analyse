#include "ffmpegprobe.h"
#include "tool/convertRgbaToWebp.h"

#include <stdio.h>
#include <string.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#define URI_BUFF_MAX    1024

//time
#if __GNUC__ > 2 || (__GNUC == 2 && __GNUC_MINOR__ >= 8)
#define G_GNUC_EXTENSION __extension__
#else
#define G_GNUC_EXTENSION
#endif

#define G_GINT64_CONSTANT(val)      (G_GNUC_EXTENSION (val##LL))
#define G_GUINT64_CONSTANT(val)     (G_GNUC_EXTENSION (val##ULL))

/* microseconds per second */
#define G_USEC_PER_SEC              1000000
#define PSECOND                     ((int64_t)(G_USEC_PER_SEC * G_GINT64_CONSTANT(1000)))
#define PMSECOND                    ((int64_t)(PSECOND / G_GINT64_CONSTANT(1000)))
typedef uint64_t                    ClockTime;
#define CLOCK_TIME_NONE             ((ClockTime)-1)
#define CLOCK_TIME_IS_VALID(time)   (((ClockTime)(time)) != CLOCK_TIME_NONE)
#define TIME_FORMAT                 "u:%02u:%02u.%09u"
#define TIME_ARGS(t)                                                                            \
        CLOCK_TIME_IS_VALID (t) ? (uint) (((ClockTime)(t)) / (PSECOND * 60 * 60)) : 99,         \
        CLOCK_TIME_IS_VALID (t) ? (uint) ((((ClockTime)(t)) / (PSECOND * 60)) % 60) : 99,       \
        CLOCK_TIME_IS_VALID (t) ? (uint) ((((ClockTime)(t)) / PSECOND) % 60) : 99,              \
        CLOCK_TIME_IS_VALID (t) ? (uint) (((ClockTime)(t)) % PSECOND) : 999999999


static uint64_t convertTime_ff_to_sys(int64_t time, AVRational base);
static int64_t convertTime_sys_to_ff(uint64_t time, AVRational base);

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
    av_log_set_level(AV_LOG_TRACE);

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

    av_dump_format(handle->fmtCtx, handle->probetrack, handle->uri, 0);

    //duration
    AVRational bq = { 1, PSECOND };
    AVStream *ps = handle->fmtCtx->streams[handle->probetrack];
    printf("time_base: %d / %d. \n", ps->time_base.num, ps->time_base.den);
    if(ps->duration > 0) {
        handle->duration = convertTime_ff_to_sys(ps->duration, ps->time_base);
    }
    //start time
    if(ps->start_time > 0) {
        handle->start_time = convertTime_ff_to_sys(ps->start_time, ps->time_base);
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



int probeShowStreamEntries(ProbeHnalde handle, const char* out_to_path)
{
    if(handle == NULL || out_to_path == NULL) {
        return -1;
    }

    FILE* fout = fopen(out_to_path, "wb+");
    if(fout == NULL) {
        printf("cannot open %s file. \n", out_to_path);
        return -1;
    }


    // AVStream *ps = handle->fmtCtx->streams[handle->probetrack];
    // const FFStream *const sti = ffstream(ps);
    // AVIndexEntry* entries = sti->index_entries;
    // int nb_entries = sti->nb_index_entries;
    // printf("entries: %d \n", nb_entries);

    // char entry_info[1024] = { 0 };

    // for(int i = 0; i < nb_entries; i ++) {
    //     snprintf(entry_info, 1024, "%d: timestamp(%" TIME_FORMAT "), key:%s", i,
    //         TIME_ARGS(convertTime_ff_to_sys(entries[i].timestamp, ps->time_base)),
    //         (entries[i].flags & AVINDEX_KEYFRAME) ? "true": "false");
    //     fwrite(entry_info, 1, 1024, fout);
    //     memset(entry_info, 0, 1024);
    // }

    // if(fout)
    //     fclose(fout);

    return 0;
}



static uint64_t convertTime_ff_to_sys(int64_t time, AVRational base)
{
    uint64_t out;
    if(time == AV_NOPTS_VALUE) {
        out = CLOCK_TIME_NONE;
    } else {
        AVRational bq = { 1, PSECOND };
        out = av_rescale_q(time, base, bq);
    }

    return out;
}

static int64_t convertTime_sys_to_ff(uint64_t time, AVRational base)
{
    int64_t out;
    if(time == CLOCK_TIME_NONE) {
        out = AV_NOPTS_VALUE;
    } else {
        AVRational bq = { 1, PSECOND };
        out = av_rescale_q(time, bq, base);
    }

    return out;
}