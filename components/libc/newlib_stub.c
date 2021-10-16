#include "osal/osal.h"
#include "ln_utils.h"
#include <reent.h>
#include <stdarg.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/unistd.h>

int _execve_r(struct _reent *ptr, const char *name, char *const *argv, char *const *env)
{
    LN_UNUSED(name);
    LN_UNUSED(argv);
    LN_UNUSED(env);

    ptr->_errno = ENOTSUP;
    return -1;
}

int _fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
    LN_UNUSED(ptr);
    LN_UNUSED(fd);
    LN_UNUSED(cmd);
    LN_UNUSED(arg);

    return 0;
}

int _fork_r(struct _reent *ptr)
{
    ptr->_errno = ENOTSUP;

    return -1;
}

int _getpid_r(struct _reent *ptr)
{
    ptr->_errno = ENOTSUP;
    return 0;
}

int _isatty_r(struct _reent *ptr, int fd)
{
    if (fd >= 0 && fd < 3)
    {
        return 1;
    }

    ptr->_errno = ENOTSUP;
    return -1;
}

int _kill_r(struct _reent *ptr, int pid, int sig)
{
    LN_UNUSED(pid);
    LN_UNUSED(sig);

    ptr->_errno = ENOTSUP;
    return -1;
}

int _link_r(struct _reent *ptr, const char *old, const char *new)
{
    LN_UNUSED(old);
    LN_UNUSED(new);

    ptr->_errno = ENOTSUP;
    return -1;
}

_off_t _lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
    LN_UNUSED(ptr);
    LN_UNUSED(fd);
    LN_UNUSED(pos);
    LN_UNUSED(whence);

    return 0;
}

int _mkdir_r(struct _reent *ptr, const char *name, int mode)
{
    LN_UNUSED(ptr);
    LN_UNUSED(name);
    LN_UNUSED(mode);

    return 0;
}

int _open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
    LN_UNUSED(ptr);
    LN_UNUSED(file);
    LN_UNUSED(flags);
    LN_UNUSED(mode);

    return 0;
}

int _close_r(struct _reent *ptr, int fd)
{
    LN_UNUSED(ptr);
    LN_UNUSED(fd);

    return 0;
}

_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    LN_UNUSED(ptr);
    LN_UNUSED(fd);
    LN_UNUSED(buf);
    LN_UNUSED(nbytes);

    return 0;
}

_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    LN_UNUSED(ptr);
    LN_UNUSED(fd);

    log_stdio_write((char *)buf, nbytes);
    return nbytes;
}

int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
    LN_UNUSED(fd);
    LN_UNUSED(pstat);

    ptr->_errno = ENOTSUP;
    return -1;
}

int _rename_r(struct _reent *ptr, const char *old, const char *new)
{
    LN_UNUSED(old);
    LN_UNUSED(new);

    ptr->_errno = ENOTSUP;
    return 0;
}

void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
    LN_UNUSED(incr);

    ptr->_errno = ENOTSUP;
    return NULL;
}

int _stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
    LN_UNUSED(ptr);
    LN_UNUSED(file);
    LN_UNUSED(pstat);

    return 0;
}

int _unlink_r(struct _reent *ptr, const char *file)
{
    LN_UNUSED(ptr);
    LN_UNUSED(file);

    return 0;
}

int _wait_r(struct _reent *ptr, int *status)
{
    LN_UNUSED(status);

    ptr->_errno = ENOTSUP;
    return -1;
}

int _gettimeofday_r(struct _reent *ptr, struct timeval *tv, void *__tzp)
{
    LN_UNUSED(ptr);
    LN_UNUSED(tv);
    LN_UNUSED(__tzp);

    return 0;
}

void *_malloc_r(struct _reent *ptr, size_t size)
{
    LN_UNUSED(ptr);

    void *mem;
    mem = (void *)OS_Malloc(size);

    return mem;
}

void *_realloc_r(struct _reent *ptr, void *old, size_t newlen)
{
    LN_UNUSED(ptr);
    LN_UNUSED(old);
    LN_UNUSED(newlen);

    return NULL;
}

void *_calloc_r(struct _reent *ptr, size_t size, size_t len)
{
    LN_UNUSED(ptr);
    LN_UNUSED(size);
    LN_UNUSED(len);

    return NULL;
}

void _free_r(struct _reent *ptr, void *addr)
{
    LN_UNUSED(ptr);

    if (NULL != addr)
    {
        OS_Free(addr);
    }
}

void _exit(int status)
{
    LN_UNUSED(status);

    while (1);
}

void abort(void)
{
    while (1);
}
