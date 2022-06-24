#ifndef _FFMPEG_PROBE_H_
#define _FFMPEG_PROBE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ProbeContext*  ProbeHnalde;

/******************************************************
 @@Name: createHandle
 @@Author:
 @@Date: 2022-06-23
 @@Function: create a handle
 @@Input:
 @@Output:
 @@Return: handle
 @@Medify History:
******************************************************/
ProbeHnalde createHandle(void);

/******************************************************
 @@Name: destroyHandle
 @@Author:
 @@Date: 2022-06-23
 @@Function: destroy a handle
 @@Input: ProbeHnalde* handle
 @@Output:
 @@Return: 0/-1
 @@Medify History:
******************************************************/
int destroyHandle(ProbeHnalde* handle);

/******************************************************
 @@Name: probeInit
 @@Author:
 @@Date: 2022-06-23
 @@Function: init ffmpeg env, and instance AVFormatContext
 @@Input: ProbeHnalde handle
          char* uri
 @@Output:
 @@Return: 0/-1
 @@Medify History:
******************************************************/
int probeInit(ProbeHnalde handle, const char* uri);

/******************************************************
 @@Name: probeDeInit
 @@Author:
 @@Date: 2022-06-23
 @@Function: free AVFormatContext
 @@Input: ProbeHnalde handle
 @@Output:
 @@Return: 0/-1
 @@Medify History:
******************************************************/
int probeDeInit(ProbeHnalde handle);

/******************************************************
 @@Name: probeShowStreamEntries
 @@Author:
 @@Date: 2022-06-23
 @@Function: show stream entries info
 @@Input: ProbeHnalde handle
          char* out_to_path
 @@Output:
 @@Return: 0/-1
 @@Medify History:
******************************************************/
int probeShowStreamEntries(ProbeHnalde handle, const char* out_to_path);

#ifdef __cplusplus
}
#endif

#endif