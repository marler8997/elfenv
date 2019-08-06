#define stdout 1

#define print(fd, msg) write(fd, msg, sizeof(msg) - 1);

void* syscall(
    void* num,
    void* param1,
    void* param2,
    void* param3,
    void* param4,
    void* param5
);

static void* write(int fd, char const* msg, unsigned length)
{
  return (void*)syscall(
                        (void*)1, // SYS_write
                        (void*)fd,
                        (void*)msg,
                        (void*)length,
                        0,
                        0);
}

static unsigned strlen2(char *s)
{
  unsigned i = 0;
  for (; s[i]; i++) { }
  return i;
}

static void string_reverse(char *start, char *limit)
{
    for (;;)
    {
        limit--;
        if (limit <= start)
            break;
        char temp = start[0];
        start[0] = limit[0];
        limit[0] = temp;
        start++;
    }
}
static char* sprint_unsigned(char *buffer, unsigned i)
{
  if (i == 0) {
    buffer[0] = '0';
    return buffer + 1;
  }
  char *start = buffer;
  do {
    buffer[0] = (i % 10) + '0';
    buffer++;
    i /= 10;
  } while (i);
  string_reverse(start, buffer);
  return buffer;
}
static int print_unsigned(int fd, unsigned i)
{
  char buffer[50];
  char *end = sprint_unsigned(buffer, i);
  write(fd, buffer, end - buffer);
}

int main(int argc, char *argv[], char *envp[])
{
  for (unsigned i = 0; i < argc; i++) {
    print(stdout, "arg[");
    print_unsigned(stdout, i);
    print(stdout, "] = '");
    write(stdout, argv[i], strlen2(argv[i]));
    print(stdout, "'\n");
  }
  for (unsigned i = 0;; i++) {
    char *env = envp[i];
    if (!env)
      break;
    write(stdout, env, strlen2(env));
    print(stdout, "\n");
  }
  return 0;
}
