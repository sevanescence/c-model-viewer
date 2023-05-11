#ifndef MK_RESOURCES_H
#define MK_RESOURCES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *filestream_open(const char *path, const char *mode) {
#ifdef _WIN32
    FILE *fp = NULL;
    errno_t errn = fopen_s(&fp, path, mode);
    if (errn) {
        return NULL;
    }
    return fp;
#else
    return fopen(path, mode);
#endif
}

/// Note: ftell might not work outside of Windows. TODO: Patch as necessary
size_t filestream_get_content_length(FILE *const fp) {
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

typedef struct {
    char *          data;
    size_t          size;
} Resource;

Resource mk_resource_from_relative_path(const char *relative_path) {
    Resource source = {};

    FILE *fp = filestream_open(relative_path, "rb");

    size_t length = filestream_get_content_length(fp);
    source.data = calloc(1, length * sizeof (char));
    source.size = length;

    fread(source.data, sizeof (char), source.size, fp);

    fclose(fp);

    return source;
}

const char *mk__resource_folder_loc = "./resources";

/// Gets a resource located in resources folder. Path should be relative to resources folder. TODO: Patch
/// non-compatability due to Windows-specific string format call.
Resource mk_get_resource(const char *relative_path) {
    /// TODO: Abstract concat functionality
    /// I hate magic numbers, but the +2 is for the null terminator and the file extra file separator. Note the
    /// "TODO" comment above.
    size_t bufsize = sizeof (char) * (strlen(relative_path) + strlen(mk__resource_folder_loc) + 2);
    char *path = calloc(1, bufsize);
    sprintf_s(path, bufsize, "%s/%s", mk__resource_folder_loc, relative_path);

    Resource source = mk_resource_from_relative_path(path);

    free(path);

    return source;
}

void mk_resource_destruct(Resource *self) {
    free(self->data);
    self->data = NULL;
    self->size = 0;
}

#endif