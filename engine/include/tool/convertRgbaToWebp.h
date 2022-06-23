#ifndef _CONVERT_RGBA_TO_WEBP_H_
#define _CONVERT_RGBA_TO_WEBP_H_

#include <webp/encode.h>

#ifdef __cplusplus
extern "C" {
#endif


struct WebpHandle {
    WebPConfig config;
    WebPPicture picture;

    int isValid;
};

typedef struct WebpHandle* WHandle;


/******************************************************
 @@Name: createWebpHandle
 @@Author:
 @@Date: 2022-06-23
 @@Function: create a handle
 @@Input:
 @@Output:
 @@Return: handle
 @@Medify History:
******************************************************/
WHandle createWebpHandle(void);

/******************************************************
 @@Name: destroyWebpHandle
 @@Author:
 @@Date: 2022-06-23
 @@Function: destroy a handle
 @@Input: WHandle *handle
 @@Output:
 @@Return: handle
 @@Medify History:
******************************************************/
int destroyWebpHandle(WHandle *handle);

/******************************************************
 @@Name: WebpInit
 @@Author:
 @@Date: 2022-06-23
 @@Function: init webp tools
 @@Input: WHandle handle
 @@Output:
 @@Return: handle
 @@Medify History:
******************************************************/
int WebpInit(WHandle handle);

/******************************************************
 @@Name: WebpDeInit
 @@Author:
 @@Date: 2022-06-23
 @@Function: deinit webp tools
 @@Input: WHandle handle
 @@Output:
 @@Return: handle
 @@Medify History:
******************************************************/
int WebpDeInit(WHandle handle);

/******************************************************
 @@Name: resetWebp
 @@Author:
 @@Date: 2022-06-23
 @@Function: reset webp tools
 @@Input: WHandle handle
 @@Output:
 @@Return: handle
 @@Medify History:
******************************************************/
int resetWebp(WHandle handle);

/******************************************************
 @@Name: resetWebp
 @@Author:
 @@Date: 2022-06-23
 @@Function: reset webp tools
 @@Input: WHandle handle
          unsigned char* buffer
          int w
          int h
          char* path
 @@Output:
 @@Return: handle
 @@Medify History:
******************************************************/
int covertToWebp(WHandle handle, const unsigned char* buffer, const int w, const int h, const char* path);



#ifdef __cplusplus
}
#endif
#endif