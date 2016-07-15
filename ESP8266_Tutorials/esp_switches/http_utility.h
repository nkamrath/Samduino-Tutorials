/*
 * http_utility.h
 *
 * Created: 3/22/2016 8:58:59 AM
 *  Author: Nate
 */ 


#ifndef HTTP_UTILITY_H_
#define HTTP_UTILITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
	HTTP_REQUEST_UNDEFINED,
	HTTP_REQUEST_GET,
	HTTP_REQUEST_POST
} http_request_type;

struct http_request_info
{
	http_request_type request_type;
	char path[256];
};

//parse an http request and get the info from it
uint8_t parse_http_request(char* request, uint16_t request_length, struct http_request_info* info);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_UTILITY_H_ */
