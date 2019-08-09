#ifndef _CURL_TOOLS
#define _CURL_TOOLS

#include "includes.h"
#include "file-io.h"
#include "images.h"
#include "draw.h"

struct stringcurl {
  char *ptr;
  size_t len;
};

extern std::string progress_string;

extern int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);

extern void init_string(struct stringcurl *s);

extern size_t writefunc(void *ptr, size_t size, size_t nmemb, struct stringcurl *s);

extern size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

extern size_t write_data_to_disk(void *ptr, size_t size, size_t nmemb, void *stream);

extern std::string curl_get_string(std::string url);

extern int curl_post_issue(std::string url, std::string title, std::string body);
extern int curl_post_star(std::string url, bool delete_star);
extern bool curl_check_star(std::string url);
//straight from the samples. Downloads a file from  url to file--->see parameters.
extern void curl_download_file(std::string url , std::string file);
extern void curl_download_file_no_alert(std::string url , std::string file);

extern void netInit();

extern void netTerm();

extern void httpInit();

extern void httpTerm();

extern void check_core_rate_limit();


#endif
