/*----------------------------------------------------------------------------
# File:		SecurityImpl.java
# Date:		Mon Jan  2 12:00:37 GMT 2012
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the Security interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;


/**
 * Implements the Security interface - it also creates 3 securityProfile
 * objects for the 3 test accounts.
 */

public class SecurityImpl extends UnicastRemoteObject implements Security
{

    //------------------
    // 1 Data Member
    //------------------
    private static Map<Integer, securityProfile> securityProfiles;

    

    //-------------
    // Constructor
    //-------------

   /**
    * Default constructor to initialize the bank with 3 accounts.
    */
    public SecurityImpl() throws RemoteException
    {   
        // Initialize security profile Map
	//---------------------------------
        securityProfiles= new HashMap<Integer, securityProfile>();




	//--------------------------------------------------------
	// add security profiles according to final project spec.
	//--------------------------------------------------------
	securityProfile profile1= new securityProfile(1,1234);
	securityProfile profile2= new securityProfile(2,2345);
	securityProfile profile3= new securityProfile(3,3456);


	profile2.denyWithdraw();
	profile3.denyDeposit();

	addSecurityProfile(1,profile1);
	addSecurityProfile(2,profile2);
	addSecurityProfile(3,profile3);

    }



    //----------------
    // Method Members
    //----------------


   /**
    * Adds the specified securityProfile to the Security system and maps it to
    * the specified account number.
    */
    public void addSecurityProfile(int acctNum, securityProfile profile)
    {   securityProfiles.put(acctNum,profile);
    }



   /**
    *  Authenticates the user based on the info provided in AccountInfo parameter.
    */
    public boolean validAuth(AccountInfo a) throws RemoteException
    {   int acctNum= a.getAccountNumber();
        int pinNum=  a.getPinNumber();

	securityProfile currProfile= securityProfiles.get(acctNum);
	return (currProfile.getPinNumber() == pinNum);
    }


   /**
    *  Uses info provided in AccountInfo parameter to determine whether the
    *  specified user is authorized to deposit.
    */
    public boolean depositAllowed(AccountInfo a) throws RemoteException
    {   int acctNum= a.getAccountNumber();
        int pinNum=  a.getPinNumber();

	securityProfile currProfile= securityProfiles.get(acctNum);
	return (currProfile.depositAllowed());
    }



   /**
    *  Uses info provided in AccountInfo parameter to determine whether the
    *  specified user is authorized to withdraw.
    */
    public boolean withdrawAllowed(AccountInfo a) throws RemoteException
    {   int acctNum= a.getAccountNumber();
        int pinNum=  a.getPinNumber();

	securityProfile currProfile= securityProfiles.get(acctNum);
	return (currProfile.withdrawAllowed());
    }



   /**
    *  Uses info provided in AccountInfo parameter to determine whether the
    *  specified user is authorized to query the account balance.
    */
    public boolean balanceAllowed(AccountInfo a) throws RemoteException
    {   int acctNum= a.getAccountNumber();
        int pinNum=  a.getPinNumber();

	securityProfile currProfile= securityProfiles.get(acctNum);
	return (currProfile.balanceAllowed());
    }




    

   /**
    *  Provides a simple test harness for this class.
    */
    public static void main(String args[])
    {
        try
	{   SecurityImpl secmgr= new SecurityImpl();
	}
	catch (RemoteException e)
	{   System.out.println(e.toString());
	}

	System.out.println("Test harness is running...");
    }

}

