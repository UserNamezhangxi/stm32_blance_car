package com.example.bluetooth;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;

public class DeviceListActivity extends AppCompatActivity {
    private static final String TAG = "DeviceListActivity";
    private ListView pairListView;
    private ListView availableListView;
    private ArrayAdapter<String> pairAdapter;
    private ArrayAdapter<String> availableAdapter ;
    private ProgressBar progressBar;
    private Button btnScan;
    private BluetoothAdapter bluetoothAdapter;

    private BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(BluetoothDevice.ACTION_FOUND)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                    availableAdapter.add(device.getName() + "\n" + device.getAddress());

                } else if (intent.getAction().equals(BluetoothAdapter.ACTION_DISCOVERY_FINISHED)) {
                    progressBar.setVisibility(View.GONE);
                    if (availableAdapter.getCount() == 0) {
                        Toast.makeText(DeviceListActivity.this,"没有发现任何设备", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(DeviceListActivity.this,"点击设备进行连接", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_list);
        init();
    }

    private void init() {
        btnScan = findViewById(R.id.btn_scan);
        progressBar = findViewById(R.id.progress_bar);
        pairListView = findViewById(R.id.list_pair_devices);
        availableListView = findViewById(R.id.list_available_devices);
        pairAdapter = new ArrayAdapter<String>(this, R.layout.device_list_item);
        availableAdapter = new ArrayAdapter<String>(this, R.layout.device_list_item);
        pairListView.setAdapter(pairAdapter);
        availableListView.setAdapter(availableAdapter);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        Set<BluetoothDevice> pairDevices = bluetoothAdapter.getBondedDevices();
        if (pairDevices != null && pairDevices.size() > 0 ) {
            for (BluetoothDevice device : pairDevices) {
                pairAdapter.add(device.getName() + "\n" + device.getAddress() );
            }
        }
        pairListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                String info = ((TextView)view).getText().toString();
                Log.d(TAG,"info=" + info);
                String address = info.substring(info.length() - 17);

                Intent intent = new Intent(DeviceListActivity.this, ControlActivity.class);
                intent.putExtra("devicesAddress", address);
                startActivity(intent);
            }
        });
        availableListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                String info = ((TextView)view).getText().toString();
                Log.d(TAG,"info=" + info);
                String address = info.substring(info.length() - 17);

                Intent intent = new Intent(DeviceListActivity.this, ControlActivity.class);
                intent.putExtra("devicesAddress", address);
                startActivity(intent);
            }
        });

        btnScan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                progressBar.setVisibility(View.VISIBLE);
                availableAdapter.clear();
                if (bluetoothAdapter.isDiscovering()) {
                    bluetoothAdapter.cancelDiscovery();
                }
                bluetoothAdapter.startDiscovery();
            }
        });

        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        IntentFilter filter1 = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(receiver,filter);
        registerReceiver(receiver,filter1);


    }
}