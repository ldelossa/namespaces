#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mount.h>
#include <fcntl.h>

int netns_link(char buf[], ssize_t size) {
    bzero(buf, size);
    snprintf(buf, size, "/proc/%d/ns/net", getpid());
    int n = readlink(buf, buf, size); 
    if (n < 0) {
        return n;
    }
    buf[n] = '\0';
    return n+1;
}

int netns_mount(char *mount_path) {
    char buf[1024];
    snprintf(buf, sizeof(buf), "/proc/%d/ns/net", getpid());
    printf("bind mounting %s to %s\n", buf, mount_path);
    return mount(buf, mount_path, 0, MS_BIND, 0);
}

int netns_join(char *mount_path) {
    int fd = open(mount_path, O_RDONLY);
    if (fd == -1) {
        perror("netns_join:open: failed to open ns mount path");
        return fd;
    }
    return setns(fd, CLONE_NEWNET);
}
