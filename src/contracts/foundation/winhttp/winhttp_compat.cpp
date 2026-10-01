// WinHTTP-backed implementation of the small libcurl surface declared in
// winhttp/curl/curl.h. Used only when no real libcurl is available on
// Windows, so the Telegram gateway keeps working with zero external deps.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winhttp.h>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <string>
#include "curl/curl.h"

#pragma comment(lib, "winhttp.lib")

namespace {

struct EasyHandle {
    std::string url;
    std::string post_fields;
    xaus_write_callback write_fn = nullptr;
    DWORD connect_timeout_ms = 5000;
    DWORD send_timeout_ms = 10000;
    long response_code = 0;
};

bool parse_url(const std::string& url, std::wstring& host, INTERNET_PORT& port,
               bool& https, std::wstring& path) {
    std::string scheme, rest;
    const auto sep = url.find("://");
    if (sep == std::string::npos) return false;
    scheme = url.substr(0, sep);
    rest = url.substr(sep + 3);
    https = (scheme == "https");
    port = https ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    const auto slash = rest.find('/');
    std::string authority = (slash == std::string::npos) ? rest : rest.substr(0, slash);
    path = L"/";
    if (slash != std::string::npos) {
        std::string p = rest.substr(slash);
        path.assign(p.begin(), p.end());
    }
    const auto colon = authority.find(':');
    if (colon != std::string::npos) {
        port = static_cast<INTERNET_PORT>(std::atoi(authority.c_str() + colon + 1));
        authority = authority.substr(0, colon);
    }
    host.assign(authority.begin(), authority.end());
    return !host.empty();
}

std::string widen_to_utf8(const std::wstring& w) {
    if (w.empty()) return {};
    const int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string out(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), out.data(), n, nullptr, nullptr);
    return out;
}

} // namespace

extern "C" {

CURLcode curl_global_init(long) { return CURLE_OK; }
void curl_global_cleanup() {}

CURL* curl_easy_init() {
    auto* h = new (std::nothrow) EasyHandle();
    return reinterpret_cast<CURL*>(h);
}

void curl_easy_cleanup(CURL* handle) {
    delete reinterpret_cast<EasyHandle*>(handle);
}

CURLcode curl_easy_setopt(CURL* handle, CURLoption option, ...) {
    auto* h = reinterpret_cast<EasyHandle*>(handle);
    if (h == nullptr) return CURLE_FAILED_INIT;
    va_list args;
    va_start(args, option);
    CURLcode rc = CURLE_OK;
    switch (option) {
        case CURLOPT_URL: {
            const char* s = va_arg(args, const char*);
            h->url = s ? s : "";
            break;
        }
        case CURLOPT_POSTFIELDS: {
            const char* s = va_arg(args, const char*);
            h->post_fields = s ? s : "";
            break;
        }
        case CURLOPT_WRITEFUNCTION:
            h->write_fn = va_arg(args, xaus_write_callback);
            break;
        case CURLOPT_CONNECTTIMEOUT_MS:
            h->connect_timeout_ms = static_cast<DWORD>(va_arg(args, long));
            break;
        case CURLOPT_TIMEOUT_MS:
            h->send_timeout_ms = static_cast<DWORD>(va_arg(args, long));
            break;
        default: {
            // Accept and ignore the remaining known flags.
            (void)va_arg(args, void*);
            break;
        }
    }
    va_end(args);
    return rc;
}

char* curl_easy_escape(CURL*, const char* string, int length) {
    if (string == nullptr) return nullptr;
    const std::size_t len = (length > 0) ? static_cast<std::size_t>(length) : std::strlen(string);
    static const char* hex = "0123456789ABCDEF";
    std::string out;
    out.reserve(len * 3);
    for (std::size_t i = 0; i < len; ++i) {
        const unsigned char c = static_cast<unsigned char>(string[i]);
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
            c == '-' || c == '_' || c == '.' || c == '~') {
            out.push_back(static_cast<char>(c));
        } else {
            out.push_back('%');
            out.push_back(hex[c >> 4]);
            out.push_back(hex[c & 0xF]);
        }
    }
    char* copy = static_cast<char*>(std::malloc(out.size() + 1));
    if (copy) std::memcpy(copy, out.c_str(), out.size() + 1);
    return copy;
}

void curl_free(void* ptr) { std::free(ptr); }

CURLcode curl_easy_perform(CURL* handle) {
    auto* h = reinterpret_cast<EasyHandle*>(handle);
    if (h == nullptr) return CURLE_FAILED_INIT;
    std::wstring host, path;
    INTERNET_PORT port = 0;
    bool https = false;
    if (!parse_url(h->url, host, port, https, path)) return CURLE_URL_MALFORMAT;

    HINTERNET session = WinHttpOpen(L"XAUUSD-Telegram/1.0",
                                    WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) return CURLE_FAILED_INIT;
    WinHttpSetTimeouts(session, (int)h->connect_timeout_ms, (int)h->connect_timeout_ms,
                       (int)h->send_timeout_ms, (int)h->send_timeout_ms);

    HINTERNET connection = WinHttpConnect(session, host.c_str(), port, 0);
    if (!connection) { WinHttpCloseHandle(session); return CURLE_COULDNT_CONNECT; }

    DWORD flags = https ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET request = WinHttpOpenRequest(connection, L"POST", path.c_str(), nullptr,
                                           WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
    if (!request) {
        WinHttpCloseHandle(connection); WinHttpCloseHandle(session);
        return CURLE_COULDNT_CONNECT;
    }
    if (https) {
        DWORD sec = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2 | WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_3;
        WinHttpSetOption(request, WINHTTP_OPTION_SECURE_PROTOCOLS, &sec, sizeof(sec));
    }
    const wchar_t* headers = L"Content-Type: application/x-www-form-urlencoded\r\n";
    BOOL sent = WinHttpSendRequest(request, headers, (DWORD)-1,
                                   (LPVOID)h->post_fields.data(),
                                   (DWORD)h->post_fields.size(),
                                   (DWORD)h->post_fields.size(), 0);
    CURLcode rc = CURLE_OK;
    if (sent && WinHttpReceiveResponse(request, nullptr)) {
        DWORD status = 0, size = 0;
        WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                            WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX);
        h->response_code = static_cast<long>(status);
        for (;;) {
            DWORD avail = 0;
            if (!WinHttpQueryDataAvailable(request, &avail) || avail == 0) break;
            std::string chunk(avail, '\0');
            DWORD read = 0;
            if (!WinHttpReadData(request, chunk.data(), avail, &read) || read == 0) break;
            if (h->write_fn) {
                const unsigned long consumed =
                    h->write_fn(chunk.data(), 1, read, nullptr);
                if (consumed != read) { rc = CURLE_UNKNOWN_OPTION; break; }
            }
        }
    } else {
        const DWORD err = GetLastError();
        rc = (err == ERROR_WINHTTP_TIMEOUT) ? CURLE_OPERATION_TIMEDOUT : CURLE_COULDNT_CONNECT;
    }
    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connection);
    WinHttpCloseHandle(session);
    return rc;
}

CURLcode curl_easy_getinfo(CURL* handle, int info, ...) {
    auto* h = reinterpret_cast<EasyHandle*>(handle);
    if (h == nullptr) return CURLE_FAILED_INIT;
    va_list args;
    va_start(args, info);
    if (info == CURLINFO_RESPONSE_CODE) {
        long* out = va_arg(args, long*);
        if (out) *out = h->response_code;
    }
    va_end(args);
    return CURLE_OK;
}

const char* curl_easy_strerror(CURLcode code) {
    switch (code) {
        case CURLE_OK: return "No error";
        case CURLE_URL_MALFORMAT: return "Invalid URL format";
        case CURLE_COULDNT_CONNECT: return "Could not connect to server";
        case CURLE_OPERATION_TIMEDOUT: return "Operation timed out";
        case CURLE_FAILED_INIT: return "Failed initializing WinHTTP session";
        default: return "Unknown error";
    }
}

} // extern "C"
