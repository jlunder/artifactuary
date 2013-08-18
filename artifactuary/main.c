#include "artifactuary.h"

#include "gles2_harness.h"
#include "spi_harness.h"

int main(int argc, char* argv[])
{
    if(argc > 1 && strcmp(argv[1], "test") == 0) {
        gles2_harness_main();
    }
    else {
        spi_harness_main();
    }
    
    return 0;
}
