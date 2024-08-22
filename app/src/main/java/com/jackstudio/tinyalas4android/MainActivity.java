package com.jackstudio.tinyalas4android;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;

import com.tinyalas4android.library.PcmReader;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    private PcmReader pcmReader4;
    private PcmReader pcmReader5;

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        pcmReader4 = new PcmReader(4, 0, callback4);
        pcmReader5 = new PcmReader(5, 0, callback5);

        findViewById(R.id.start).setOnClickListener(v -> {
            pcmReader4.start();
            pcmReader5.start();
        });
        findViewById(R.id.stop).setOnClickListener(v -> {
            if (pcmReader4 != null) {
                pcmReader4.stop();
            }
            if (pcmReader5 != null) {
                pcmReader5.stop();
            }
        });
    }

    private PcmReader.PcmDataCallback callback4 = new PcmReader.PcmDataCallback() {
        @Override
        public void onPcmData(byte[] data) {
            Log.i(TAG, "pcmReader: data 4= " + data.toString());

        }
    };
    private PcmReader.PcmDataCallback callback5 = new PcmReader.PcmDataCallback() {
        @Override
        public void onPcmData(byte[] data) {
            Log.i(TAG, "pcmReader: data 5= " + data.toString());

        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        pcmReader4.stop();
    }
}