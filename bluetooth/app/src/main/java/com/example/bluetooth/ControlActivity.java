package com.example.bluetooth;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.Toast;

public class ControlActivity extends AppCompatActivity implements View.OnClickListener {
    private static final String TAG = "ControlActivity";

    private ChatUtils chatUtils;
    public static final int MESSAGE_STATE_CHANGED = 0;
    public static final int MESSAGE_READ = 1;
    public static final int MESSAGE_WRITE = 2;
    public static final int MESSAGE_DEVICE_NAME = 3;
    public static final int MESSAGE_TOAST = 4;

    public static final String DEVICE_NAME = "deviceName";
    public static final String TOAST = "toast";

    private Context context;

    private String connectedDevice;

    private Handler handler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message message) {
            switch (message.what) {
                case MESSAGE_STATE_CHANGED:
                    switch (message.arg1) {
                        case ChatUtils.STATE_NONE:
                            setState("Not Connected");
                            break;
                        case ChatUtils.STATE_LISTEN:
                            setState("Not Connected");
                            break;
                        case ChatUtils.STATE_CONNECTING:
                            setState("Connecting...");
                            break;
                        case ChatUtils.STATE_CONNECTED:
                            setState("Connected: " + connectedDevice);
                            break;
                    }
                    break;
                case MESSAGE_WRITE:
                    byte[] buffer1 = (byte[]) message.obj;
                    String outputBuffer = new String(buffer1);
                    Log.d(TAG, "outputBuffer = " + outputBuffer);
                    break;
                case MESSAGE_READ:
                    byte[] buffer = (byte[]) message.obj;
                    String inputBuffer = new String(buffer, 0, message.arg1);
                    // adapterMainChat.add(connectedDevice + ": " + inputBuffer);
                    Log.d(TAG, "inputBuffer = " + inputBuffer);
                    break;
                case MESSAGE_DEVICE_NAME:
                    connectedDevice = message.getData().getString(DEVICE_NAME);
                    Toast.makeText(context, connectedDevice, Toast.LENGTH_SHORT).show();
                    break;
                case MESSAGE_TOAST:
                    Toast.makeText(context, message.getData().getString(TOAST), Toast.LENGTH_SHORT).show();
                    break;
            }
            return false;
        }
    });

    private void setState(CharSequence subTitle) {
        getSupportActionBar().setSubtitle(subTitle);
        Log.d(TAG, "subTitle = " + subTitle);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);
        chatUtils = new ChatUtils(this, handler);

        String address = getIntent().getStringExtra("devicesAddress");
        chatUtils.connect(BluetoothAdapter.getDefaultAdapter().getRemoteDevice(address));

        context = this;
        Button up = findViewById(R.id.up);
        Button left = findViewById(R.id.left);
        Button right = findViewById(R.id.right);
        Button back = findViewById(R.id.back);
        Button stop = findViewById(R.id.stop);
        Button led = findViewById(R.id.led);
        Button highSpeed = findViewById(R.id.speed_high);
        Button normalSpeed = findViewById(R.id.speed_normal);

        stop.setOnClickListener(this);
        up.setOnClickListener(this);
        left.setOnClickListener(this);
        right.setOnClickListener(this);
        back.setOnClickListener(this);
        led.setOnClickListener(this);
        highSpeed.setOnClickListener(this);
        normalSpeed.setOnClickListener(this);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (chatUtils != null) {
            chatUtils.stop();
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.back:
                chatUtils.write(new byte[]{Constant.BACK});
                break;
            case R.id.up:
                chatUtils.write(new byte[]{Constant.FRONT});
                break;
            case R.id.left:
                chatUtils.write(new byte[]{Constant.LEFT});
                break;
            case R.id.right:
                chatUtils.write(new byte[]{Constant.RIGHT});
                break;
            case R.id.stop:
                chatUtils.write(new byte[]{Constant.STOP});
                break;
            case R.id.led:
                chatUtils.write(new byte[]{Constant.LED});
                break;
            case R.id.speed_high:
                chatUtils.write(new byte[]{Constant.SPEED_HIGH});
                break;
            case R.id.speed_normal:
                chatUtils.write(new byte[]{Constant.SPEED_NORMAL});
                break;
            default:
                break;
        }
    }
}