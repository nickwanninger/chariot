// Generated by tools/gen_syscalls.py. Do not change
#pragma once
#ifdef USERLAND
#include <chariot/awaitfs_types.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/netdb.h>
#else
#include <types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void sysbind_restart();
void sysbind_exit_thread(int code);
void sysbind_exit_proc(int code);
int sysbind_execve(const char* path, const char ** argv, const char ** envp);
long sysbind_waitpid(int pid, int* stat, int options);
int sysbind_fork();
int sysbind_spawnthread(void * stack, void* func, void* arg, int flags);
int sysbind_jointhread(int tid);
int sysbind_prctl(int option, unsigned long arg1, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
int sysbind_open(const char * path, int flags, int mode);
int sysbind_close(int fd);
long sysbind_lseek(int fd, long offset, int whence);
long sysbind_read(int fd, void* buf, size_t len);
long sysbind_write(int fd, void* buf, size_t len);
int sysbind_stat(const char* pathname, struct stat* statbuf);
int sysbind_fstat(int fd, struct stat* statbuf);
int sysbind_lstat(const char* pathname, struct stat* statbuf);
int sysbind_dup(int fd);
int sysbind_dup2(int old, int newfd);
int sysbind_chdir(const char* path);
int sysbind_getcwd(char * dst, int len);
int sysbind_chroot(const char * path);
int sysbind_unlink(const char * path);
int sysbind_ioctl(int fd, int cmd, unsigned long value);
int sysbind_yield();
int sysbind_getpid();
int sysbind_gettid();
int sysbind_getuid();
int sysbind_geteuid();
int sysbind_getgid();
int sysbind_getegid();
void* sysbind_mmap(void * addr, long length, int prot, int flags, int fd, long offset);
int sysbind_munmap(void* addr, size_t length);
int sysbind_mrename(void * addr, char* name);
int sysbind_mgetname(void* addr, char* name, size_t sz);
int sysbind_mregions(struct mmap_region * regions, int nregions);
unsigned long sysbind_mshare(int action, void* arg);
int sysbind_dirent(int fd, struct dirent * ent, int offset, int count);
time_t sysbind_localtime(struct tm* tloc);
size_t sysbind_gettime_microsecond();
int sysbind_usleep(unsigned long usec);
int sysbind_socket(int domain, int type, int proto);
ssize_t sysbind_sendto(int sockfd, const void * buf, size_t len, int flags, const struct sockaddr* addr, size_t addrlen);
ssize_t sysbind_recvfrom(int sockfd, void * buf, size_t len, int flags, const struct sockaddr* addr, size_t addrlen);
int sysbind_bind(int sockfd, const struct sockaddr* addr, size_t addrlen);
int sysbind_accept(int sockfd, struct sockaddr* addr, size_t addrlen);
int sysbind_connect(int sockfd, const struct sockaddr* addr, size_t addrlen);
int sysbind_signal_init(void * sigret);
int sysbind_sigaction(int sig, struct sigaction* new_action, struct sigaction* old);
int sysbind_sigreturn();
int sysbind_sigprocmask(int how, unsigned long set, unsigned long* old_set);
int sysbind_kill(int pid, int sig);
int sysbind_awaitfs(struct await_target * fds, int nfds, int flags, long long timeout_time);
unsigned long sysbind_kshell(char* cmd, int argc, char ** argv, void* data, size_t len);
int sysbind_futex(int* uaddr, int op, int val, int val2, int* uaddr2, int val3);
int sysbind_sysinfo(struct sysinfo * info);
int sysbind_dnslookup(const char * name, unsigned int* ip4);
#ifdef __cplusplus
}
#endif
