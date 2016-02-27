#include "sdncurl.h"
#include <stdlib.h>
#include <string>
#include <memory.h>
#include <QVector>


#include <qdebug.h>

char SDNCurl::wr_buf[MAX_BUFSIZE] = {0};
int SDNCurl::wr_index = 0;

SDNCurl::SDNCurl()
{

}

/*
 * Write data callback function (called within the context of
 * curl_easy_perform.
 */
 size_t SDNCurl::write_data( void *buffer, size_t size, size_t nmemb, void *userp )
 {
    int segsize = size * nmemb;

    /* Check to see if this data exceeds the size of our buffer. If so,
    * set the user-defined context value and return 0 to indicate a
    * problem to curl.
    */
    if ( SDNCurl::wr_index + segsize > MAX_BUFSIZE ) {
        *(int *)userp = 1;
        return 0;
    }

    /* Copy the data from the curl buffer into our buffer */
    memcpy( (void *)&SDNCurl::wr_buf[SDNCurl::wr_index], buffer, (size_t)segsize );

    /* Update the write index */
    SDNCurl::wr_index += segsize;

    /* Null terminate the buffer */
    SDNCurl::wr_buf[SDNCurl::wr_index] = 0;

    /* Return the number of bytes received, indicating to curl that all is okay */
    return segsize;
 }

bool SDNCurl::getFlows(char *filename)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)  // 返回结果用文件存储
         return false;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init();    // 初始化
    if (curl)
    {
         //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
         //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头

         curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/wm/staticflowpusher/list/00:00:00:00:00:00:00:01/json");
         curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

         res = curl_easy_perform(curl);   // 执行

         //curl_slist_free_all(headers);

         curl_easy_cleanup(curl);
    }
    fclose(fp);
    qDebug()<<"get success!";
    return true;
}

bool SDNCurl::addFlows()
{
    char cJsonData[1024];
    memset(cJsonData, 0, sizeof(cJsonData));
    QVector<std::string> t_sjsonVec;
    std::string strJson;

    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
    strJson += "\"name\" : \"flow-mod-1\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"1\",";
    strJson += "\"active\" : \"true\",";
    strJson += "\"actions\" : \"output=flood\"";
    strJson += "}";
    t_sjsonVec.append(strJson);

    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
    strJson += "\"name\" : \"flow-mod-2\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"2\",";
    strJson += "\"active\" : \"true\",";
    strJson += "\"actions\" : \"output=flood\"";
    strJson += "}";
    t_sjsonVec.append(strJson);

    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
    strJson += "\"name\" : \"flow-mod-3\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"3\",";
    strJson += "\"active\" : \"true\",";
    strJson += "\"actions\" : \"output=flood\"";
    strJson += "}";
    t_sjsonVec.append(strJson);

    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
    strJson += "\"name\" : \"flow-mod-8\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"3\",";
    strJson += "\"active\" : \"false\",";
    strJson += "\"actions\" : \"output=2\"";
    strJson += "}";
    t_sjsonVec.append(strJson);

    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
    strJson += "\"name\" : \"flow-mod-9\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"2\",";
    strJson += "\"active\" : \"false\",";
    strJson += "\"actions\" : \"output=3\"";
    strJson += "}";
    t_sjsonVec.append(strJson);


    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:02\",";
    strJson += "\"name\" : \"flow-mod-4\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"1\",";
    strJson += "\"active\" : \"true\",";
    strJson += "\"actions\" : \"output=flood\"";
    strJson += "}";
    t_sjsonVec.append(strJson);

    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:02\",";
    strJson += "\"name\" : \"flow-mod-5\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"2\",";
    strJson += "\"active\" : \"true\",";
    strJson += "\"actions\" : \"output=1\"";
    strJson += "}";
    t_sjsonVec.append(strJson);


    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:03\",";
    strJson += "\"name\" : \"flow-mod-6\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"1\",";
    strJson += "\"active\" : \"true\",";
    strJson += "\"actions\" : \"output=flood\"";
    strJson += "}";
    t_sjsonVec.append(strJson);

    strJson = "{";
    strJson += "\"switch\" : \"00:00:00:00:00:00:00:03\",";
    strJson += "\"name\" : \"flow-mod-7\",";
    strJson += "\"cookie\" : \"0\",";
    strJson += "\"priority\" : \"32768\",";
    strJson += "\"in_port\" : \"2\",";
    strJson += "\"active\" : \"true\",";
    strJson += "\"actions\" : \"output=1\"";
    strJson += "}";
    t_sjsonVec.append(strJson);




//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
//    strJson += "\"name\" : \"flow-mod-1\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"1\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=flood\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);
//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
//    strJson += "\"name\" : \"flow-mod-2\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"2\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=1\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);

//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:01\",";
//    strJson += "\"name\" : \"flow-mod-3\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"3\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=1\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);

//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:02\",";
//    strJson += "\"name\" : \"flow-mod-4\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"1\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=flood\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);

//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:02\",";
//    strJson += "\"name\" : \"flow-mod-5\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"2\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=1\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);

//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:02\",";
//    strJson += "\"name\" : \"flow-mod-6\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"3\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=1\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);

//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:03\",";
//    strJson += "\"name\" : \"flow-mod-7\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"1\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=flood\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);

//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:03\",";
//    strJson += "\"name\" : \"flow-mod-8\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"2\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=1\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);

//    strJson = "{";
//    strJson += "\"switch\" : \"00:00:00:00:00:00:00:03\",";
//    strJson += "\"name\" : \"flow-mod-9\",";
//    strJson += "\"cookie\" : \"0\",";
//    strJson += "\"priority\" : \"32768\",";
//    strJson += "\"in_port\" : \"3\",";
//    strJson += "\"active\" : \"true\",";
//    strJson += "\"actions\" : \"output=1\"";
//    strJson += "}";
//    t_sjsonVec.append(strJson);


    for(int i=0; i<t_sjsonVec.size();++i)
    {
        memset(cJsonData, 0, sizeof(cJsonData));

        strcpy(cJsonData, t_sjsonVec.at(i).c_str());

        CURL *pCurl = NULL;
        CURLcode res;
        // In windows, this will init the winsock stuff
        curl_global_init(CURL_GLOBAL_ALL);

        // get a curl handle
        pCurl = curl_easy_init();
        if (NULL != pCurl)
        {
            // 设置超时时间为1秒
            //curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 1);
            curl_easy_setopt(pCurl, CURLOPT_URL, "http://192.168.1.145:8080/wm/staticflowpusher/json");
            // 设置http发送的内容类型为JSON
            curl_slist *headers = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
            curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);
            // 设置要POST的JSON数据
            curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, cJsonData);
            // Perform the request, res will get the return code
            res = curl_easy_perform(pCurl);
            // Check for errors
            if (res != CURLE_OK)
            {
                printf("curl_easy_perform() failed:%s\n", curl_easy_strerror(res));
            }
            // always cleanup
            curl_easy_cleanup(pCurl);
        }
        curl_global_cleanup();
        qDebug()<<"add success:"<<i;
    }



    return true;
}

bool SDNCurl::deleteFlow()
{
    char cJsonData[1024];
    memset(cJsonData, 0, sizeof(cJsonData));
    std::string strJson = "{";
    strJson += "\"name\" : \"flow-mod-1\"";
    strJson += "}";
    strcpy(cJsonData, strJson.c_str());

    CURL *pCurl;
    CURLcode res;

    pCurl = curl_easy_init();
    if(pCurl) {
        curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(pCurl,CURLOPT_CUSTOMREQUEST,"DELETE");
        //curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION,&http_callback);
        curl_easy_setopt(pCurl, CURLOPT_URL,"http://127.0.0.1:8080/wm/staticflowpusher/json");
        // 设置http发送的内容类型为JSON
        curl_slist *headers = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
        curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(pCurl,CURLOPT_POSTFIELDS,cJsonData);
        res = curl_easy_perform(pCurl);
        // Check for errors
        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed:%s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(pCurl);
    }

    return true;
}

bool SDNCurl::getTopo(QString &_TopoStr)
{
    CURL *curl;
    CURLcode res;
    int  wr_error;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();    // 初始化
    if (curl)
    {
        curl_slist *headers = curl_slist_append(NULL, "Accept: Agent-007");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头

        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080//wm/topology/links/json");
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&wr_error);
        curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, SDNCurl::write_data );


        res = curl_easy_perform(curl);   // 执行
        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed:%s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        _TopoStr = QString(QLatin1String(SDNCurl::wr_buf));
        memset(SDNCurl::wr_buf,0,MAX_BUFSIZE);
        SDNCurl::wr_index=0;
    }
    curl_global_cleanup();

    qDebug()<<_TopoStr;
    qDebug()<<"get success!";
    return true;
}
