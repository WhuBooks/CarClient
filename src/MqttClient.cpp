//
// Created by books on 19-1-22.
//

#include "MqttClient.h"

namespace CarClient {

    MqttClient::MqttClient(const std::string &host, const int &port)
            : mosquittopp(), m_host(host), m_port(port)
    {
        m_keepalive = 60;

        m_send_topic = "";
        m_send_qos = 0;

        m_recv_topic = "";
        m_recv_qos = 0;
        m_recv_func = nullptr;

        connect_async(m_host.c_str(), m_port, m_keepalive);
        loop_start();
    }

    MqttClient::~MqttClient()
    {
        disconnect();
        loop_stop();
    }

    void MqttClient::initialSend(const std::string &topic, int qos)
    {
        m_send_topic = topic;
        m_send_qos = qos;
    }

    void MqttClient::initialRecv(const std::string &topic, RecvFunc func, int qos)
    {
        m_recv_topic = topic;
        m_recv_qos = qos;
        m_recv_func = func;

        subscribe(nullptr, m_recv_topic.c_str(), m_recv_qos);
    }

    bool MqttClient::sendMsg(const std::string &msg)
    {
        int ret = publish(NULL, m_send_topic.c_str(), msg.length(), msg.c_str(), m_send_qos, false);
        return (ret == MOSQ_ERR_SUCCESS);
    }

    void MqttClient::on_connect(int rc)
    {
        if (rc == 0)
        {
            std::cout << ">> Mqtt Client - Connected with server" << std::endl;
            if (!m_recv_topic.empty())
                subscribe(nullptr, m_recv_topic.c_str(), m_recv_qos);
        }
        else
        {
            std::cerr << ">> Mqtt Client - Impossible to connect with server (" << mosqpp::connack_string(rc) << ")"
                      << std::endl;
        }
    }

    void MqttClient::on_disconnect(int rc)
    {

        std::cout << ">> Mqtt Client - Disconnection (" << mosqpp::connack_string(rc) << ")" << std::endl;
    }

    void MqttClient::on_publish(int mid)
    {
        std::cout << ">> Mqtt Client - Message (" << mid << ") succeed to be published " << std::endl;
    }

    void MqttClient::on_message(const struct mosquitto_message *message)
    {
        std::cout << ">> Mqtt Client - Message (" << message->mid << " " << message->topic << ") succeed received"
                  << std::endl;
        if (m_recv_func != nullptr)
            m_recv_func(std::string((char *) message->payload));
    }

    void MqttClient::on_subscribe(int mid, int qos_count, const int *granted_qos)
    {
        std::cout << ">> Mqtt Client - Subscribe (" << mid << "): ";
        for (int i = 0; i < qos_count; i++)
            std::cout << granted_qos[i] << " ";
        std::cout << std::endl;
    }

    void MqttClient::on_unsubscribe(int mid)
    {
        std::cout << ">> Mqtt Client - Unsubscribe (" << mid << ")" << std::endl;
    }

    void MqttClient::on_log(int level, const char *str)
    {
        std::cout << ">> Mqtt Client - Log " << str << std::endl;
    }

    void MqttClient::on_error()
    {
        std::cerr << ">> Mqtt Client - Error Occured!" << std::endl;
    }

}


