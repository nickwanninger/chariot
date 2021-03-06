#include <sched.h>
#include <syscall.h>

int sys::setpgid(int pid, int pgid) {
  /* TODO: EACCESS (has execve'd) */
  if (pgid < 0) return -EINVAL;

  if (pgid == 0) pgid = curproc->pid;

  process *proc = NULL;
  if (pid == 0) {
    pid = curproc->pid;
    proc = curproc;  // optimization
  } else {
    // if there is a process `pid`, the main thread must exist
    // with the same `tid`.
    auto *thd = thread::lookup(pid);
    if (thd == NULL) return -ESRCH;
    // TODO: permissions?
    proc = &thd->proc;
  }
  if (proc == NULL) return -ESRCH;

  /* TODO: EPERM (move proc to another session (?)) */



  proc->datalock.lock();
  proc->pgid = pgid;
  proc->datalock.unlock();

  return 0;
}

int sys::getpgid(int pid) {
  process *proc = NULL;
  if (pid == 0) {
    pid = curproc->pid;
    proc = curproc;  // optimization
  } else {
    // if there is a process `pid`, the main thread must exist
    // with the same `tid`.
    auto *thd = thread::lookup(pid);
    if (thd == NULL) return -ESRCH;
    // TODO: permissions?
    proc = &thd->proc;
  }
  if (proc == NULL) return -ESRCH;

  /* TODO: probably lock the process data? */
  return proc->pgid;
}
