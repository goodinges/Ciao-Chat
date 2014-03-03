/**
 * Copyright (C) 2011, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

package com.skype.ref;

import java.util.ArrayList;
import java.util.List;

import com.skype.ref.SkypeApp.Buddy;
import com.skype.ref.SkypeApp.State;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.Spinner;

public class SkypeKitVideoDemo extends Activity implements SkypeApp.updateObserver {
	public class runMode {
		public final static int NORMAL = 0;
		public final static int TEST_FILEPLAYBACK = 1;
		public final static int TEST_AUDIOPLAYBACK = 2;
		public final static int TEST_AUDIORECORD = 3;
	};
	private final static String TAG = "SkypeKitVideoDemo";
	private final static int PREVIEW_SURFACE = 1; 
	private final static int VIDEO_SURFACE = 2; 

	private int initEnabled = (PREVIEW_SURFACE | VIDEO_SURFACE);
	private static boolean running = false;

	private Button btnQuit;
	private Button btnDisconnect;
	private Button btnStopAVHosts;
	private Button btnStopKit;
	private Spinner spnRunMode;
	private Button btnStartKit;
	private Button btnInitAVHosts;
	private Button btnStartAVHosts;
	private Button btnConnect;
	private EditText editSkypeName;
	private EditText editPassword;
	private ImageView iconStatus;
	private Button btnLogin;
	private Button btnLogout;
	private ImageButton btnCallStart;
	private ImageButton btnCallEnd;
	private ListView listConversations;
	private ArrayAdapter<SkypeApp.Buddy> buddyAdapter;

	private SurfaceView preview;
	private SurfaceHolder previewHolder = null;
	private SurfaceView video;
	private SurfaceHolder videoHolder = null;

	private Context context = this;
	private SkypeKitRunner kitRunner = new SkypeKitRunner(context);
	private surfaceCallback videoCallback = new surfaceCallback();
	private SkypeApp app = new SkypeApp(this);
	private NativeCodeCaller nativeCode = new NativeCodeCaller();

	// Used to catch surface creation, to pass handles down to JNI
	private class surfaceCallback implements SurfaceHolder.Callback {
		public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
			if (holder == previewHolder) {
				Log.e(TAG, "preview surfaceChanged: f: " + format + ", w: " + width + ", h: " + height);

				nativeCode.setPreviewDisplay(holder.getSurface());
				initEnabled &= PREVIEW_SURFACE;
			} else if (holder == videoHolder) {
				Log.e(TAG, "video surfaceChanged: f: " + format + ", w: " + width + ", h: " + height);

				nativeCode.setVideoDisplay(holder.getSurface());
				initEnabled &= VIDEO_SURFACE;
			} else {
				Log.e(TAG, "Unknown surface: f: " + format + ", w: " + width + ", h: " + height);
				assert (false);
			}

			if ((!running) && (0 == initEnabled)) {
				if (!start()) {
					Log.e(TAG, "Startup failed");
				}
			}
			updateUIState();
		}

		public void surfaceCreated(SurfaceHolder holder) {
			Log.e(TAG, "surfaceCreated");
		}

		public void surfaceDestroyed(SurfaceHolder holder) {
			Log.e(TAG, "surfaceDestroyed");
			initEnabled = (VIDEO_SURFACE | PREVIEW_SURFACE);
		}
	}

	// Called when the activity is first created
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		// Wire up buttons
		btnQuit = (Button) findViewById(R.id.btnQuit);
		btnQuit.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onQuit();
			}
		});
		btnDisconnect = (Button) findViewById(R.id.btnDisconnect);
		btnDisconnect.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onDisconnect();
			}
		});
		btnStopAVHosts = (Button) findViewById(R.id.btnStopAVHosts);
		btnStopAVHosts.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onStopAVHosts();
			}
		});
		btnStopKit = (Button) findViewById(R.id.btnStopKit);
		btnStopKit.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onStopKit();
			}
		});
		spnRunMode = (Spinner) findViewById(R.id.spnRunmode);
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.mode_prompts, android.R.layout.simple_spinner_item);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spnRunMode.setAdapter(adapter);
		btnStartKit = (Button) findViewById(R.id.btnStartKit);
		btnStartKit.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onStartKit();
			}
		});
		btnInitAVHosts = (Button) findViewById(R.id.btnInitAVHosts);
		btnInitAVHosts.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onInitAVHosts();
			}
		});
		btnStartAVHosts = (Button) findViewById(R.id.btnStartAVHosts);
		btnStartAVHosts.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onStartAVHosts();
			}
		});
		btnConnect = (Button) findViewById(R.id.btnConnect);
		btnConnect.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onConnect();
			}
		});
		editSkypeName = (EditText) findViewById(R.id.editSkypeName);
		editPassword = (EditText) findViewById(R.id.editPassword);
		iconStatus = (ImageView) findViewById(R.id.status);
		btnLogin = (Button) findViewById(R.id.btnLogin);
		btnLogin.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onLogin();
			}
		});
		btnLogout = (Button) findViewById(R.id.btnLogout);
		btnLogout.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onLogout();
			}
		});
		btnCallStart = (ImageButton) findViewById(R.id.btnCallStart);
		btnCallStart.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onCallStart();
			}
		});
		btnCallEnd = (ImageButton) findViewById(R.id.btnCallEnd);
		btnCallEnd.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				onCallEnd();
			}
		});

		listConversations = (ListView) findViewById(R.id.listConversations);
		buddyAdapter = new ArrayAdapter<SkypeApp.Buddy>(this, android.R.layout.simple_list_item_checked, new ArrayList<SkypeApp.Buddy>());
		listConversations.setAdapter(buddyAdapter);
		listConversations.setOnItemClickListener(new ListView.OnItemClickListener() {
					public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
						OnContactClick(position, id);
					}
				});

		// Wire up SurfaceViews
		preview = (SurfaceView) findViewById(R.id.preview);
		video = (SurfaceView) findViewById(R.id.video);
		previewHolder = preview.getHolder();
		videoHolder = video.getHolder();
		previewHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		videoHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		previewHolder.addCallback(videoCallback);
		videoHolder.addCallback(videoCallback);

		updateUIState();
	}

	// Called when the activity is becoming visible to the user
	@Override
	public void onStart() {
		super.onStart();
	}

	// Called after your activity has been stopped, prior to it being started again
	@Override
	public void onRestart() {
		super.onRestart();
	}

	// Called when the activity will start interacting with the user 
	@Override
	protected void onResume() {
		super.onResume();
	}

	// Called when the system is about to start resuming a previous activity 
	@Override
	protected void onPause() {
		super.onPause();
	}

	// Called when the activity is no longer visible to the user, because another activity has been resumed and is covering this one
	@Override
	protected void onStop() {
		super.onStop();
		stop();
		this.finish();
	}

	// The final call you receive before your activity is destroyed
	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	public boolean start() {
		if (running) {
			Log.e(TAG, "Already running");
		}
		running = false;
		kitRunner.startApp(true);
		nativeCode.initAVHosts(runMode.NORMAL);
		nativeCode.startAVHosts();
		if (!app.connect()) {
			Log.e(TAG, "Failed to connect to SkypeKit");
			return (running);
		}
		app.login();
		running = true;
		updateUIState();
		return (running);
	}

	private void stop() {
		app.logout();
		app.disconnect();
		kitRunner.stopApp();
		nativeCode.stopAVHosts();
		running = false;
	}

	private void updateUIState() {
		boolean started = kitRunner.isAppRunning();
		boolean connected = app.isConnected();
		boolean loggedIn = !(app.getState() instanceof SkypeApp.LoggedOutState);
		boolean inCall = app.getState() instanceof SkypeApp.InCallState;
		boolean callRinging = app.getState() instanceof SkypeApp.CallRingingState;

		if (loggedIn) {
			iconStatus.setImageResource(R.drawable.status_online);
		} else {
			iconStatus.setImageResource(R.drawable.status_offline);
		}

		btnInitAVHosts.setEnabled(initEnabled == 0);
		btnStartKit.setEnabled(!started);
		btnStopKit.setEnabled(started);
		btnConnect.setEnabled(started && !connected);
		btnDisconnect.setEnabled(connected);
		btnLogin.setEnabled(connected && !loggedIn);
		btnLogout.setEnabled(loggedIn);
		btnCallStart.setEnabled(loggedIn && !inCall);
		btnCallEnd.setEnabled(loggedIn && inCall);
		if (callRinging) {
			btnCallStart.setImageResource(R.drawable.call_incoming);
		} else {
			btnCallStart.setImageResource(R.drawable.call_start);
		}
	}

	private void onStartKit() {
		kitRunner.startApp(true);
		updateUIState();
	}

	private void onStopKit() {
		kitRunner.stopApp();
		updateUIState();
	}

	private void onInitAVHosts() {
		long id = spnRunMode.getSelectedItemId();
		Log.e(TAG, "mode=" + id);
		nativeCode.initAVHosts((int) id);
	}

	private void onStartAVHosts() {
		nativeCode.startAVHosts();
		updateUIState();
	}

	private void onStopAVHosts() {
		nativeCode.stopAVHosts();
		updateUIState();
	}

	private void onConnect() {
		app.connect();
		updateUIState();
	}

	private void onDisconnect() {
		app.disconnect();
		updateUIState();
	}

	private void onQuit() {
		stop();
		this.finish();
	}

	private void onLogin() {
		String pass = editPassword.getText().toString();
		String user = editSkypeName.getText().toString();
		app.setAccountName(user);
		if (pass.length() > 0) {
			Log.e(TAG, "login " + user + " with password " + pass);
			app.login(pass);
		} else {
			Log.e(TAG, "Default account is not available.");
		}
		updateUIState();
	}

	private void onLogout() {
		app.logout();
	}

	private void onCallStart() {
		int pos = listConversations.getCheckedItemPosition();
		if (pos == AdapterView.INVALID_POSITION) {
			Log.e(TAG, "accept incoming call");
			app.startVideoCall(null);
		} else {
			Buddy b = buddyAdapter.getItem(pos);
			Log.e(TAG, "place a call to " + b.toString());
			app.startVideoCall(b);
		}
	}

	private void onCallEnd() {
		app.stopCall();
	}

	private void OnContactClick(int position, long id) {
	}

	public void onDefaultAccountChange(String defAccount) {
		editSkypeName.setText(defAccount);
	}

	final Handler handler = new Handler() {

	};

	public void onOnlineStatusChange(State state) {
		if (state instanceof SkypeApp.LoggedInState) {
			handler.post(new Runnable() {
				public void run() {
					app.getBuddyList();
				}
			});
		}
		handler.post(new Runnable() {
			public void run() {
				updateUIState();
			}
		});
	}

	public void onBuddyListChange(List<SkypeApp.Buddy> buddies) {
		buddyAdapter.clear();
		for (Buddy buddy : buddies)
			buddyAdapter.add(buddy);
		updateUIState();
	}
	
	public void onAlertMessage(String message)
	{
	    AlertDialog alertDialog = new AlertDialog.Builder(this).create();
	    alertDialog.setTitle("Fatal Error");
	    alertDialog.setMessage(message);
	    alertDialog.setButton("OK", new DialogInterface.OnClickListener() {
	        public void onClick(DialogInterface dialog, int which) {
	            return;
	        } });
	    
	    alertDialog.show();
	}
}
