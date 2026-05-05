package com.promusic.app;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

public class MainActivity extends Activity {

    // C++ (main.cpp) ထဲက Code တွေကို လှမ်းခေါ်ဖို့ Library ကို Load လုပ်ခြင်း
    static {
        System.loadLibrary("promusicengine");
    }

    // C++ ဘက်မှာ ရေးထားတဲ့ Function တွေကို Java ဘက်ကနေ လှမ်းသုံးနိုင်ရန် ကြေငြာခြင်း
    public native void startEngine();
    public native void stopEngine();
    public native void setVolume(float volume);

    private boolean isPlaying = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // ui.xml ကို ဖုန်းမျက်နှာပြင်မှာ ပြသရန်
        int layoutId = getResources().getIdentifier("ui", "layout", getPackageName());
        setContentView(layoutId);

        // ui.xml ထဲက Play/Pause ခလုတ်ကို ရှာပြီး ချိတ်ဆက်ခြင်း
        int playBtnId = getResources().getIdentifier("btnPlayPause", "id", getPackageName());
        ImageView btnPlayPause = findViewById(playBtnId);

        // ခလုတ်နှိပ်လိုက်တဲ့အခါ အလုပ်လုပ်မည့် လုပ်ဆောင်ချက်
        btnPlayPause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isPlaying) {
                    stopEngine(); // C++ Engine ကို ရပ်ခိုင်းခြင်း
                    Toast.makeText(MainActivity.this, "Paused", Toast.LENGTH_SHORT).show();
                } else {
                    startEngine(); // C++ Engine ကနေ 5.1 Channel ဖွင့်ခိုင်းခြင်း
                    Toast.makeText(MainActivity.this, "Playing 5.1 Surround Audio...", Toast.LENGTH_SHORT).show();
                }
                isPlaying = !isPlaying; // အခြေအနေကို ပြောင်းလဲခြင်း
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        stopEngine(); // App ကို ပိတ်လိုက်ရင် သီချင်းပါ ရပ်သွားစေရန်
    }
}
