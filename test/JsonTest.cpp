//
// Created by books on 19-1-21.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>

#include <nlohmann/json.hpp>
#include <bits/unordered_set.h>
#include <chrono>

using json=nlohmann::json;

void test1()
{
    // create an empty structure (null)
    json j;

    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["pi"] = 3.141;

    // add a Boolean that is stored as bool
    j["happy"] = true;

    // add a string that is stored as std::string
    j["name"] = "Niels";

    // add another null object by passing nullptr
    j["nothing"] = nullptr;

    // add an object inside the object
    j["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j["list"] = {1, 0, 2};

    // add another object (using an initializer list of pairs)
    j["object"] = {{"currency", "USD"},
                   {"value",    42.99}};

    std::cout << j.dump(4) << std::endl;

    // instead, you could also write (which looks very similar to the JSON above)
    json j2 = {
            {"pi",      3.141},
            {"happy",   true},
            {"name",    "Niels"},
            {"nothing", nullptr},
            {"answer",  {{"everything", 42}}},
            {"list",    {       1, 0, 2}},
            {"object",  {{"currency",   "USD"}, {"value", 42.99}}}
    };

    std::cout << j2.dump(4) << std::endl;

    // a way to express the empty array []
    json empty_array_explicit = json::array();
    std::cout<<empty_array_explicit.dump(4)<<std::endl;

    // ways to express the empty object {}
    json empty_object_implicit = json({});
    std::cout<<empty_object_implicit.dump(4)<<std::endl;

    json empty_object_explicit = json::object();
    std::cout<<empty_object_explicit.dump(4)<<std::endl;

    // a way to express an _array_ of key/value pairs [["currency", "USD"], ["value", 42.99]]
    json array_not_object = json::array({ {"currency", "USD"}, {"value", 42.99} });
    std::cout<<array_not_object.dump(4)<<std::endl;
}

void test2()
{
    // create object from string literal
    json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;
    std::cout << j.dump(4) << std::endl;

    // or even nicer with a raw string literal
    auto j2 = R"({ "happy": true, "pi": 3.141})"_json;
    std::cout << j2.dump(4) << std::endl;

    // parse explicitly
    auto j3 = json::parse("{ \"happy\": true, \"pi\": 3.141 }");
    std::cout << j3.dump(4) << std::endl;

    // store a string in a JSON value
    json j_string = "this is a string";

    // retrieve the string value (implicit JSON to std::string conversion)
    std::string cpp_string = j_string;
    // retrieve the string value (explicit JSON to std::string conversion)
    auto cpp_string2 = j_string.get<std::string>();

    // retrieve the serialized value (explicit JSON serialization)
    std::string serialized_string = j_string.dump();

    // output of original string
    std::cout << cpp_string << " == " << cpp_string2 << " == " << j_string.get<std::string>() << '\n';
    // output of serialized value
    std::cout << j_string << " == " << serialized_string << std::endl;
}


void test3()
{
    std::vector<int> c_vector {1, 2, 3, 4};
    json j_vec(c_vector);
    std::cout<<j_vec.dump(4)<<std::endl;
// [1, 2, 3, 4]

    std::deque<double> c_deque {1.2, 2.3, 3.4, 5.6};
    json j_deque(c_deque);
    std::cout<<j_deque.dump(4)<<std::endl;
// [1.2, 2.3, 3.4, 5.6]

    std::list<bool> c_list {true, true, false, true};
    json j_list(c_list);
    std::cout<<j_list.dump(4)<<std::endl;
// [true, true, false, true]

    std::forward_list<int64_t> c_flist {12345678909876, 23456789098765, 34567890987654, 45678909876543};
    json j_flist(c_flist);
    std::cout<<j_flist.dump(4)<<std::endl;
// [12345678909876, 23456789098765, 34567890987654, 45678909876543]

    std::array<unsigned long, 4> c_array {{1, 2, 3, 4}};
    json j_array(c_array);
    std::cout<<j_array.dump(4)<<std::endl;
// [1, 2, 3, 4]

    std::set<std::string> c_set {"one", "two", "three", "four", "one"};
    json j_set(c_set); // only one entry for "one" is used
    std::cout<<j_set.dump(4)<<std::endl;
// ["four", "one", "three", "two"]

    std::unordered_set<std::string> c_uset {"one", "two", "three", "four", "one"};
    json j_uset(c_uset); // only one entry for "one" is used
    std::cout<<j_uset.dump(4)<<std::endl;
// maybe ["two", "three", "four", "one"]

    std::multiset<std::string> c_mset {"one", "two", "one", "four"};
    json j_mset(c_mset); // both entries for "one" are used
    std::cout<<j_mset.dump(4)<<std::endl;
// maybe ["one", "two", "one", "four"]

    std::unordered_multiset<std::string> c_umset {"one", "two", "one", "four"};
    json j_umset(c_umset); // both entries for "one" are used
    std::cout<<j_umset.dump(4)<<std::endl;
// maybe ["one", "two", "one", "four"]

    std::map<std::string, int> c_map { {"one", 1}, {"two", 2}, {"three", 3} };
    json j_map(c_map);
    std::cout<<j_map.dump(4)<<std::endl;
// {"one": 1, "three": 3, "two": 2 }

    std::unordered_map<const char*, double> c_umap { {"one", 1.2}, {"two", 2.3}, {"three", 3.4} };
    json j_umap(c_umap);
    std::cout<<j_umap.dump(4)<<std::endl;
// {"one": 1.2, "two": 2.3, "three": 3.4}

    std::multimap<std::string, bool> c_mmap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
    json j_mmap(c_mmap); // only one entry for key "three" is used
    std::cout<<j_mmap.dump(4)<<std::endl;
// maybe {"one": true, "two": true, "three": true}

    std::unordered_multimap<std::string, bool> c_ummap { {"one", true}, {"two", true}, {"three", false}, {"three", true} };
    json j_ummap(c_ummap); // only one entry for key "three" is used
    std::cout<<j_ummap.dump(4)<<std::endl;
// maybe {"one": true, "two": true, "three": true}

}


// a simple struct to model a person
struct person {
    std::string name;
    std::string address;
    int age;
};
void to_json(json& j, const person& p)
{
    j = json{{"name",    p.name},
             {"address", p.address},
             {"age",     p.age}};
}

void from_json(const json& j, person& p)
{
    p.name = j.at("name").get<std::string>();
    p.address = j.at("address").get<std::string>();
    p.age = j.at("age").get<int>();
}

void test4()
{
    // create a person
    person p {"Ned Flanders", "744 Evergreen Terrace", 60};

    // conversion: person -> json
    json j = p;

    std::cout << j.dump(4) << std::endl;
    // {"address":"744 Evergreen Terrace","age":60,"name":"Ned Flanders"}

    // conversion: json -> person
    person p2 = j;

    // that's it
    assert(p.name == p2.name && p.age==p2.age && p.address==p2.address);
}

struct A{

    std::vector<int> vec;

    std::map<int,int> map2;
//    std::vector<std::pair<int,int>> map2;
    std::map<std::string,int> map;
};

void to_json(nlohmann::json &j,const A &a)
{
    j=nlohmann::json{{"vec",a.vec},
                     {"map",a.map},
                     {"map2",a.map2}};
}

void from_json(const nlohmann::json &j,A &a)
{
    a.vec=j.at("vec").get<std::vector<int>>();
    a.map=j.at("map").get<std::map<std::string,int>>();
    std::vector<std::pair<int,int>> pairs=j.at("map2").get<std::vector<std::pair<int,int>>>();
    a.map2.insert(pairs.begin(),pairs.end());
}

void test5()
{
    A a;
    a.vec={1,2,3};
    a.map["0"]=0;
    a.map["1"]=1;
    a.map2[0]=0;
    a.map2[1]=1;
//    a.map2.push_back(std::make_pair(0,0));
//    a.map2.push_back(std::make_pair(1,1));


    nlohmann::json j1=a;
    std::cout<<j1.dump(4)<<std::endl;


    std::chrono::steady_clock::time_point tp1=std::chrono::steady_clock::now();
    for(int i=0;i<1000;i++)
        A a2=j1;
//    std::cout<<a2.vec.size()<<std::endl;
    std::chrono::steady_clock::time_point tp2=std::chrono::steady_clock::now();
    std::chrono::milliseconds timespan=std::chrono::duration_cast<std::chrono::milliseconds>(tp2-tp1);
    std::cout<<timespan.count()<<std::endl;

}


int main(int argc, char **argv)
{
    test5();


    return 1;
}