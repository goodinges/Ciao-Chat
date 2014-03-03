print('****************************************************************************');
print('SkypeKit Python Wrapper Tutorial: Account Login');
print('****************************************************************************');

# This examle demonstrates, how to:
# 1. Create a Skype instance with the Python wrapper.
# 2. Retrieve an Account class instance.
# 3. Assign a custom callback to Account class, to detect login status.
# 4. Log in with that account.

# NB! You will need to launch the SkypeKit runtime before running this example.

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
    raise SystemExit('Program requires Skype and skypekit modules')

#----------------------------------------------------------------------------------
# Taking skypename and password arguments from command-line.

if len(sys.argv) != 3:
	print('Usage: python login1.py <skypename> <password>');
	sys.exit();

accountName = sys.argv[1];
accountPsw  = sys.argv[2];
loggedIn	= False;

#----------------------------------------------------------------------------------
# Creating our main Skype object

try:
	MySkype = Skype.GetSkype(keypair.keyFileName);
except Exception:
	raise SystemExit('Unable to create Skype instance')

#----------------------------------------------------------------------------------
# Defining our own Account property change callback and assigning it to the
# SkyLib.Account class.

def AccountOnChange (self, property_name):
	global loggedIn;
	if property_name == 'status':
		print ('Login sequence: ' + self.status);
		if self.status == 'LOGGED_IN':
			loggedIn = True;
		if self.status == 'LOGGED_OUT':
			loggedIn = False;

Skype.Account.OnPropertyChange = AccountOnChange;

#----------------------------------------------------------------------------------
# Retrieving account and logging in with it.

account = MySkype.GetAccount(accountName);

print('Logging in with ' + accountName);
account.LoginWithPassword(accountPsw, False, False);

while not loggedIn:
	sleep(1);
	
print('You should now be able to see this account as online from other Skype instances.');
raw_input('Press ENTER to log out');
print('Exiting..');
MySkype.stop();
