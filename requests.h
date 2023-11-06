#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *recv_post_req(int socket, char host[16], char *command, char *user[1], char *token);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *recv_get_req(int socket, char host[16], char *command, char *token, char *cookies[1], char *get_delete);

char* compute_get_request(char* host, char* url, char* query_params,
                            char** cookies, int cookies_count, char *token, char *command);


char* compute_post_request(char* host, char* url, char* content_type, char** body_data,
                            int body_data_fields_count, char** cookies, int cookies_count, char *token);

#endif
