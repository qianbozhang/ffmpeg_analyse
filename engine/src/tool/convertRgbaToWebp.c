#include "tool/convertRgbaToWebp.h"

#include <stdio.h>
#include <string.h>


static int WebPWriter(const uint8_t* data, size_t data_size, const WebPPicture* picture)
{
    if(picture->custom_ptr) {
        FILE* const fout = (FILE*) picture->custom_ptr;
        size_t num = fwrite(data, data_size, 1, fout);
    }

    return 1;
}

WHandle createWebpHandle(void)
{
    WHandle handle = (WHandle) malloc (sizeof(struct WebpHandle));
    if(handle) {
        // memset(handle, 0, sizeof(WebpHandle));
        handle->isValid = 0;
    }

    return handle;
}

int destroyWebpHandle(WHandle *handle)
{
    if(*handle) {
        free(*handle);
        *handle = NULL;
    }

    return 0;
}

int WebpInit(WHandle handle)
{
    if(handle == NULL) {
        return -1;
    }

    if(!WebPConfigInit( &handle->config) || !WebPPictureInit(&handle->picture)) {
        printf("config ot picture init fail. \n");
        handle->isValid = 0;
        return -1;
    }

    handle->config.lossless = 0;
    handle->config.quality = 75;
    handle->config.method = 2;

    if(!WebPValidateConfig(&handle->config)) {
        printf("invalid config. \n");
        handle->isValid = 0;
        return -1;
    }

    handle->picture.use_argb = 1;
    handle->picture.writer = WebPWriter;

    handle->isValid = 1;

    return 0;
}

int WebpDeInit(WHandle handle)
{
    if(handle == NULL) {
        return -1;
    }

    if(handle->isValid) {
        WebPPictureFree(&handle->picture);
    }

    handle->isValid = 0;

    return 0;
}

int resetWebp(WHandle handle)
{
    if(handle == NULL) {
        return -1;
    }

    if(handle->isValid) {
        WebPPictureFree(&handle->picture);
    }

    return 0;
}


int covertToWebp(WHandle handle, const unsigned char* buffer, const int w, const int h, const char* path)
{
    if(handle == NULL || buffer == NULL || path == NULL || w <= 0 || h <= 0) {
        printf("invalid params. \n");
        return -1;
    }

    if(!handle->isValid) {
        printf("cannot convert \n");
        return -1;
    }

    printf("convert rgba buffer[%d x %d] to %s \n", w, h, path);
    FILE* fp = fopen(path, "wb");
    if(!fp) {
        printf("cannot create path: %s. \n", path);
        return -1;
    }

    handle->picture.width = w;
    handle->picture.height = h;
    handle->picture.custom_ptr = (void*)fp;

    if(WebPPictureImportRGBA(&handle->picture, buffer, 4 * w) && WebPEncode(&handle->config, &handle->picture)) {
        printf("convert success \n");
    } else {
        printf("convert fail \n");
    }

    fclose(fp);

    return 0;
}