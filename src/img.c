#include "img.h"
#include <png.h>
#include <stdlib.h>


struct Image *image_alloc(const char *src)
{
    struct Image *img = malloc(sizeof(struct Image));
    char header[8];

    FILE *fp = fopen(src, "rb");

    if (!fp)
    {
        fprintf(stderr, "[read_png_file]: Unable to read file '%s'.\n", src);
        exit(EXIT_FAILURE);
    }

    fread(header, 1, 8, fp);

    if (png_sig_cmp((unsigned char*)header, 0, 8))
    {
        fprintf(stderr, "[read_png_file]: Incorrect header in file '%s'.\n", src);
        exit(EXIT_FAILURE);
    }

    img->ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!img->ptr)
    {
        fprintf(stderr, "[read_png_file]: png_create_read_struct failed.\n");
        exit(EXIT_FAILURE);
    }

    img->info = png_create_info_struct(img->ptr);

    if (setjmp(png_jmpbuf(img->ptr)))
    {
        fprintf(stderr, "[read_png_file]: init_io failed");
        exit(EXIT_FAILURE);
    }

    png_init_io(img->ptr, fp);
    png_set_sig_bytes(img->ptr, 8);

    png_read_info(img->ptr, img->info);

    img->w = png_get_image_width(img->ptr, img->info);
    img->h = png_get_image_height(img->ptr, img->info);

    png_read_update_info(img->ptr, img->info);

    if (setjmp(png_jmpbuf(img->ptr)))
    {
        fprintf(stderr, "[read_png_file] Error during read_image\n");
        exit(EXIT_FAILURE);
    }

    img->rows = malloc(sizeof(png_bytep) * img->h);

    for (int y = 0; y < img->h; ++y)
        img->rows[y] = malloc(png_get_rowbytes(img->ptr, img->info));

    png_read_image(img->ptr, img->rows);

    fclose(fp);

    return img;
}


void image_free(struct Image *img)
{
    for (int i = 0; i < img->h; ++i)
        free(img->rows[i]);

    free(img->rows);
    png_destroy_read_struct(&img->ptr, &img->info, 0);
    free(img);
}


SDL_Color image_at(struct Image *img, int x, int y)
{
    png_byte *ptr = &(img->rows[y][x * 4]);
    return (SDL_Color){ ptr[0], ptr[1], ptr[2] };
}

