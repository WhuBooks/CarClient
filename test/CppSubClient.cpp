//
// Created by books on 19-1-21.
//

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <algorithm>

#include <mosquittopp.h>
#include <nlohmann/json.hpp>

// a simple struct to model a person
struct person
{
    std::string name;
    std::string address;
    int age;
};

void to_json(nlohmann::json &j, const person &p)
{
    j = nlohmann::json{{"name",    p.name},
                       {"address", p.address},
                       {"age",     p.age}};
}

void from_json(const nlohmann::json &j, person &p)
{
    p.name = j.at("name").get<std::string>();
    p.address = j.at("address").get<std::string>();
    p.age = j.at("age").get<int>();
}


class MqttClientTest:public mosqpp::mosquittopp
{
public:

    MqttClientTest(const std::string &_id, const std::string &_topic, const std::string &_host="localhost", const int &_port=1883)
            :mosquittopp(_id.c_str(),false)
    {
        // Mandatory initialization for mosquitto library
        this->keepalive = 60;    // Basic configuration setup for myMosq class
        this->id = _id;
        this->port = _port;
        this->host = _host;
        this->topic = _topic;

        // non blocking connection to broker request
        connect_async(host.c_str(), port, keepalive);
//        connect(host.c_str(),port,keepalive);
        // Start thread managing connection / publish / subscribe
        subscribe(nullptr,topic.c_str(),0);
        loop_start();
//        loop_forever();
    }

    ~MqttClientTest()
    {
        disconnect();
        loop_stop();
        mosqpp::lib_cleanup();
    }

    bool sendMessage(const std::string &msg)
    {
        int ret = publish(NULL, topic.c_str(), msg.length(), msg.c_str(), 0, false);
        return (ret == MOSQ_ERR_SUCCESS);
    }

private:

    std::string host;
    std::string id;
    std::string topic;
    int port;
    int keepalive;

    std::string msg;
    std::thread pthread;

    void on_connect(int rc)
    {
        if (rc == 0)
        {
            std::cout << ">> Mqtt Client - Connected with server" << std::endl;
//            subscribe(nullptr,topic.c_str(),0);
        }
        else
        {
            std::cout << ">> Mqtt Client - Impossible to connect with server(" << rc << ")" << std::endl;
        }
    };

    void on_disconnect(int rc)
    {
        std::cout << ">> Mqtt Client - Disconnection (" << rc << ")" << std::endl;
    };

    void on_publish(int mid)
    {
        std::cout << ">> Mqtt Client - Message (" << mid << ") succeed to be published " << std::endl;
    };

    void on_message(const struct mosquitto_message *message)
    {
        std::cout<<">> Mqtt Client - Message ("<<message->mid<<" "<<message->topic<<") succeed received"<<std::endl;
        std::cout<<(char *)message->payload<<std::endl;
    };

    void on_subscribe(int mid, int qos_count, const int *granted_qos)
    {
        std::cout<<">> Mqtt Client - Subscribe ("<<mid<<"): ";
        for(int i=0;i<qos_count;i++)
            std::cout<<granted_qos[i]<<" ";
        std::cout<<std::endl;
    };

    void on_unsubscribe(int mid)
    {
        std::cout << ">> Mqtt Client - Unsubscribe (" << mid << ")" << std::endl;
    };

    void on_log(int level, const char *str)
    {
//        std::cout<<">> Mqtt Client - Log "<<str<<std::endl;
    };

    void on_error()
    {
        std::cerr<<">> Mqtt Client - Error Occured!"<<std::endl;
    };

};

int main()
{
    mosqpp::lib_init();
    MqttClientTest client("id_recv","Books");
//    client.loop_forever();
    while(1)
    {
//        std::cout<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 1;

}