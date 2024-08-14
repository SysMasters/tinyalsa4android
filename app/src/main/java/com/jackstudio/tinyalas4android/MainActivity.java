package com.jackstudio.tinyalas4android;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    private PcmReader pcmReader;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        pcmReader = new PcmReader(data -> {
            Log.i(TAG, "pcmReader: data = " + data.toString());
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        pcmReader.release();
    }
}