# CMake ရဲ့ အနည်းဆုံး လိုအပ်တဲ့ version
cmake_minimum_required(VERSION 3.4.1)

# App ရဲ့ C++ Engine နာမည်ကို သတ်မှတ်ခြင်း
add_library(
        promusicengine
        SHARED
        main.cpp )

# Android ရဲ့ Log တွေကို ဖမ်းယူဖို့ Library ရှာခြင်း
find_library(
        log-lib
        log )

# C++ Code ကို Android System နဲ့ Oboe (Audio Library) ဆီ ချိတ်ဆက်ပေးခြင်း
target_link_libraries(
        promusicengine
        ${log-lib}
        oboe )
