#define _GNU_SOURCE
#include <sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/mount.h>
#include <unistd.h>
#include <limits.h>

#include "netns.h"

int join(char *join_path) {
  char buf[PATH_MAX];
  if (netns_link(buf, sizeof(buf)) < 0) {
    perror("readlink: failed to read link");
    return -1;
  }
  printf("original netns link: %s\n", buf);

  if (netns_join(join_path) == -1) {
      perror("netns_join: failed to join");
      return -1;
  }

  char cur[PATH_MAX];
  if (netns_link(cur, sizeof(cur)) < 0) {
    perror("readlink: failed to read new link");
    return -1;
  }
  printf("new netns link: %s\n", cur);

  return 1;
}

int main(int argc, char *argv[]) {

  char *mount_path = NULL;
  char *join_path = NULL;
  
  // need at least one argument
  if (argc == 1) {
      printf("no argument provided\n");
      printf("usage: -m <mount_path> -j <join_path>\n");
      return -1;
  }

  int option;
  while ((option = getopt(argc, argv, "m:j:")) != -1) {
    switch (option) {
    case 'm':
      mount_path = optarg;
      break;
    case 'j':
      join_path = optarg;
      break;
    default:
      // argument must be one of the above.
      printf("no argument provided\n");
      printf("usage: -m <mount_path> -j <join_path>\n");
      return -1;
    }
  }

  if (join_path) {
      return join(join_path);
  }

  char buf[PATH_MAX];
  if (netns_link(buf, sizeof(buf)) < 0) {
    perror("readlink: failed to read link");
    return -1;
  }
  printf("original netns link: %s\n", buf);

  if (unshare(CLONE_NEWNET) == -1) {
    perror("unshare: failed to create net ns");
    return -1;
  }

  char cur[PATH_MAX];
  if (netns_link(cur, sizeof(cur)) < 0) {
    perror("readlink: failed to read new link");
    return -1;
  }
  printf("new netns link: %s\n", cur);

  if (strcmp(buf, cur) == 0) {
    perror("strcmp: unshare did not create new namespace");
    return -1;
  }

  // attempt bind mount to /var/run/netns
  if (netns_mount(mount_path) < 0) {
    perror("strcmp: failed to bind mount");
    return -1;
  }

  read(STDIN_FILENO, 0, 1);

  return 0;
}
