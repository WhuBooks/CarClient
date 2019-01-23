//
// Created by books on 19-1-22.
//

#ifndef CARCLIENT_MQTTCLIENT_H
#define CARCLIENT_MQTTCLIENT_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <functional>
#include <thread>

#include <mosquittopp.h>

namespace CarClient{

    class MqttClient:public mosqpp::mosquittopp
    {
        typedef std::function<void(const std::string &)> RecvFunc;
    public:
        explicit MqttClient(const std::string &host="localhost", const int &port=1883);

        ~MqttClient();

        void initialSend(const std::string &topic,int qos=0);

        void initialRecv(const std::string &topic,RecvFunc func,int qos=0);

        bool sendMsg(const std::string &msg);

        static void init(){
            std::cout<<"Mqtt Lib Init - "<<mosqpp::strerror(mosqpp::lib_init())<<std::endl;
        };
        static void clean(){
            std::cout<<"Mqtt Lib Clean - "<<mosqpp::strerror(mosqpp::lib_cleanup())<<std::endl;
        };

    private:

        std::string m_host;
        std::string m_id;
        int m_port;
        int m_keepalive;

        std::string m_send_topic;
        int m_send_qos;
        std::string m_recv_topic;
        int m_recv_qos;

        RecvFunc m_recv_func;

        void on_connect(int rc) override;

        void on_disconnect(int rc) override;

        void on_publish(int mid) override;

        void on_message(const struct mosquitto_message *message) override;

        void on_subscribe(int mid, int qos_count, const int *granted_qos) override;

        void on_unsubscribe(int mid) override;

        void on_log(int level, const char *str) override;

        void on_error() override;
    };
}



#endif //CARCLIENT_MQTTCLIENT_H
