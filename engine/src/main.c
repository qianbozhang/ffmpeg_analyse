#include <stdio.h>
#include "ffmpegprobe.h"

int main(int argc, char const *argv[])
{
    ProbeHnalde phandle = createHandle();
    char uri[1024] = {0};
    // char filepath = std::string( argv[1] );
    strncpy(uri, argv[1], strlen(argv[1]));
    printf("uri:%s \n", uri);

    //init
    probeInit(phandle, uri);
    

    //deinit
    probeDeInit(phandle);


    //destroy
    destroyHandle(&phandle);

    
    return 0;
}
