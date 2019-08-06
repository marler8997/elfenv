#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define errorf(fmt,...) fprintf(stderr, "Error: " fmt "\n",##__VA_ARGS__)

struct string {
  const char *ptr;
  size_t length;
};
static struct string make_string(const char *ptr, size_t length)
{
  struct string s;
  s.ptr = ptr;
  s.length = length;
  return s;
}

// Returns: string with NULL ptr on error, logs erros
static struct string try_read_file(const char *filename)
{
  struct stat file_stat;
  if (0 != stat(filename, &file_stat)) {
    errorf("stat '%s' failed, errno=%d", filename, errno);
    return make_string(NULL, 0);
  }

  char *contents = malloc(file_stat.st_size);
  if (!contents) {
    errorf("malloc %llu failed, errno=%d", (unsigned long long)file_stat.st_size, errno);
    return make_string(NULL, 0);
  }

  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    errorf("open '%s' failed, errno=%d", filename, errno);
    free(contents);
    return make_string(NULL, 0);
  }

  size_t total_read = 0;
  while (total_read < file_stat.st_size) {
    ssize_t last_read = read(fd, contents + total_read, file_stat.st_size - total_read);
    if (last_read < 0) {
      errorf("read failed, total_read=%llu, expected=%llu, errno=%d",
             (unsigned long long)total_read, (unsigned long long)file_stat.st_size, errno);
      close(fd);
      free(contents);
      return make_string(NULL, 0);
    }
    total_read += last_read;
  }
  close(fd);
  return make_string(contents, file_stat.st_size);
}

static void usage()
{
  fprintf(stderr, "interp2 EXE INTERP\n");
}
int main(int argc, char *argv[])
{
  if (argc == 0) {
    errorf("0 arguments");
    return 1;
  }
  argc--;argv++;
  {
    int new_argc = 0;
    for (int i = 0; i < argc; i++) {
      char *arg = argv[i];
      if (arg[0] != '-') {
        argv[new_argc++] = arg;
      } else {
        errorf("unknown option '%s'", arg);
        return 1;
      }
    }
    argc = new_argc;
  }

  if (argc == 0) {
    usage();
    return 1;
  }
  if (argc == 1) {
    errorf("missing interpreter argument from command line");
    return 1;
  }
  if (argc > 2) {
    errorf("too many arguments");
    return 1;
  }
  char *exe_filename = argv[0];
  char *interp_filename = argv[1];

  struct string exe = try_read_file(exe_filename);
  if (!exe.ptr)
    return 1; // error already logged

  errorf("not implemented, read the elf header, check for the interp2 section, move/change it if it already exists, otherwise, add a new section to the end of the file");
  return 1;
}
