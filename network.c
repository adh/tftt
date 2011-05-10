#include "network.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <signal.h>

int tcp_connect(char* hostname,
                char* service){
  struct addrinfo hints;
  struct addrinfo* res;
  struct addrinfo* i;
  int ret;
  int fd;


  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
  hints.ai_protocol = 0;

  ret = getaddrinfo(hostname, service, &hints, &res);
  if (ret != 0){
    fprintf(stderr, "getaddrinfo: %s",
            gai_strerror(ret));
  }

  for (i = res; i != NULL; i = i ->ai_next){
    fd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
    if (fd == -1){
      continue;
    }
    
    if (connect(fd, i->ai_addr, i->ai_addrlen) != -1){
      freeaddrinfo(res);
      return fd;
    }
    close(fd);
  }
  freeaddrinfo(res);
  fprintf(stderr, "No usable address for %s:%s\n", hostname, service);
  exit(1);
}

int tcp_bind(char* hostname,
             char* service){
  struct addrinfo hints;
  struct addrinfo* res;
  struct addrinfo* i;
  int val = 1;
  int ret;
  int fd;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
  hints.ai_protocol = 0;

  ret = getaddrinfo(hostname, service, &hints, &res);
  if (ret != 0){
    fprintf(stderr, "getaddrinfo: %s",
            gai_strerror(ret));
  }

  for (i = res; i != NULL; i = i ->ai_next){
    fd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
    if (fd == -1){
      continue;
    }
    val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

#ifdef IPV6_V6ONLY
    if (i->ai_family == AF_INET6){
      /* Assume we want both v6 _AND_ v4 clients, on many systems this
       * is disabled by default */
      val = 0;
      setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &val, sizeof(val));
    }
#endif    

    if (bind(fd, i->ai_addr, i->ai_addrlen) != -1){
      freeaddrinfo(res);

      if (listen(fd, 5) == -1){
        perror("listen");
        close(fd);
        exit(1);
      }
      
      return fd;
    }

    close(fd);
  }
  freeaddrinfo(res);
  fprintf(stderr, "No usable address for %s:%s\n", hostname, service);
  exit(1);
}
