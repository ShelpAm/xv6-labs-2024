struct stat;

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int *);
int pipe(int *);
int write(int, void const *, int);
int read(int, void *, int);
int close(int);
int kill(int);
int exec(char const *, char **);
int open(char const *, int);
int mknod(char const *, short, short);
int unlink(char const *);
int fstat(int fd, struct stat *);
int link(char const *, char const *);
int mkdir(char const *);
int chdir(char const *);
int dup(int);
int getpid(void);
char *sbrk(int);
int sleep(int);
int uptime(void);
int trace(int);

// ulib.c
int stat(char const *, struct stat *);
char *strcpy(char *, char const *);
void *memmove(void *, void const *, int);
char *strchr(char const *, char c);
int strcmp(char const *, char const *);
void fprintf(int, char const *, ...) __attribute__((format(printf, 2, 3)));
void printf(char const *, ...) __attribute__((format(printf, 1, 2)));
char *gets(char *, int max);
uint strlen(char const *);
void *memset(void *, int, uint);
int atoi(char const *);
int memcmp(void const *, void const *, uint);
void *memcpy(void *, void const *, uint);

// umalloc.c
void *malloc(uint);
void free(void *);
