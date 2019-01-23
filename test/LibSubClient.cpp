//
// Created by books on 19-1-22.
//

#include <iostream>
#include <MqttClient.h>
#include <nlohmann/json.hpp>

struct Iden{
    std::string name;
    int age;
};

void to_json(nlohmann::json &j,const Iden &obj)
{
    j=nlohmann::json{{"name",obj.name},{"age",obj.age}};
}

void from_json(const nlohmann::json &j,Iden &obj)
{
    obj.name=j.at("name").get<std::string>();
    obj.age=j.at("age").get<int>();
}

void msg_cb(const std::string &msg)
{
//    nlohmann::json j=nlohmann::json::parse(msg);
    Iden iden=nlohmann::json::parse(msg);
    std::cout<<iden.name<<"\t"<<iden.age<<std::endl;
}

int main()
{
    CarClient::MqttClient::init();
    
    CarClient::MqttClient client;
    client.initialRecv("books",msg_cb);

    std::this_thread::sleep_for(std::chrono::minutes(2));

//    while(1)
//    {
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    }

    CarClient::MqttClient::clean();
    return 1;
}