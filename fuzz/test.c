/* How to fuzz:

clang main.c -O2 -g -fsanitize=address,fuzzer -o fuzz
cp -r data temp
./fuzz temp/ -dict=gltf.dict -jobs=12 -workers=12

*/
#include "cgltf.h"
#include <stdio.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    cgltf_options options = {cgltf_file_type_invalid};
    cgltf_data* data = NULL;
    cgltf_result res = cgltf_parse(&options, Data, Size, &data);
    if (res == cgltf_result_success)
    {
        cgltf_validate(data);
        cgltf_free(data);
    }
    return 0;
}

int main(int argc, char **argv)
{
    FILE *f;
    char *buf = NULL;
    long siz_buf;

    if(argc < 2)
    {
        fprintf(stderr, "no input file\n");
        goto err;
    }

    f = fopen(argv[1], "rb");
    if(f == NULL)
    {
        fprintf(stderr, "error opening input file %s\n", argv[1]);
        goto err;
    }

    fseek(f, 0, SEEK_END);

    siz_buf = ftell(f);
    rewind(f);

    if(siz_buf < 1) goto err;

    buf = (char*)malloc((size_t)siz_buf);
    if(buf == NULL)
    {
        fprintf(stderr, "malloc() failed\n");
        goto err;
    }

    if(fread(buf, (size_t)siz_buf, 1, f) != 1)
    {
        fprintf(stderr, "fread() failed\n");
        goto err;
    }

    (void)LLVMFuzzerTestOneInput((uint8_t*)buf, (size_t)siz_buf);

err:
    free(buf);

    return 0;
}
