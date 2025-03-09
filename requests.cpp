#include <cstdlib>     /* exit, atoi, malloc, free */
#include <string>     /* memcpy, memset */

#include "./helpers.h"
#include "./requests.h"

char *compute_get_request(char *host, char *url, vector<string> cookies, const string& jwt)
{

    char *line = (char *) calloc(LINELEN, sizeof(char));
    char *message = (char *) calloc(BUFLEN, sizeof(char));

    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (!jwt.empty()) {
        sprintf(line, "Authorization: Bearer %s", jwt.c_str());
        compute_message(message, line);
    }

    if (!cookies.empty()) {
        sprintf(line, "Cookie: %s", cookies[0].c_str());
        compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}


char *compute_post_request(char *host, char *url, char* content_type, nlohmann::json *json, const string& jwt)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    string buff = json->dump();

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %lu", buff.length());
    compute_message(message, line);

    if (!jwt.empty()) {
        sprintf(line, "Authorization: Bearer %s", jwt.c_str());
        compute_message(message, line);
    }

    compute_message(message, "");
    strcat(message, buff.c_str());

    free(line);
    return message;
}

// la fel ca la get_request doar ca fara cookie
char *compute_delete_request(char *host, char *url, const string& jwt)
{

    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);


    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (!jwt.empty()) {
        sprintf(line, "Authorization: Bearer %s", jwt.c_str());
        compute_message(message, line);
    }

    
    compute_message(message, "");
    return message;
}