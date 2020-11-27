// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

const int width = 10;
const int height = 20;
const int rowstride = width * 4;

const int skip_size = 1;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (!(size >= (width * height * 4) + skip_size)) {
        return 0;
    }
    const gchar *profile;
    GdkPixbuf *pixbuf, *tmp;
    GBytes *bytes;
    unsigned int rot_amount = ((unsigned int) data[0]) % 4;
    size_t new_size = size - skip_size;
    uint8_t *new_data = (uint8_t *) calloc(new_size, sizeof(uint8_t));
    memcpy(new_data, &data[skip_size], new_size);

    bytes = g_bytes_new(new_data, new_size);
    pixbuf = g_object_new(GDK_TYPE_PIXBUF,
            "width", width,
            "height", height,
            "rowstride", rowstride,
            "bits-per-sample", 8,"n-channels", 3,
            "has-alpha", FALSE,
            "pixel-bytes", bytes,
            NULL);
    if (pixbuf == NULL) {
        g_object_unref(pixbuf);
        g_bytes_unref(bytes);
        free(new_data);
        return 0;
    }
    gdk_pixbuf_scale(pixbuf, pixbuf,
            0, 0, 
            gdk_pixbuf_get_width(pixbuf) / 4, 
            gdk_pixbuf_get_height(pixbuf) / 4,
            0, 0, 0.5, 0.5,
            GDK_INTERP_NEAREST);
    tmp = gdk_pixbuf_rotate_simple(pixbuf, rot_amount * 90);
    g_object_unref(tmp);
    tmp = gdk_pixbuf_flip(pixbuf, TRUE);
    g_object_unref(tmp);
    tmp = gdk_pixbuf_composite_color_simple(pixbuf,
            gdk_pixbuf_get_width(pixbuf) / 4, 
            gdk_pixbuf_get_height(pixbuf) / 4,
            GDK_INTERP_NEAREST,
            128,
            8,
            G_MAXUINT32,
            G_MAXUINT32/2);
    g_object_unref(tmp);

    char *buf = (char *) calloc(new_size + 1, sizeof(char));
    memcpy(buf, new_data, new_size);
    buf[new_size] = '\0';

    gdk_pixbuf_set_option(pixbuf, buf, buf);
    profile = gdk_pixbuf_get_option(pixbuf, buf);
    tmp = gdk_pixbuf_new_from_data(gdk_pixbuf_get_pixels(pixbuf),
            GDK_COLORSPACE_RGB,
            FALSE,
            gdk_pixbuf_get_bits_per_sample(pixbuf),
            gdk_pixbuf_get_width(pixbuf), 
            gdk_pixbuf_get_height(pixbuf),
            gdk_pixbuf_get_rowstride(pixbuf),
            NULL,
            NULL);
    tmp = gdk_pixbuf_flip(tmp, TRUE);

    free(new_data);
    free(buf);
    g_bytes_unref(bytes);
    g_object_unref(tmp);
    g_object_unref(pixbuf);
    return 0;
}
