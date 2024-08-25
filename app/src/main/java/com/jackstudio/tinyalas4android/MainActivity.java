package com.jackstudio.tinyalas4android;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import com.lib.audio.wav.WavFileWriter;
import com.lib.audio.wav.WavHeader;
import com.tinyalas4android.library.PcmConfig;
import com.tinyalas4android.library.PcmReader;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    private PcmReader pcmReader4;
    private PcmReader pcmReader5;

    private WavFileWriter writer;

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        pcmReader4 = new PcmReader(3, 0, callback4);
        pcmReader5 = new PcmReader(4, 0, callback5);

        findViewById(R.id.start).setOnClickListener(v -> {
            pcmReader4.start();
//            pcmReader5.start();

            PcmConfig pcmConfig = pcmReader4.getPcmConfig();
            WavHeader header = new WavHeader(pcmConfig.getRate(), pcmConfig.getChannels(), pcmConfig.getFormat());
            File file = new File(Environment.getExternalStorageDirectory(), "text_test.wav");
            if (file.exists()) {
                file.delete();
            }
            writer = new WavFileWriter();
            try {
                writer.openFile(file.getAbsolutePath(), header);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        findViewById(R.id.stop).setOnClickListener(v -> {
            if (pcmReader4 != null) {
                pcmReader4.stop();
            }
            if (pcmReader5 != null) {
                pcmReader5.stop();
            }
            try {
                writer.closeFile();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }

    private PcmReader.PcmDataCallback callback4 = new PcmReader.PcmDataCallback() {
        @Override
        public void onPcmData(byte[] data, int size) {
//            Log.i(TAG, "pcmReader: data 4= " + Arrays.toString(data) + "===size:" + size);
            Log.i(TAG, "pcmReader: data 3= " + data + "===size:" + size+"===data.length:"+data.length);
            writer.writeData(data, 0, data.length);

        }
    };
    private PcmReader.PcmDataCallback callback5 = new PcmReader.PcmDataCallback() {
        @Override
        public void onPcmData(byte[] data, int size) {
            Log.i(TAG, "pcmReader: data 4= " + data + "===size:" + size+"===data.length:"+data.length);

        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        pcmReader4.stop();
    }
}