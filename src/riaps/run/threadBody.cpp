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


extern "C" void* thread1(std::string addr) {
    pthread_t tid = pthread_self();
    // std::cout << "Thread: Process ID: " << getpid() << " Thread ID: " << tid << std::endl;

    // CppComponent testComp(NULL);

    // std::cout << "Test Component Name: " << testComp.getName() << std::endl;

    zmq::context_t ctx;
    zmq::socket_t sock(ctx, ZMQ_PAIR);
    // sock.connect("inproc://part_TEST_control");
    sock.connect(addr);

    int i = 0;
    nlohmann::json send;
    std::string send_str;
    zmq::message_t reply;
    nlohmann::json reply_json;

    while (i < 100000) {
        send["Contents"] = "Ready";
        send_str = send.dump();
        // std::cout << "Thread: sending: " << send_str << std::endl;
        // printf("Thread %l sending: ", tid, send_str);
        // fflush();
        zmq::message_t query(send_str.length());
        std::memcpy(query.data(), (send_str.c_str()), (send_str.size()));
        sock.send(query);

        sock.recv(&reply);
        reply_json = nlohmann::json::parse(reply.to_string());
        // std::cout << "Thread: reply received: \n" << reply_json.dump(4) << std::endl;
        // std::cout << "Value of Point: " << reply_json["__value__"]["values"][1] << std::endl;
        i = (int)reply_json["__value__"]["values"][1];
    }
    
    sock.close();
    ctx.close();

    pthread_exit(NULL);
}
