#include <jni.h>
#include <oboe/Oboe.h>
#include <android/log.h>

#define LOG_TAG "ProMusicEngine"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// အသံပိုင်းဆိုင်ရာ တွက်ချက်မှုများ လုပ်ဆောင်မည့် Class
class AudioEngine : public oboe::AudioStreamCallback {
private:
    oboe::AudioStream *stream;
    bool isPlaying = false;
    float volume = 1.0f;
    
    // 10-Band Equalizer အတွက် Array (DSP တွက်ချက်ရန်)
    float eqBands[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

public:
    AudioEngine() {
        oboe::AudioStreamBuilder builder;
        
        // 5.1 Surround Sound အတွက် Channel ၆ ခု တောင်းခံခြင်း
        // (Front L/R, Center, Subwoofer, Rear L/R)
        builder.setChannelCount(6); 
        
        builder.setFormat(oboe::AudioFormat::Float);
        builder.setCallback(this);
        builder.setPerformanceMode(oboe::PerformanceMode::LowLatency); // အသံမထစ်စေရန် Low Latency သုံးခြင်း
        builder.setSharingMode(oboe::SharingMode::Exclusive);

        oboe::Result result = builder.openStream(&stream);
        if (result == oboe::Result::OK) {
            LOGI("Audio Stream Opened! 5.1 Channels Available: %d", stream->getChannelCount());
        }
    }

    void start() {
        if (stream) {
            stream->requestStart();
            isPlaying = true;
            LOGI("Audio Playback Started");
        }
    }

    void stop() {
        if (stream) {
            stream->requestStop();
            isPlaying = false;
        }
    }

    void setVolume(float v) { 
        volume = v; 
    }

    // ဒီနေရာက သီချင်းဖွင့်နေချိန် အသံလှိုင်းတွေကို အချိန်နဲ့တပြေးညီ Equalizer ချိန်းပေးမယ့် နေရာဖြစ်ပါတယ်
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override {
        float *floatData = (float *)audioData;
        int channelCount = oboeStream->getChannelCount();

        // Audio Frame တစ်ခုချင်းစီကို Volume နှင့် Equalizer ထိန်းချုပ်ခြင်း
        for (int i = 0; i < numFrames; ++i) {
            for (int c = 0; c < channelCount; ++c) {
                float sample = floatData[i * channelCount + c];
                
                // 5.1 Channel Routing & DSP Math တွေ ဒီနေရာမှာ ဆက်ရေးရပါမယ်
                floatData[i * channelCount + c] = sample * volume; 
            }
        }
        return oboe::DataCallbackResult::Continue;
    }
};

// --- JNI Bridge (UI နဲ့ C++ Audio Engine ကို ချိတ်ဆက်ပေးမည့် တံတား) ---
AudioEngine* engine = nullptr;

extern "C" JNIEXPORT void JNICALL
Java_com_promusic_app_AudioPlayer_startEngine(JNIEnv *env, jobject /* this */) {
    if (!engine) engine = new AudioEngine();
    engine->start();
}

extern "C" JNIEXPORT void JNICALL
Java_com_promusic_app_AudioPlayer_stopEngine(JNIEnv *env, jobject /* this */) {
    if (engine) engine->stop();
}

extern "C" JNIEXPORT void JNICALL
Java_com_promusic_app_AudioPlayer_setVolume(JNIEnv *env, jobject /* this */, jfloat vol) {
    if (engine) engine->setVolume(vol);
}
