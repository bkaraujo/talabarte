#include "talabarte/core/graphics/lifecycle.h"

b8 graphics_initialize() {
    return TRUE;
}

void graphics_compute_size(enum AspectRatio ratio, enum Resolution resolution, i32* width, i32* height) {
    switch (resolution) {
        case RESOLUTION_HD : *height =  720; break;
        case RESOLUTION_FHD: *height = 1080; break;
        case RESOLUTION_QHD: *height = 1440; break;
        case RESOLUTION_UHD: *height = 2160; break;
    }

    switch (ratio) {
        case ASPECT_RATIO_4x3 : *width = (*height *  4) / 3; break;
        case ASPECT_RATIO_16x9: *width = (*height * 16) / 9; break;
        case ASPECT_RATIO_21x9: *width = (*height * 21) / 9; break;
    }
}

void graphics_terminate() {

}