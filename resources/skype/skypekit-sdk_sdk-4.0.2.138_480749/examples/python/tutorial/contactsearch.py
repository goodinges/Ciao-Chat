print('****************************************************************************');
print('SkypeKit Python Wrapper Tutorial: Contact search.');
print('****************************************************************************');

# This example demonstrates, how to:
# 1. Perform simple asynchronous contact search.
# 1. Perform complex asynchronous contact search.

# NB! You will need to launch the SkypeKit runtime before running this example.

# As the contact search over P2P network is somewhat slow, it cannot really
# be implemented as a blocking function. Instead, you can create a search record 
# (ContactSearch object), add multiple search filters to it and then start the search
# with ContactSearch.Submit method.
#
# Each time the search process finds a matching contact ContactSearch.OnNewResult
# event will get fired.
#
# When no more matches are found, ContactSearch.contact_search_status will change
# to 'FINISHED'.
# 
# The list of collected contacts can be compiled either in the OnNewResult callback,
# if you want your UI to respond to matches in real time. Alternatively you can 
# wait until the search finishes and then yse ContactSearch.GetResults method.

#----------------------------------------------------------------------------------
# Importing necessary libraries. Note that you will need to set the keyFileName value
# in the keypair.py file.

import sys;
import keypair;
from time import sleep;

sys.path.append(keypair.distroRoot + '/ipc/python');
sys.path.append(keypair.distroRoot + '/interfaces/skype/python');

try:
	import Skype;	
except ImportError:
	raise SystemExit('Program requires Skype and skypekit modules');

#----------------------------------------------------------------------------------
# Taking skypename and password arguments from command-line.

if len(sys.argv) < 4:
	print('Usage: python contactsearch.py <skypename> <password> <searchstring>');
	sys.exit();

accountName = sys.argv[1];
accountPsw  = sys.argv[2];
searchFor	= sys.argv[3];

loggedIn		= False;

#----------------------------------------------------------------------------------
# Defining our own Account property change callback and assigning it to the
# SkyLib.Account class.

def AccountOnChange (self, property_name):
	global loggedIn;
	if property_name == 'status':
		if self.status == 'LOGGED_IN':
			loggedIn = True;
			print('Login complete.');

Skype.Account.OnPropertyChange = AccountOnChange;


#----------------------------------------------------------------------------------
# ContactSearch class.
# ContactSearch.OnPropertyChange - to catch when the search has stopped or finished.
# ContactSearch.OnNewResult - to retrieve matching contacts in real-time, as they
# get found by the search process.

def ContactSearchOnChange (self, property_name):
	if property_name == 'contact_search_status':
		print('search status: ' + self.contact_search_status);

		# When there is a large number of matches (conceivably you could get hundreds 
		# of matches) the result list is terminated by 'page breaks'. In that case
		# the search will stop and object's contact_search_status changes to 'EXTENDABLE'
		# You can then choos to continue the search with Extend method.
		if self.contact_search_status == 'EXTENDABLE':
			print('Auto-extending the search..');
			self.Extend();
		
		if self.contact_search_status == 'FINISHED':	 	
			self.Release();
			print('Press ENTER to exit');
			
Skype.ContactSearch.OnPropertyChange = ContactSearchOnChange;

def ContactSearchOnNewResult (self, contact, rankValue):
	print(str(rankValue + 1) + '. ' + contact.skypename);

Skype.ContactSearch.OnNewResult = ContactSearchOnNewResult;
	
#----------------------------------------------------------------------------------
# Creating our main Skype object

try:
	MySkype = Skype.GetSkype(keypair.keyFileName);
except Exception:
	raise SystemExit('Unable to create Skype instance');

#----------------------------------------------------------------------------------
# Retrieving account and logging in with it. Then waiting in loop.

account = MySkype.GetAccount(accountName);

print('Logging in with ' + accountName);
account.LoginWithPassword(accountPsw, False, False);

while loggedIn == False:
	sleep(1);

#----------------------------------------------------------------------------------
# Simple contact search, just by skypename string:

SearchRec = MySkype.CreateBasicContactSearch(searchFor);

#----------------------------------------------------------------------------------
# More complex contact search, by skypename and language.
# Note that the AddStrTerm method takes proprty ID as it's first argument, rather 
# than property name string.
#
# By default, filters criteria are added (filter1 AND filter2 AND ..)
# To get multiple groups of match criteria you can insert an OR between them
# with ContactSearch.AddOr method.
#
# Complete list of comparison operators between property value and pattern:
# 'EQ' - equal
# 'GT' - greater than
# 'GE' - greater or equal
# 'LT' - less than
# 'LE' - less or equal
# 'PREFIX_EQ' - starts with equal
# 'PREFIX_GE' - starts with greater or equal
# 'PREFIX_LE' - starts with less or equal
# 'CONTAINS_WORDS' - contains words
# 'CONTAINS_WORD_PREFIXES' - contains words that start with pattern
#
# Comment the simple search example above and uncomment the three codelines below
# to try out the complex search.

#SearchRec = MySkype.CreateContactSearch();
#SearchRec.AddStrTerm(Skype.Contact.P_SKYPENAME, 'EQ', searchFor, False);
#SearchRec.AddLanguageTerm('en', False);

if SearchRec.IsValid():
	SearchRec.Submit();
else:
	print('Invalid search record.');

raw_input('');

print('Exiting..');
SearchRec.Release();
MySkype.stop();
