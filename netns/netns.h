#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

// write the result of 'readlink' on the current PID's 'net' namespace
// symbolic link into @buf.
//
// a null byte is appended to the read link so @buf can be treated as a string.
//
// @buf should be large enough to hold the string 'net:[{MAX_INODE}]\0'
//
// returns:
//  n < 0 if error
//  number of bytes written to buf, including null byte, on success.
int netns_link(char *buf, ssize_t size);

// mount the current '/proc/[pid]/ns/net' to the given path at @mount_path.
//
// @mount_path should be a null terminated string and represent a valid
// filesystem path to a regular file.
//
// the regular file must exist before this method is ran.
//
// 
int netns_mount(char *mount_path);

int netns_join(char *mount_path);
