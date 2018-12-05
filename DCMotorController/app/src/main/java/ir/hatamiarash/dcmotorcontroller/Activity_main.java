package ir.hatamiarash.dcmotorcontroller;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.github.anastr.speedviewlib.PointerSpeedometer;

import app.akexorcist.bluetotohspp.library.BluetoothSPP;
import app.akexorcist.bluetotohspp.library.BluetoothState;
import app.akexorcist.bluetotohspp.library.DeviceList;
import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import mehdi.sakout.fancybuttons.FancyButton;

public class Activity_main extends AppCompatActivity {
	
	BluetoothSPP bluetooth;
	
	@BindView(R.id.status)
	TextView status;
	@BindView(R.id.start)
	FancyButton start;
	@BindView(R.id.stop)
	FancyButton stop;
	@BindView(R.id.speed_up)
	FancyButton speed_up;
	@BindView(R.id.speed_down)
	FancyButton speed_down;
	@BindView(R.id.pointerSpeedometer)
	PointerSpeedometer speedometer;
	
	@OnClick(R.id.start)
	void startMotor() {
		sendCode("1");
	}
	
	@OnClick(R.id.stop)
	void stopMotor() {
		sendCode("2");
	}
	
	@OnClick(R.id.speed_up)
	void speedUP() {
		sendCode("3");
	}
	
	@OnClick(R.id.speed_down)
	void speedDown() {
		sendCode("4");
	}
	
	@SuppressLint("SetTextI18n")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		ButterKnife.bind(this);
		
		speedometer.setMinSpeed(0);
		speedometer.setMaxSpeed(255);
		speedometer.setTrembleData(0, 0);
		speedometer.speedTo(55);
		
		bluetooth = new BluetoothSPP(this);
		
		bluetooth.setBluetoothConnectionListener(new BluetoothSPP.BluetoothConnectionListener() {
			public void onDeviceConnected(String name, String address) {
				setStatus("Device Connected");
			}
			
			public void onDeviceDisconnected() {
				setStatus("Device Disconnected");
			}
			
			public void onDeviceConnectionFailed() {
				setStatus("Connection Failed");
			}
		});
		
		
		if (!bluetooth.isBluetoothAvailable()) {
			setStatus("Bluetooth is not available");
		} else {
			if (!bluetooth.isBluetoothEnabled()) {
				setStatus("Bluetooth is not enable");
				bluetooth.enable();
			} else {
				bluetooth.setupService();
				bluetooth.startService(false);
				setStatus("Connecting ...");
				Intent intent = new Intent(getApplicationContext(), DeviceList.class);
				startActivityForResult(intent, BluetoothState.REQUEST_CONNECT_DEVICE);
			}
		}
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == BluetoothState.REQUEST_CONNECT_DEVICE) {
			if (resultCode == Activity.RESULT_OK)
				bluetooth.connect(data);
		} else if (requestCode == BluetoothState.REQUEST_ENABLE_BT) {
			if (resultCode == Activity.RESULT_OK) {
				bluetooth.setupService();
				bluetooth.startService(BluetoothState.DEVICE_ANDROID);
				setStatus("Connected");
			} else {
				setStatus("Error !!");
			}
		}
	}
	
	private void setStatus(String text) {
		status.setText(text);
		Log.w("BLUETOOTH", text);
	}
	
	private void sendCode(String text) {
		bluetooth.send(text, true);
		try {
			Thread.sleep(500);
		} catch (InterruptedException ignore) {
		}
	}
}
