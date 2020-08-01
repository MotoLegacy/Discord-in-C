//
// Net.c - Handles POST/Get Requests, among other Networking things.
//
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "defs.h"

// Some misc. depends for CURL.. why aren't these in curl.h?
struct stringcurl {
    char *ptr;
    size_t len;
}; 
void init_string(struct stringcurl *s) {
    s->len = 0;
    s->ptr = (char*)malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct stringcurl *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = (char*)realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}
// end CURL garbage

/*
=====================
Net_PostRequest
=====================
Does POST request handling, URL and
PostData is required, insert an empty char array
if AuthToken is not available/is not required.
*/
HTTPResponse_t Net_PostRequest(char* URL, char* PostData, char* AuthToken)
{
    // Initialize CURL
    CURL *Curl;
    CURLcode CurlResponse;

    curl_global_init(CURL_GLOBAL_ALL);

    Curl = curl_easy_init();

    // HTTP Response
    HTTPResponse_t Response;

    // Do.. CURL stuff..
    if (Curl) {
        struct stringcurl Body;
		init_string(&Body);
		struct stringcurl Header;
		init_string(&Header);

        curl_easy_setopt(Curl, CURLOPT_POST, 1L);
		curl_easy_setopt(Curl, CURLOPT_URL, URL);

        // Use TLSv1.2 because we're cool and know what's up.
        curl_easy_setopt(Curl, CURLOPT_USE_SSL, CURL_SSLVERSION_TLSv1_2);

		curl_easy_setopt(Curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &Body);
		curl_easy_setopt(Curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(Curl, CURLOPT_HEADERDATA, &Header);
		curl_easy_setopt(Curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(Curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(Curl, CURLOPT_NOPROGRESS, 1L);
		struct curl_slist *HeaderChunk = NULL;
        HeaderChunk = curl_slist_append(HeaderChunk, "Accept: */*");
        HeaderChunk = curl_slist_append(HeaderChunk, "Content-Type: application/json");
		HeaderChunk = curl_slist_append(HeaderChunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
    
        // Authorization
        char TokenKey[256];
        va_append(TokenKey, "Authorization: %s", AuthToken);
        HeaderChunk = curl_slist_append(HeaderChunk, TokenKey);

        HeaderChunk = curl_slist_append(HeaderChunk, "Host: discordapp.com");

        // Content Length
        char ContentLength[128];
        va_append(ContentLength, "Content-Length: %d", strlen(PostData));
		HeaderChunk = curl_slist_append(HeaderChunk, ContentLength);

        CurlResponse = curl_easy_setopt(Curl, CURLOPT_HTTPHEADER, HeaderChunk);
        curl_easy_setopt(Curl, CURLOPT_POSTFIELDS, PostData);
		curl_easy_setopt(Curl, CURLOPT_POSTFIELDSIZE, (long)strlen(PostData));

        CurlResponse = curl_easy_perform(Curl);

        Response.Header = Header.ptr;
        Response.Body = Body.ptr;
        
        // Make sure everything was a-ok
        if (CurlResponse == CURLE_OK) {
            curl_easy_getinfo (Curl, CURLINFO_RESPONSE_CODE, &Response.Code);
        } else {
            printf("[CURL ERR]: %s\n", curl_easy_strerror(CurlResponse));
            Response.Code = 99;
        }
    }

    curl_easy_cleanup(Curl);

    return Response;
}

/*
=====================
Net_GetRequest
=====================
Does GET request handling, URL is required, insert an 
empty char array if AuthToken is not available/is 
not required.
*/
HTTPResponse_t Net_GetRequest(char* URL, char* AuthToken)
{
    // Initialize CURL
    CURL *Curl;
    CURLcode CurlResponse;

    curl_global_init(CURL_GLOBAL_ALL);

    Curl = curl_easy_init();

    // HTTP Response
    HTTPResponse_t Response;

    // Do.. CURL stuff..
    if (Curl) {
        struct stringcurl Body;
		init_string(&Body);
		struct stringcurl Header;
		init_string(&Header);

		curl_easy_setopt(Curl, CURLOPT_URL, URL);

        // Use TLSv1.2 because we're cool and know what's up.
        //curl_easy_setopt(Curl, CURLOPT_USE_SSL, CURL_SSLVERSION_TLSv1_2);

		curl_easy_setopt(Curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		curl_easy_setopt(Curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(Curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(Curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &Body);
		curl_easy_setopt(Curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(Curl, CURLOPT_HEADERDATA, &Header);
		struct curl_slist *HeaderChunk = NULL;
		HeaderChunk = curl_slist_append(HeaderChunk, "Accept: */*");
		HeaderChunk = curl_slist_append(HeaderChunk, "Content-Type: application/json");
		HeaderChunk = curl_slist_append(HeaderChunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");

        // Authorization
        char TokenKey[256];
        va_append(TokenKey, "Authorization: %s", AuthToken);
        HeaderChunk = curl_slist_append(HeaderChunk, TokenKey);

		HeaderChunk = curl_slist_append(HeaderChunk, "Host: discordapp.com");
		HeaderChunk = curl_slist_append(HeaderChunk, "Content-Length: 0");

		CurlResponse = curl_easy_setopt(Curl, CURLOPT_HTTPHEADER, HeaderChunk);
	
		CurlResponse = curl_easy_perform(Curl);
		curl_easy_getinfo (Curl, CURLINFO_RESPONSE_CODE, &Response.Code);

        Response.Header = Header.ptr;
        Response.Body = Body.ptr;

        // Make sure everything was a-ok
        if (CurlResponse == CURLE_OK) {
            curl_easy_getinfo (Curl, CURLINFO_RESPONSE_CODE, &Response.Code);
        } else {
            printf("[CURL ERR]: %s\n", curl_easy_strerror(CurlResponse));
            Response.Code = 99;
        }
    }

    curl_easy_cleanup(Curl);

    return Response;
}