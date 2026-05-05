cmake_minimum_required(VERSION 3.22.1)
project("promusicengine")

# Oboe Library ကို ရှာဖွေခြင်း
find_package(oboe REQUIRED CONFIG)

add_library(promusicengine SHARED main.cpp)

find_library(log-lib log)

# Oboe ကို ကိုယ်ပိုင် C++ Engine နှင့် ချိတ်ဆက်ခြင်း
target_link_libraries(promusicengine ${log-lib} oboe::oboe)
