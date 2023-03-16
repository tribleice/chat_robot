#include "chat.h"

ci_ptr Init_Chat()
{
    ci_ptr chat_ptr = (ci_ptr)malloc(sizeof(ci));

    chat_ptr->info = 0;

    chat_ptr->headers = NULL;

    chat_ptr->chunk.memory = malloc(1);
    /* will be grown as needed by the realloc above */
    chat_ptr->chunk.size = 0;
    /*no data at this point*/

    chat_ptr->request = calloc(1, BUFF_SIZE);
    chat_ptr->post_data = calloc(1, BUFF_SIZE * 4);

    chat_ptr->curl = curl_easy_init();
    if (chat_ptr->curl)
    {
        // 设置请求信息
        chat_ptr->headers = curl_slist_append(chat_ptr->headers, CONTENT_TYPE);
        chat_ptr->headers = curl_slist_append(chat_ptr->headers, AUTHORIZATION);
        curl_easy_setopt(chat_ptr->curl, CURLOPT_HTTPHEADER, chat_ptr->headers); // 设置HTTP头
        curl_easy_setopt(chat_ptr->curl, CURLOPT_URL, CHAT_URL);                 // 设置post请求的url地址
        // curl_easy_setopt(chat_ptr->curl, CURLOPT_TIMEOUT, 1);//设置发送超时时间
        curl_easy_setopt(chat_ptr->curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(chat_ptr->curl, CURLOPT_WRITEDATA, (void *)&chat_ptr->chunk);
        curl_easy_setopt(chat_ptr->curl, CURLOPT_POST, 1);
        curl_easy_setopt(chat_ptr->curl, CURLOPT_VERBOSE, 1); // 打印调试信息
    }

    return chat_ptr;
}

void clean_up(ci_ptr chat_ptr)
{
    free(chat_ptr->chunk.memory);
    curl_slist_free_all(chat_ptr->headers);
    curl_easy_cleanup(chat_ptr->curl);

    free(chat_ptr->request);
    if (chat_ptr->request != NULL)
    {
        chat_ptr->request != NULL;
    }

    free(chat_ptr->post_data);
    if (chat_ptr->post_data != NULL)
    {
        chat_ptr->post_data != NULL;
    }

    free(chat_ptr);
    if (chat_ptr != NULL)
    {
        chat_ptr = NULL;
    }

    curl_global_cleanup();
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char *result = calloc(1, BUFF_SIZE * 4);
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL)
    {
        /*	out	of	memory!	*/
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    // int i = 0;
    // printf("\ncontents:");
    // for (i = 0; i < realsize; i++)
    // {
    //     printf("%c", mem->memory[i]);
    // }
    // printf("\n");

    // printf("contents === %s\n", contents);
    printf("mem->memory === %s\n", mem->memory);

    strcpy(result, mem->memory);
    result = strstr(mem->memory, "content") + 10; // 定位到content
    result = strtok(result, ",");                 // 去除content的value后面的内容
    result[strlen(result) - 2] = '\0';            // 去除末尾的"}
    printf("result === %s\n", result + 4);        // 去除开头的\n\n
    return realsize;
}

void Communication(ci_ptr chat_ptr)
{
    char *send_msg;

    while (1)
    {
        printf("please input message:\n");
        scanf("%s", send_msg);
        if ("exit" == send_msg)
        {
            return;
        }

        // 拼接post_data
        sprintf(chat_ptr->request, "%s%s%s", "\"", send_msg, "\"");
        // printf("request === %s\n", chat_ptr->request);
        sprintf(chat_ptr->post_data, "%s%s%s", POST_DATA_TEMPLATE, chat_ptr->request, "}]}");
        printf("post_data === %s\n", chat_ptr->post_data);
        printf("file of %s:	line of %d\n", __FILE__, __LINE__);
        curl_easy_setopt(chat_ptr->curl, CURLOPT_POSTFIELDS, chat_ptr->post_data);
        printf("file of %s:	line of %d\n", __FILE__, __LINE__);
        chat_ptr->res = curl_easy_perform(chat_ptr->curl);
        curl_easy_getinfo(chat_ptr->curl, CURLINFO_RESPONSE_CODE, &chat_ptr->info);
        if (chat_ptr->res != CURLE_OK || chat_ptr->info != 200)
        {
            printf("!crul failed to perform url='%s' chat_ptr->res=[%d] rcode=%d\n", CHAT_URL, chat_ptr->res, chat_ptr->info);
            exit(1);
        }
        else
        {
            printf("%lu	bytes retrieved\n", (long)chat_ptr->chunk.size);
        }
    }

    return;
}
