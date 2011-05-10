#ifndef H__tftt__network__
#define H__tftt__network__

int tcp_connect(char* hostname,
                char* service);
int tcp_bind(char* hostname,
             char* service);

#endif
