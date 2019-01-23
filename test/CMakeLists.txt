
add_executable(PubClient PubClient.cpp)

add_executable(SubClient SubClient.cpp)

add_executable(JsonTest JsonTest.cpp)

add_executable(CppPubClient CppPubClient.cpp)

add_executable(CppSubClient CppSubClient.cpp)

add_executable(LibPubClient LibPubClient.cpp)
target_link_libraries(LibPubClient ClientLib)

add_executable(LibSubClient LibSubClient.cpp)
target_link_libraries(LibSubClient ClientLib)

