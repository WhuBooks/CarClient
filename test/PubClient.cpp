//
// Created by books on 2019/1/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mosquitto.h>
#include <nlohmann/json.hpp>

#define HOST "localhost"
#define PORT  1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE  512

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

int main()
{
    int major,minor,revision;
    mosquitto_lib_version(&major,&minor,&revision);
    printf("%d\t%d\t%d\n",major,minor,revision);


	char buff[MSG_MAX_SIZE];
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
	//连接服务器
	if (mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE))
	{
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}
	//开启一个线程，在线程里不停的调用 mosquitto_loop() 来处理网络信息
	int loop = mosquitto_loop_start(mosq);
	if (loop != MOSQ_ERR_SUCCESS)
	{
		printf("mosquitto loop error\n");
		return 1;
	}
	while (fgets(buff, MSG_MAX_SIZE, stdin) != NULL)
	{
		/*发布消息*/
		// create a person
		person p {"Books", "744 Evergreen Terrace", 60};

		// conversion: person -> json
		nlohmann::json j = p;

		std::string j_str=j.dump();


		mosquitto_publish(mosq, NULL, "Gai :", j_str.length()+1, j_str.c_str(), 0, 0);
		memset(buff, 0, sizeof(buff));
	}
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return 0;
}
