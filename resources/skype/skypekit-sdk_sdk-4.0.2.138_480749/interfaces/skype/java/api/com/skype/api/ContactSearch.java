package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * This class encapsulates functionality for looking up contacts on the Skype network. Contacts can be searched by portion of their name, e-mail address, language preferences, etc. <br><br>Contact search is asynchronous. ContactSearch.Submit is a non-blocking function that initiates the search. Upon finding a matching contact, ContactSearch.OnNewResult event gets fired, that gives you the reference to the discovered contact. You can get up to 100 matching contacts per search. Note that you will need to keep a live reference of the ContactSearch object while the search is doing its work.  <br><br>So, to perform a contact search:  <br> - create a contact search object <br> - specify search terms and conditions <br> - submit search <br> - in ContactSearch.OnNewResult callback, update your UI <br> - in ContactSearch.OnChange, check for terminal values of P_CONTACT_SEARCH_STATUS and update the UI accordingly. <br><br>When the search has done its job, the ContactSearch.P_CONTACT_SEARCH_STATUS property will go to one of the terminal values. <br><br>The terminal values are: <br> - FINISHED - the search has stopped. Note that this does not mean any matches were actually found. <br> - FAILED - the search has failed. <br> - EXTENDABLE - this state should be considered the same as FINISHED. The feature of extending long search results is about to be deprecated. It is still possible for search objects to occasionally reach that state, so it should be handled in the UI (as FINISHED), but the extending feature itself should not be implemented in your UI. <br><br>There are three methods to create the ContactSearch objects. <br><br>A) Skype.CreateIdentitySearch <br><br>This method takes a string argument and looks for exact matches against Contact.P_SKYPENAME property. So for example, identity search for "echo" will return 0 results and search for "echo123" will return exactly one.  <br><br>Identity in this case means skypename - contact search does not work with PSTN type contacts. However, it does work for SKYPE type contacts that have supplied P_PHONE_HOME, P_PHONE_OFFICE or P_PHONE_MOBILE values in their account data. To search for those, you will need to use complex search (see below). <br><br>Note that you should always check for boolean return value of the CreateIdentitySearch method. If the user submits a string that is not a valid skypename, the method will return false and the ContactSearchRef argument will return as NULL. <br><br>B) Skype.CreateBasicContactSearch <br><br>This method takes a string argument and looks for non-exact matches against both P_SKYPENAME and P_FULLNAME properties of the contact. If you intend to implement a simple, one-input search feature - this is the best method for you. The non-exact matching operates similarly to the SQL LIKE condition. <br><br>C) Skype.CreateContactSearch <br><br>This method enables you to implement advanced contact search, matching against multiple seach criteria. It takes no input arguments and expects search criteria to be added to the already constructed search object. <br><br>Criteria can be added with ContactSearch.AddStrTerm and ContactSearch.AddIntTerm methods. <br><br>These methods take Contact class porperty ID, condition, and the match pattern as inputs. <br><br>Only the following Contact properties can be used for search: <br> - P_SKYPENAME  <br> - P_FULLNAME <br> - P_BIRTHDAY (uint) <br> - P_GENDER (uint: 1-male, 2-female) <br> - P_LANGUAGES <br> - P_COUNTRY <br> - P_PROVINCE <br> - P_CITY <br> - P_PHONE_HOME <br> - P_PHONE_OFFICE <br> - P_PHONE_MOBILE <br> - P_EMAILS <br> - P_HOMEPAGE <br> - P_ABOUT <br><br>String searches are case insensitive, i.e. search for echo123 also matches ECHO123 <br><br>When adding multiple criteria, default behaviour is that the criterions are additive. I.e. a term skypename == "joe" followed by term country == "us" will result in intersection between all joes and everybody in US. <br><br>You can explicitly add an "OR" instead of "AND" between conditions, using the AddOr method. <br><br>By default, AND criteria are grouped together, before OR's, so that: <br><br>AddTerm(condition1) <br>AddTerm(condition2) <br>AddOr() <br>AddTerm(condition3) <br>AddTerm(condition4) <br><br>will result in the following logical statement: <br>(condition1 AND condition2) OR (condition3 AND condition4) <br><br>However, you can add "global" critera, by using the add_to_subs argument of the AddXX methods. <br><br>AddTerm(condition1) <br>AddTerm(condition2) <br>AddOr() <br>AddTerm(condition3) <br>AddTerm(condition4, add_to_subs=true) <br><br>which would result in: <br>(condition1 AND condition2 AND condition4) OR (condition3 AND condition4) <br><br><br>Every one of the contact properties can only be used once, per search. For example, you cannot create a search for two different P_FULLNAME patterns. The &valid argument will still return tue if you do this, but the last criteria for any given property will override all previous ones. So, a search like this: <br><br>cs->AddStrTerm(Contact.P_FULLNAME, ContactSearch.EQ, "John Smith", isValid); <br>cs->AddOr(); <br>cs->AddStrTerm(Contact.P_FULLNAME, ContactSearch.EQ, "Ivan Sidorov", isValid); <br><br>will only return matches for "Ivan Sidorov" and none for "John Smith". <br><br>Some of the contact properties are automatically combined for purposes of search. <br><br>A search for P_SKYPENAME also returns matches from the P_FULLNAME property and vice versa. <br><br>So that this: <br>cs->AddStrTerm(Contact.P_SKYPENAME, ContactSearch.EQ, "john.smith", isValid); <br><br>..and this: <br>cs->AddStrTerm(Contact.P_FULLNAME, ContactSearch.EQ, "John Smith", isValid); <br><br>..and this: <br>cs->AddStrTerm(Contact.P_SKYPENAME, ContactSearch.EQ, "john.smith", isValid); <br>cs->AddOr(); <br>cs->AddStrTerm(Contact.P_FULLNAME, ContactSearch.EQ, "John Smith", isValid); <br><br>..all search from both the P_FULLNAME and P_SKYPENAME fields. <br><br><br>Before using ContactGroup.Submit to start the search, you should always check whether the search criteria ended up being valid. This you can do with ContactSearch.IsValid method. <br><br>As you probably noticed, each of the AddXX methods also return a validity check boolean. However, it is a better practice to do the overall check as well, even if all the individual search criteria ended up looking Ok. <br><br>For example, lets take a search for contact's e-mail. This can be done with two different methods. Firstly we can use the ContactSearch.AddEmailTerm method. This method will actually validate whether the input is a valid e-mail address: <br><br>cs->AddEmailTerm ("test@test@test", isValid);  <br>will return the isValid argument as false. <br><br>However, you can also add the e-mail search criterion as a simple string, like this: <br><br>cs->AddStrTerm(Contact.P_EMAILS, ContactSearch.EQ, "test@test@test@", isValid); <br>in which case the isValid will return true. <br><br>However, if you then check entire search object with:  <br><br>cs->IsValid(isValid); <br><br>the isValid will correctly return false. <br>
 */


public class ContactSearch extends SkypeObject {


	public interface ContactSearchListener {
		/** This event gets called when there are changes to ContactSearch properties defined in ContactSearch.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
		/**This callback is fired when a new matching contact has been found during the search. <br>*/
		public void OnNewResult(SkypeObject obj, Contact contact, int rankValue);
		
	}
	
	public ContactSearch(int oid, Skype skype) {
		super(oid,skype);
	}
	
	private static final int MODULE_ID = 1;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the ContactSearch class */
	public enum PROPERTY {
	
		/** type: STATUS */
		contact_search_status(200);
		
		private static final Map<Integer,PROPERTY> lookup = new HashMap<Integer,PROPERTY>();
		
		static {
			for(PROPERTY s : EnumSet.allOf(PROPERTY.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private PROPERTY(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static PROPERTY get(int code) {
			return lookup.get(code);
		}
		
		public static PROPERTY fromString(String s) {
			for (PROPERTY p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	public Object GetPropertyAsEnum(int propid) {
		return PROPERTY.get(propid);
	}
	
	public String GetStrProperty(PROPERTY prop) {
		//check in propcache if so then return
		if (mPropCache.containsKey(new Integer(prop.id))){
			String value =  (String)mPropCache.get(prop.id);
			if (value != null && !(value.length() == 0) ){
				return value;
			}
		}
		//else get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(1, mObjectId, prop.id);
		
		String string = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null){
			string = r.GetAsString();
		}
		
		if (string != null)
		{
			mPropCache.put(new Integer(prop.id), string);
		}
		return string;
	}
	
	public int GetIntProperty(PROPERTY prop) {
		//check in propcache if so then return
		if (mPropCache.containsKey(new Integer(prop.id))){
			int value = ((Integer)mPropCache.get(prop.id)).intValue();
			if (value != 0){
				return value;
			}
		}
		//else get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(moduleID(), mObjectId, prop.id);
		
		Integer integer = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null){
			integer  = r.GetAsInt();
		}
		
		if (integer != null)
		{
			mPropCache.put(new Integer(prop.id), integer);
			return integer.intValue();
		}
		else
		{
			return 0;
		}
	}
	
	public boolean GetBooleanProperty(PROPERTY prop) {
		//check in propcache if so then return
		if (mPropCache.containsKey(new Integer(prop.id))){
			return ((Boolean)mPropCache.get(prop.id)).booleanValue();
		}
		//else get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(moduleID(), mObjectId, prop.id);
		
		Boolean boolResp = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null){
			boolResp  = r.GetAsBoolean();
		}
		
		if (boolResp != null)
		{
			mPropCache.put(new Integer(prop.id), boolResp);
			return boolResp.booleanValue();
		}
		else
		{
			return false;
		}
	}
	
	public byte [] GetBinProperty(PROPERTY prop) {
		//get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(1, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**
	Possible values for the ContactSearch.P_STATUS property. <br> */
	public enum STATUS {
	
		/** Transient state, obtained after submission and actually initiating the search on the network. <br>*/
		CONSTRUCTION(1),
		
		/** Waiting for results to show up. This is a transient state. <br>*/
		PENDING(2),
		
		/** Enough matches are found. No more OnNewResult events will fire. The feature of extending long search results is about to be deprecated. It is still possible for search objects to occasionally reach that state, so it should be handled in the UI (as FINISHED), but the extending feature itself should not be implemented in your UI.  <br>*/
		EXTENDABLE(3),
		
		/** The search is finished. No more matches are expected. This is a terminal state. <br>*/
		FINISHED(4),
		
		/** ContactSearch failed. Better check if the search terms made any sense, with ContactSearch.IsValid. This is a terminal state. <br>*/
		FAILED(5);
		
		private static final Map<Integer,STATUS> lookup = new HashMap<Integer,STATUS>();
		
		static {
			for(STATUS s : EnumSet.allOf(STATUS.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private STATUS(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static STATUS get(int code) {
			return lookup.get(code);
		}
		
		public static STATUS fromString(String s) {
			for (STATUS p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *construct CONTACT_BIRTHDAY term based on current time
	 * @param min_age_in_years
	 * @param add_to_subs
	 * @return valid
	 */
	public boolean AddMinAgeTerm( int min_age_in_years, boolean add_to_subs) {
	
		Request request = null;
		try {
			request = new XCallRequest(1,1);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('u',1,min_age_in_years);
		request.addParm('b',2,add_to_subs);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean valid = false;
		valid = r.GetAsBoolean(1);
		return valid;
	}
	
	/**
	 *construct CONTACT_BIRTHDAY term based on current time
	 * @param max_age_in_years
	 * @param add_to_subs
	 * @return valid
	 */
	public boolean AddMaxAgeTerm( int max_age_in_years, boolean add_to_subs) {
	
		Request request = null;
		try {
			request = new XCallRequest(1,2);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('u',1,max_age_in_years);
		request.addParm('b',2,add_to_subs);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean valid = false;
		valid = r.GetAsBoolean(1);
		return valid;
	}
	
	/**
	 *Adds a search term against Contact.P_EMAILS property and pre-validates the value given in the email argument. <br>
	 * @param email e-mail addres to search for. <br>
	 * @param add_to_subs This argument enables you to group conditions. See ContactSearch class details for more information. <br>
	 * @return valid Returns false if the value in email property did not look like a valid email address. <br>
	 */
	public boolean AddEmailTerm( String email, boolean add_to_subs) {
	
		Request request = null;
		try {
			request = new XCallRequest(1,3);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('S',1,email);
		request.addParm('b',2,add_to_subs);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean valid = false;
		valid = r.GetAsBoolean(1);
		return valid;
	}
	
	/**
	 * @param language
	 * @param add_to_subs
	 * @return valid
	 */
	public boolean AddLanguageTerm( String language, boolean add_to_subs) {
	
		Request request = null;
		try {
			request = new XCallRequest(1,4);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('S',1,language);
		request.addParm('b',2,add_to_subs);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean valid = false;
		valid = r.GetAsBoolean(1);
		return valid;
	}
	
	/**
	List of available  matching conditions that can be used in AddTerm methods. <br> */
	public enum CONDITION {
	
		/** Equals <br>*/
		EQ(0),
		
		/** Is greater than <br>*/
		GT(1),
		
		/** Is greater or equal. <br>*/
		GE(2),
		
		/** Is less than <br>*/
		LT(3),
		
		/** Less or equal <br>*/
		LE(4),
		
		/** Start of a word macthes exactly (string properties only). <br>*/
		PREFIX_EQ(5),
		
		/** Start of a word is greater or equal (string properties only). <br>*/
		PREFIX_GE(6),
		
		/** Start of a word is less or equal (string properties only). <br>*/
		PREFIX_LE(7),
		
		/** Contains the word (string properties only). <br>*/
		CONTAINS_WORDS(8),
		
		/** One of the words starts with searched value (string properties only). <br>*/
		CONTAINS_WORD_PREFIXES(9);
		
		private static final Map<Integer,CONDITION> lookup = new HashMap<Integer,CONDITION>();
		
		static {
			for(CONDITION s : EnumSet.allOf(CONDITION.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private CONDITION(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static CONDITION get(int code) {
			return lookup.get(code);
		}
		
		public static CONDITION fromString(String s) {
			for (CONDITION p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *Adds a string search term to a custom contact search object.  <br>
	 * @param prop Following Contact class string propkeys can be used for Contact search: <br> - Contact.P_SKYPENAME <br> - Contact.P_FULLNAME <br> - Contact.P_LANGUAGES <br> - Contact.P_COUNTRY <br> - Contact.P_PROVINCE <br> - Contact.P_CITY <br> - Contact.P_PHONE_HOME <br> - Contact.P_PHONE_OFFICE <br> - Contact.P_PHONE_MOBILE <br> - Contact.P_EMAILS <br> - Contact.P_HOMEPAGE <br> - Contact.P_ABOUT <br>Note that while Contact.P_EMAILS is technically a string and can be used in this method, it is recommended that you use ContactSearch.AddEmailTerm method instead. <br>
	 * @param cond Search condition (ContactSearch.CONDITION) <br>
	 * @param value Value to match against. <br>
	 * @param add_to_subs This argument enables you to group conditions. See ContactSearch class details for more information. <br>
	 * @return valid Returns true if the ContactSearch term-set remains valid after adding this term. <br>
	 */
	public boolean AddStrTerm( int prop, CONDITION cond, String value, boolean add_to_subs) {
	
		Request request = null;
		try {
			request = new XCallRequest(1,5);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('e',1,prop);
		request.addParm('e',2,cond.getId());
		request.addParm('S',3,value);
		request.addParm('b',4,add_to_subs);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean valid = false;
		valid = r.GetAsBoolean(1);
		return valid;
	}
	
	/**
	 *Adds a search term to a custom contact search object. For now, there are only two searchable Contact properties that are integers, so this can oly be used for Contact.P_BIRTHDAY and Contact.P_GENDER. <br>
	 * @param prop Propkey to search for. Either Contact.P_BIRTHDAY or Contact.P_GENDER <br>
	 * @param cond Search condition (ContactSearch.CONDITION) <br>
	 * @param value Value to match against. <br>
	 * @param add_to_subs This argument enables you to group conditions. See ContactSearch class details for more information. <br>
	 * @return valid Returns true if the ContactSearch term-set remains valid after adding this term. <br>
	 */
	public boolean AddIntTerm( int prop, CONDITION cond, int value, boolean add_to_subs) {
	
		Request request = null;
		try {
			request = new XCallRequest(1,6);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('e',1,prop);
		request.addParm('e',2,cond.getId());
		request.addParm('u',3,value);
		request.addParm('b',4,add_to_subs);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean valid = false;
		valid = r.GetAsBoolean(1);
		return valid;
	}
	
	/**
	 *used to group terms (AddTerm(1), AddTerm(2), Or(), AddTerm(3), AddTerm(4), etc)
	 */
	public void AddOr() {
	
		Request request = null;
		try {
			request = new XCallRequest(1,7);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *checks that terms list is non-empty and does not contain unsupported keys
	 * @return result
	 */
	public boolean IsValid() {
	
		Request request = null;
		try {
			request = new XCallRequest(1,8);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean result = false;
		result = r.GetAsBoolean(1);
		return result;
	}
	
	/**
	 *launch search
	 */
	public void Submit() {
	
		Request request = null;
		try {
			request = new XCallRequest(1,9);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *extend if search is EXTENDABLE
	 */
	public void Extend() {
	
		Request request = null;
		try {
			request = new XCallRequest(1,10);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *releases results that are not referenced from elsewhere
	 */
	public void Release() {
	
		Request request = null;
		try {
			request = new XCallRequest(1,12);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *result list is dynamically updated
	 * @param from
	 * @param count
	 * @return contacts
	 */
	public Contact [] GetResults( int from, int count) {
	
		Request request = null;
		try {
			request = new XCallRequest(1,11);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('u',1,from);
		request.addParm('u',2,count);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		Vector<Contact> contacts = new Vector<Contact>();
		while (r.HasMore(1))
		{
			int oid = 0;
			Contact contact = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) { 
				contact = (Contact)skype.factory(Contact.moduleID(), oid, skype);
			}
			contacts.add(contact);
		}
		return contacts.toArray(new Contact[contacts.size()]);
		
	}
	

}