#ifndef _CHAT_H_
#define _CHAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#define CHAT_URL "https://api.openai.com/v1/chat/completions"
#define CONTENT_TYPE "Content-Type:application/json;charset=UTF-8"
#define AUTHORIZATION "Authorization: Bearer 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"
#define POST_DATA_TEMPLATE "{\"model\":\"gpt-3.5-turbo\",\"messages\": [{\"role\": \"user\", \"content\": "
#define BUFF_SIZE 1024
// 存储返回信息
struct MemoryStruct
{
    char *memory;
    size_t size;
};

// 存储发送信息
typedef struct curl_info
{
    int info;
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers;
    struct MemoryStruct chunk;
    char *request;
    char *post_data;
} ci, *ci_ptr;

ci_ptr Init_Chat();
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
void Communication(ci_ptr chat_ptr);
void clean_up(ci_ptr chat_ptr);
#endif