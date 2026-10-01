#ifndef XAUS_WINHTTP_CURL_COMPAT_H
#define XAUS_WINHTTP_CURL_COMPAT_H
// Minimal libcurl-compatible surface implemented on top of the native
// Windows WinHTTP API. It exists so the project builds a self-contained
// Windows .exe without requiring external curl/vcpkg installations.
// Only the options actually used by TelegramGateway are honored.
#ifdef __cplusplus
extern "C" {
#endif

typedef void CURL;

typedef enum {
    CURLE_OK = 0,
    CURLE_FAILED_INIT = 2,
    CURLE_URL_MALFORMAT = 3,
    CURLE_COULDNT_CONNECT = 7,
    CURLE_OPERATION_TIMEDOUT = 28,
    CURLE_UNKNOWN_OPTION = 48,
    CURLE_TOO_MANY_REDIRECTS = 47,
    CURLE_UNSUPPORTED_PROTOCOL = 1
} CURLcode;

typedef enum {
    CURLOPT_POSTFIELDS = 10065L,
    CURLOPT_URL = 10002L,
    CURLOPT_WRITEFUNCTION = 20011L,
    CURLOPT_POST = 47L,
    CURLOPT_CONNECTTIMEOUT_MS = 156L,
    CURLOPT_TIMEOUT_MS = 155L,
    CURLOPT_NOSIGNAL = 99L,
    CURLOPT_SSL_VERIFYPEER = 64L,
    CURLOPT_SSL_VERIFYHOST = 81L
} CURLoption;

#define CURL_GLOBAL_DEFAULT 3L

typedef unsigned long (*xaus_write_callback)(char*, unsigned long, unsigned long, void*);

CURL* curl_easy_init(void);
void curl_easy_cleanup(CURL* handle);
CURLcode curl_easy_setopt(CURL* handle, CURLoption option, ...);
CURLcode curl_easy_perform(CURL* handle);
CURLcode curl_easy_getinfo(CURL* handle, int info, ...);
char* curl_easy_escape(CURL* handle, const char* string, int length);
void curl_free(void* ptr);
const char* curl_easy_strerror(CURLcode code);
CURLcode curl_global_init(long flags);
void curl_global_cleanup(void);

/* CURLINFO constants (compatible values with libcurl's public enum). */
#define CURLINFO_RESPONSE_CODE 0x200002L

#ifdef __cplusplus
}
#endif
#endif
