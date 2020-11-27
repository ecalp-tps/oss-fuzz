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

#include "fuzzer_temp_file.h"

const int skip_size = 1;
const int min_size = skip_size + 1;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < min_size) {
        return 0;
    }
    GError *error = NULL;
    GdkPixbuf *pixbuf;
    unsigned int rot_amount = ((unsigned int) data[0]) % 4;
    size_t new_size = size - skip_size;
    uint8_t *new_data = (uint8_t *) calloc(new_size, sizeof(uint8_t));
    memcpy(new_data, &data[skip_size], new_size);

    char *tmpfile = fuzzer_get_tmpfile(new_data, new_size);
    pixbuf = gdk_pixbuf_new_from_file(tmpfile, &error);
    if (error != NULL) {
        free(new_data);
        g_clear_error(&error);
        fuzzer_release_tmpfile(tmpfile);
        return 0;
    }

    char *buf = (char *) calloc(new_size + 1, sizeof(char));
    memcpy(buf, new_data, new_size);
    buf[new_size] = '\0';

    gdk_pixbuf_get_width(pixbuf);
    gdk_pixbuf_get_height(pixbuf);
    gdk_pixbuf_get_bits_per_sample(pixbuf);
    gdk_pixbuf_scale(pixbuf, pixbuf,
            0, 0, 
            gdk_pixbuf_get_width(pixbuf) / 4, 
            gdk_pixbuf_get_height(pixbuf) / 4,
            0, 0, 0.5, 0.5,
            GDK_INTERP_NEAREST);
    pixbuf = gdk_pixbuf_rotate_simple(pixbuf, rot_amount * 90);
    gdk_pixbuf_set_option(pixbuf, buf, buf);
    gdk_pixbuf_get_option(pixbuf, buf);

    free(new_data);
    free(buf);
    g_object_unref(pixbuf);
    fuzzer_release_tmpfile(tmpfile);
    return 0;
}
