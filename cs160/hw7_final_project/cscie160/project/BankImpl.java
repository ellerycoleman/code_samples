/*----------------------------------------------------------------------------
# File:		BankImpl.java
# Date:		Wed Dec 28 00:24:27 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the Bank interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;


/**
 * Implements the ATM interface and provides a single bank account.
 */

public class BankImpl extends UnicastRemoteObject implements Bank
{

    //------------------
    // 2 Data Members
    //------------------
    private static Map<Integer, AccountImpl> bank;
    private static int numberGenerator;




    //-------------
    // Constructor
    //-------------


   /**
    * Default constructor to initialize the bank with 3 accounts.
    */
    public BankImpl() throws RemoteException
    {   
        // Initialize numberGenerator and bank
        numberGenerator= 1;
        bank= new HashMap<Integer, AccountImpl>();


        // add bank accounts with specified balances.
        addAccount(0F);     // account 1, balance of   0.00.
        addAccount(100F);   // account 2, balance of 100.00.
        addAccount(500F);   // account 3, balance of 500.00.
    }





    //-----------------
    // Method members
    //-----------------


   /**
    * Returns a reference to the specified account.
    */
    public Account getAccount(int account) throws RemoteException
    {   System.out.print("BankImpl.getAccount() has been invoked for account #" + account + ".\n");
        Account acct= bank.get(account);
	return acct;
    }



   /**
    * Adds an account to the bank.
    */
    public void addAccount(float balance) throws RemoteException
    {   bank.put(numberGenerator,new AccountImpl(balance));
        System.out.println("Account #" + numberGenerator + " has been added to bank.");
	++numberGenerator;
    }

}


