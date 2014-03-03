package com.skype.test;

import com.skype.api.Account;
import com.skype.api.Account.AccountListener;
import com.skype.api.Account.PROPERTY;
import com.skype.api.SkypeObject;


public class LoginListener extends Thread implements Callback,
        AccountListener
{

	private Callback	callback;
	Account.STATUS	 status;
	Account.LOGOUTREASON reason;

	public LoginListener(final Callback callback) {
		this.callback = callback;
	}

	@Override
	public void run()
	{
		try {
			while (true) {
				System.out.print(".");
				sleep(30);
			}
		}
		catch (InterruptedException e) {
			System.out.println("listener interrupted, bye");
		}
	}

	@Override
	public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value)
	{
		if (prop == Account.PROPERTY.status) {
			status = Account.STATUS.get((Integer)value);
			
			System.out.println("\nstatus:" + status);
			
			switch (status) {
			case LOGGED_IN:
				callback.completed();
			}
		}
		else if (prop == Account.PROPERTY.logoutreason) {
			reason = Account.LOGOUTREASON.get((Integer)value);

			System.out.println("\nlogoutreason:" + reason);
			callback.completed();
		}
		else {
			System.out.println("\nAccount prop change: " + prop + ", " + value);
		}
	}

	@Override
	public void completed()
	{
	}

	public Account.LOGOUTREASON getReason()
	{
		return reason;
	}
	
	public Account.STATUS getStatus()
	{
		return status;
	}

}
