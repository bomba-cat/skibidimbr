#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <png.h>

#define IMAGE_SIZE 22

// Structure to hold RGB color values
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

// BIOS color attributes
Color bios_colors[] = {
    {0, 0, 0},         // 0: Black
    {0, 0, 255},       // 1: Blue
    {0, 255, 0},       // 2: Green
    {0, 255, 255},     // 3: Cyan
    {255, 0, 0},       // 4: Red
    {255, 0, 255},     // 5: Magenta
    {165, 42, 42},     // 6: Brown
    {192, 192, 192},   // 7: Light Gray
    {128, 128, 128},   // 8: Dark Gray
    {0, 0, 255},       // 9: Light Blue
    {0, 255, 0},       // A: Light Green
    {0, 255, 255},     // B: Light Cyan
    {255, 0, 0},       // C: Light Red
    {255, 0, 255},     // D: Light Magenta
    {255, 255, 0},     // E: Yellow
    {255, 255, 255}    // F: White
};

int find_nearest_bios_color(Color pixel) {
    int nearest_color_index = 0;
    double nearest_distance = INFINITY;

    for (int i = 0; i < sizeof(bios_colors) / sizeof(bios_colors[0]); i++) {
        double distance = sqrt(pow(pixel.r - bios_colors[i].r, 2) +
                               pow(pixel.g - bios_colors[i].g, 2) +
                               pow(pixel.b - bios_colors[i].b, 2));
        if (distance < nearest_distance) {
            nearest_distance = distance;
            nearest_color_index = i;
        }
    }

    return nearest_color_index;
}

Color* read_png(const char* filename, int* width, int* height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File opening failed");
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return NULL;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);

    // Ensure the image is 22x22
    if (*width != IMAGE_SIZE || *height != IMAGE_SIZE) {
        printf("Image must be 22x22 pixels!\n");
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return NULL;
    }

    png_set_expand_gray_1_2_4_to_8(png);
    png_set_palette_to_rgb(png);
    png_set_tRNS_to_alpha(png);
    png_read_update_info(png, info);

    Color* image = malloc(IMAGE_SIZE * IMAGE_SIZE * sizeof(Color));
    png_bytep row = malloc(png_get_rowbytes(png, info));

    for (int y = 0; y < IMAGE_SIZE; y++) {
        png_read_rows(png, &row, NULL, 1);
        for (int x = 0; x < IMAGE_SIZE; x++) {
            image[y * IMAGE_SIZE + x].r = row[x * 4];
            image[y * IMAGE_SIZE + x].g = row[x * 4 + 1];
            image[y * IMAGE_SIZE + x].b = row[x * 4 + 2];
        }
    }

    free(row);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return image;
}

int main() {
    char filename[256];
    printf("Enter the path to a 22x22 PNG file: ");
    scanf("%s", filename);

    int width, height;
    Color* image = read_png(filename, &width, &height);
    if (!image) {
        return 1;
    }

    // Output 4-bit color representation
    unsigned char output[IMAGE_SIZE][IMAGE_SIZE / 2]; // Each byte holds 2 pixels
    for (int i = 0; i < IMAGE_SIZE; i++) {
        for (int j = 0; j < IMAGE_SIZE; j++) {
            int color_index = find_nearest_bios_color(image[i * IMAGE_SIZE + j]);
            if (j % 2 == 0) {
                output[i][j / 2] = color_index << 4; // Store first pixel in high nibble
            } else {
                output[i][j / 2] |= color_index; // Store second pixel in low nibble
            }
        }
    }

    // Print output representation
    for (int i = 0; i < IMAGE_SIZE; i++) {
        for (int j = 0; j < IMAGE_SIZE / 2; j++) {
            printf("%02X ", output[i][j]);
        }
        printf("\n");
    }

    free(image);
    return 0;
}

