/*
C++ Thread Body File
Created on Sept. 21, 2023

@author: EthanMayer
*/

#include <pthread.h>
#include <iostream>
#include <zmq.hpp>
#include <unistd.h>
#include "json.hpp"


extern "C" void* thread1(void*) {
    pthread_t tid = pthread_self();
    // long pid = getpid();
    std::cout << "Thread: Process ID: " << getpid() << " Thread ID: " << tid << std::endl;

    // CppComponent testComp(NULL);

    // std::cout << "Test Component Name: " << testComp.getName() << std::endl;

    zmq::context_t ctx;
    zmq::socket_t sock(ctx, ZMQ_PAIR);
    // zmq::socket_t sock(ctx, ZMQ_REQ);
    // sock.connect("inproc://part_TEST_control");
    sock.connect("tcp://localhost:5555");

    nlohmann::json send;
    send["Contents"] = "Ready";
    std::string send_str = send.dump();
    std::cout << "Thread: sending: " << send_str << std::endl;
    zmq::message_t query(send_str.length());
    memcpy(query.data(), (send_str.c_str()), (send_str.size()));
    sock.send(query);

    zmq::message_t reply;
    sock.recv(&reply);
    nlohmann::json reply_json = nlohmann::json::parse(reply.to_string());
    // nlohmann::json reply_json = nlohmann::json::parse(std::string(static_cast<char*>(reply.data()), reply.size()));
    std::cout << "Thread: reply received: \n" << reply_json.dump(4) << std::endl;

    pthread_exit(NULL);
}
