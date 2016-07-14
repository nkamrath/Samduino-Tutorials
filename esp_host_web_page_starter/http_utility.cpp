/*
 * http_utility.h
 *
 * Created: 3/22/2016 8:59:15 AM
 *  Author: Nate
 */ 

#include "http_utility.h"

uint8_t parse_http_request(char* request, uint16_t request_length, struct http_request_info* info)
{
	uint16_t path_start_index = request_length;
	//find the GET or POST on first line
	for(uint16_t i = 0; i < request_length; i++)
	{
		if(i < request_length - 3 && request[i] == 'G' && request[i+1] == 'E' && request[i+2] == 'T')
		{
			path_start_index = i + 4;
			info->request_type = HTTP_REQUEST_GET;
			break;
		}
		else if(i < request_length - 3 && request[i] == 'P' && request[i+1] == 'O' && request[i+2] == 'S' && request[i+3] == 'T')
		{
			path_start_index = i + 5;
			info->request_type = HTTP_REQUEST_POST;
			break;
		}
	}

	if(path_start_index == request_length)
	{
		//didn't find the GET or POST
		return 1;
	}

	//now extract the path requested in the http request
	uint8_t path_found = 0;
	for(uint16_t i = path_start_index; i < request_length; i++)
	{
		if(request[i] == ' ')
		{
			//space means end of path
			path_found = 1;
			//drop null terminator on end of path string so end is clear
			info->path[i - path_start_index] = '\0';
			break;
		}
		else
		{
			//we have another character for the requested path, add it to info struct
			info->path[i - path_start_index] = request[i];
		}
	}

	if(path_found == 0)
	{
		//found some info, but couldn't extract the full path
		return 2;
	}
	else
	{
		//successfully parsed the request
		return 0;
	}
}