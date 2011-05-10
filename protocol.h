#ifndef H__tftt__protocol__
#define H__tftt__protocol__

#include <stdint.h>

#define TFTT_REGULAR_FILE 0
#define TFTT_DIRECTORY    1
#define TFTT_SYMLINK      2

/* TODO: How to do absolute symlinks? */

typedef struct file_header_t {
  int mode;        /* permissions only */
  char* filename;
  uint64_t size;
  
  time_t mtime;
  
  mode_t mode;
  char* user_name;
  char* group_name;

  uint32_t crc32;
} file_header_t;

void write_header(int fd, file_header_t* hdr);
file_header_t* read_header(int fd);

#endif
