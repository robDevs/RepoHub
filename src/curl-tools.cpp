#include "curl-tools.h"
#include "controls.h"

std::string progress_string;

//progress func for file download.
int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) {
    // ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on
    if (TotalToDownload <= 0.0) {
        return 0;
    }

    update_keys();

    if(circle_released) {
        return 1;
    }

    std::string NowDownloaded_string = std::to_string(byte_to_mb((int)round(NowDownloaded)));
    NowDownloaded_string.resize(NowDownloaded_string.find(".") + 3);

    std::string TotalToDownload_string = std::to_string(byte_to_mb((int)round(TotalToDownload)));
    TotalToDownload_string.resize(TotalToDownload_string.find(".") + 3);

    uint64_t space_avail = get_space_avail();

    std::string free_space = "Free Space: ";
    if(space_avail > 1024*1024*1024) free_space += std::to_string(byte_to_gb(space_avail));
    else free_space += std::to_string(byte_to_mb(space_avail));
    free_space.resize(free_space.find(".") + 3);
    if(space_avail > 1024*1024*1024) free_space += " GB";
    else free_space += " MB";


    std::string label = "Downloading file...";
    std::string label1 = NowDownloaded_string;
    label1 += " MB / ";
    label1 += TotalToDownload_string;
    label1 += " MB";

		// how wide you want the progress meter to be
    int total=320;
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    // part of the progressmeter that's already "full"
    int progress = round(fractiondownloaded * total);

		vita2d_start_drawing();

		vita2d_draw_rectangle(960 / 2 - 200, 544 / 2 - 100, 400, 200, RGBA8(36,41,46,255));
		vita2d_draw_rectangle(960 / 2 - 160, 544 / 2 - 2, progress, 4, RGBA8(0,255,0,255));

        vita2d_font_draw_text(font20, (960/2)-(vita2d_font_text_width(font20, 20.0f, label.c_str()) / 2), 544 / 2 - 40, RGBA8(255,255,255,255), 20.0f, label.c_str());
        vita2d_font_draw_text(font20, (960/2)-(vita2d_font_text_width(font20, 20.0f, label1.c_str()) / 2), 544 / 2 - 20, RGBA8(255,255,255,255), 20.0f, label1.c_str());

        vita2d_font_draw_text(font20, (960/2)-(vita2d_font_text_width(font20, 20.0f, free_space.c_str()) / 2), 544 / 2 + 20, RGBA8(255,255,255,255), 20.0f, free_space.c_str());

		vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();

    // if you don't return 0, the transfer will be aborted - see the documentation
    return 0;
}

//progress func for curl_get_string
//currently just draws a header as getting a string is fast.
int progress_func_string(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) {
    // ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on
    if (TotalToDownload <= 0.0) {
        return 0;
    }

		// how wide you want the progress meter to be
        // part of the progressmeter that's already "full"

		vita2d_start_drawing();

        draw_header("Fetching Data...");

		vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();

    // if you don't return 0, the transfer will be aborted - see the documentation
    return 0;
}

void init_string(struct stringcurl *s) {
  s->len = 0;
  s->ptr = (char*)malloc(s->len+1);
  if (s->ptr == NULL) {
    return;
    //exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}


size_t writefunc(void *ptr, size_t size, size_t nmemb, struct stringcurl *s){
  size_t new_len = s->len + size*nmemb;
  s->ptr = (char*)realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    return 0;
    //exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

size_t write_data_to_disk(void *ptr, size_t size, size_t nmemb, void *stream) {
  size_t written = sceIoWrite(   *(int*) stream , ptr , size*nmemb);
  return written;
}

std::string curl_get_string(std::string url){

	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// Set useragant string
		curl_easy_setopt(curl, CURLOPT_USERAGENT, \
      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 \
      (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		// Set SSL VERSION to TLS 1.2
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
		// Set timeout for the connection to build
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		// Follow redirects (?)
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		// The function that will be used to write the data
		std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
		// Internal CURL progressmeter must be disabled if we provide our own callback
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		// Install the callback function
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func_string);
		// write function of response headers
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		// the response header data where to write to
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		// Request Header :
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
        if(have_token) {
            std::string final_token_header = "Authorization: token ";
            final_token_header += token;
            headerchunk = curl_slist_append(headerchunk, final_token_header.c_str());
        }
        headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 \
        (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) \
        Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);


		// Perform the request
		res = curl_easy_perform(curl);
		int httpresponsecode = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);

		if(res != CURLE_OK){
		}

		return response_string;
	}
	return "Error";
}

//straight from the samples. Downloads a file from  url to file--->see parameters.
//modified to use std::string
void curl_download_file(std::string url , std::string file){
    init_keys();
	int imageFD = sceIoOpen( file.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if(!imageFD) { return; }

	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// Set useragant string
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		// Set SSL VERSION to TLS 1.2
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
		// Set timeout for the connection to build
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		// Follow redirects (?)
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		// The function that will be used to write the data
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
		// The data filedescriptor which will be written to
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFD);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		// Install the callback function
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
		// write function of response headers
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		// the response header data where to write to
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		// Request Header :
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);


		// Perform the request
		res = curl_easy_perform(curl);
		int httpresponsecode = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);

		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            draw_alert_message(word_wrap(curl_easy_strerror(res), 30));
		}
        else {
            std::string result_string = "File saved to:";
            result_string += "\nux0:data/RepoHub/Downloads/";
            draw_alert_message(result_string);
        }
	}
    else {
        draw_alert_message("Failed to init Curl");
    }

	// close filedescriptor
	sceIoClose(imageFD);

	// cleanup
	curl_easy_cleanup(curl);

}

void curl_download_file_no_alert(std::string url , std::string file) {
    init_keys();
	int imageFD = sceIoOpen( file.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if(!imageFD) { return; }

	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// Set useragant string
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		// Set SSL VERSION to TLS 1.2
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
		// Set timeout for the connection to build
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		// Follow redirects (?)
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		// The function that will be used to write the data
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
		// The data filedescriptor which will be written to
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFD);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		// Install the callback function
		//curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
		// write function of response headers
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		// the response header data where to write to
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		// Request Header :
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);


		// Perform the request
		res = curl_easy_perform(curl);
		int httpresponsecode = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);

		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            draw_alert_message(word_wrap(curl_easy_strerror(res), 30));
		}
	}
    else {
        draw_alert_message("Failed to init Curl");
    }

	// close filedescriptor
	sceIoClose(imageFD);

	// cleanup
	curl_easy_cleanup(curl);
}

void netInit() {
	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);

	SceNetInitParam netInitParam;
	int size = 4*1024*1024;
	netInitParam.memory = malloc(size);
	netInitParam.size = size;
	netInitParam.flags = 0;
	sceNetInit(&netInitParam);

	sceNetCtlInit();
}

void netTerm() {
	sceNetCtlTerm();

	sceNetTerm();

	sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
}

void httpInit() {
	sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);

	sceHttpInit(4*1024*1024);
}

void httpTerm() {
	sceHttpTerm();

	sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTP);
}

void check_core_rate_limit() {
    std::string rate_limit_string = "Api Requests remaining: ";
    rate_limit_string += std::to_string(core_remain);
    if(core_remain == 10)
        draw_alert_message(rate_limit_string);
    if(core_remain < 5)
        draw_alert_message(rate_limit_string);
}

void check_search_rate_limit() {
    std::string rate_limit_string = "Search Requests remaining: ";
    rate_limit_string += std::to_string(core_remain);
    if(search_remain == 10)
        draw_alert_message(rate_limit_string);
    if(search_remain < 5)
        draw_alert_message(rate_limit_string);
}
