/**
 * Copyright (C) 2010, Skype Limited
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

package com.skype.ipc;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import com.skype.ipc.Event;
import com.skype.ipc.PropertyChange;

import com.skype.util.Log;

public class EventDispatcher extends Thread {
  private static final String TAG = "EventDispatcher";  
  private final BlockingQueue<AbstractDecoder> event_queue;
  private boolean die;
  private Thread my_thread;
  private EventHandler myHandler;
  
  public EventDispatcher(EventHandler inHandler) {
    setName("EventDispatcher thread");
    myHandler=inHandler;
    die=false;
	event_queue=new LinkedBlockingQueue<AbstractDecoder>(); // does not need to be fair
  }

  public void Die() {
	  die=true;
	  my_thread.interrupt(); // trigger interrupt in case of waiting
  }
  
  public void run() {
	my_thread=Thread.currentThread();
	Log.d(TAG,"Running! Threadid="+my_thread.getId());
	AbstractDecoder event;
    while(!die) {
    	event=null;
		try {
			event=event_queue.take();
		} catch (InterruptedException e) {
			if(die)
			{
				Log.d(TAG,"EventDispatcher eventQueue.take() interrupted and die=true.");
				break;
			}
			else
			{
				Log.d(TAG,"EventDispatcher eventQueue.take() interrupted but die=false. Ignoring.");
			}
		}
		if(event==null) {}
		else if(event instanceof Event)
		{
			int eid=((Event)event).getEventId();
			Log.d(TAG,"Dispatching an event id="+eid);
			try
			{
				myHandler.HandleEvent((Event)event);
			}
			catch(Exception e)
			{
				Log.e(TAG,"Skype.HandleEvent threw unhandled exception. Event (id="+eid+") probably lost. Stack trace follows.");
				e.printStackTrace();
			}
			Log.d(TAG,"Dispatched an event id="+eid);
		}
		else if(event instanceof PropertyChange)
		{
			int prid=((PropertyChange)event).propid;
			Log.d(TAG,"Dispatching a propertychange propid="+prid);
			try
			{
				myHandler.HandlePropertyChange((PropertyChange)event);
			}
			catch(Exception e)
			{
				Log.e(TAG,"Skype.HandlePropertyChange threw unhandled exception. Property change (id="+prid+") probably lost. Stack trace follows."); e.printStackTrace();
			}

			Log.d(TAG,"Dispatched a propertychange propid="+prid);
		}
		else
		{
			Log.e(TAG,"Event queue entry is of unknown type! - discarding");
		}
    }
    Log.d(TAG,"Exiting!");
  }

  	// These are called from response listener
	public void AddPropertyChange(PropertyChange c) throws InterruptedException
	{
		Log.d(TAG,"Adding PropertyChange (propid="+c.propid+") to eventQueue. eQ size="+event_queue.size());
		event_queue.put(c);
		Log.d(TAG,"Added PropertyChange (propid="+c.propid+") to eventQueue. eQ size="+event_queue.size());
	}
	
	public void AddEvent(Event e) throws InterruptedException
	{
		Log.d(TAG,"Adding Event (event_id="+e.getEventId()+") to eventQueue. eQ size="+event_queue.size());
		event_queue.put(e);
		Log.d(TAG,"Added Event (event_id="+e.getEventId()+") to eventQueue. eQ size="+event_queue.size());
	}
}

