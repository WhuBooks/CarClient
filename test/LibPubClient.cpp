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

int main()
{
    CarClient::MqttClient::init();

    CarClient::MqttClient client;
    client.initialSend("books");

    for(int i=0;i<10000;i++)
    {
//        client.sendMsg("I am books\t"+std::to_string(i));
        Iden iden;
        iden.name="Books";
        iden.age=i;
//        nlohmann::json j=iden;
//        client.sendMsg(j.dump());

        client.sendMsg(nlohmann::json(iden).dump());

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    CarClient::MqttClient::clean();
}