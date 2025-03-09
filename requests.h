#ifndef _REQUESTS_
#define _REQUESTS_

#include "json.hpp"

using namespace std;

// Computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, char *url,
							vector<string> cookies, const string& jwt);

// Computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char *host, char *url, char* content_type, nlohmann::json *json, const string& jwt);

char *compute_delete_request(char *host, char *url, const string& jwt);

#endif