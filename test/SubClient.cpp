//
// Created by books on 2019/1/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>
#include <string>

#include <iostream>
#include <nlohmann/json.hpp>

#define HOST "localhost"
#define PORT  1883
#define KEEP_ALIVE 60

bool session = true;

struct person {
    std::string name;
    std::string address;
    int age;

    bool operator==(const person &other)
    {
        return name==other.name && address==other.address && age==other.age;
    }
};
void to_json(nlohmann::json& j, const person& p)
{
    j = nlohmann::json{{"name",    p.name},
                       {"address", p.address},
                       {"age",     p.age}};
}

void from_json(const nlohmann::json& j, person& p)
{
    p.name = j.at("name").get<std::string>();
    p.address = j.at("address").get<std::string>();
    p.age = j.at("age").get<int>();
}

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (message->payloadlen)
    {
        std::string msg((char*)message->payload);

        nlohmann::json j=nlohmann::json::parse(msg);
        std::cout<<j.dump(4)<<std::endl;

        person p=j;
        printf("Person %s\t%s\t%d",p.name.c_str(),p.address.c_str(),p.age);

//        printf("%s %s", message->topic, message->payload);
    }
    else
    {
        printf("%s (null)\n", message->topic);
    }
    fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    int i;
    if (!result)
    {
        printf("Connect Callback\n");

        /* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq, NULL, "Gai :", 2);
    }
    else
    {
        fprintf(stderr, "Connect failed\n");
    }
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    int i;
    printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
    for (i = 1; i < qos_count; i++)
    {
        printf(", %d", granted_qos[i]);
    }
    printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
    /* Pring all log messages regardless of level. */
    printf("%s\n", str);
}

int main()
{
    struct mosquitto *mosq = NULL;
    //libmosquitto 库初始化
    mosquitto_lib_init();
    printf("Mosquitto Init Done!\n");
    //创建mosquitto客户端
    mosq = mosquitto_new(NULL, session, NULL);
    if (!mosq)
    {
        printf("create client failed..\n");
        mosquitto_lib_cleanup();
        return 1;
    }
    //设置回调函数，需要时可使用
//    mosquitto_log_callback_set(mosq, my_log_callback);
    mosquitto_connect_callback_set(mosq, my_connect_callback);
    mosquitto_message_callback_set(mosq, my_message_callback);
//    mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
    //客户端连接服务器
    if (mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE))
    {
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }
    //循环处理网络消息
    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
