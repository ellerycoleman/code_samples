/*----------------------------------------------------------------------------
# File:		ATMImpl.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the ATM interface for cscie160, final project.
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

public class ATMImpl extends UnicastRemoteObject implements ATM
{

    //------------------
    // 2 Data Members
    //------------------
    private static Map<Integer, Account> bank;
    private static int numberGenerator;




    //-------------
    // Constructor
    //-------------


   /**
    * Default constructor to initialize the bank with 3 accounts.
    */
    public ATMImpl() throws RemoteException
    {   // Initialize numberGenerator and bank
        numberGenerator= 0000001;
        bank= new HashMap<Integer, Account>();


        // add bank accounts with specified balances.
        bank.put(numberGenerator++,new Account());      // account 1, balance of   0.00.
        bank.put(numberGenerator++,new Account(100F));  // account 2, balance of 100.00.
        bank.put(numberGenerator++,new Account(500F));  // account 3, balance of 500.00.
    }





    //-----------------
    // Method members
    //-----------------


   /**
    * Deposits the specified amount into the account.
    */
    public void deposit(int account, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.deposit() has been invoked on account #" + account + ".\n");
        Account acct= bank.get(account);
	acct.deposit(amount);
    }



   /**
    * Withdraws the specified amount from the account.
    */
    public void withdraw(int account, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.withdraw() has been invoked for account #" + account + ".\n");
        Account acct= bank.get(account);
	acct.withdraw(amount);
    }



   /**
    * Returns the balance of the specified account.
    */
    public Float getBalance(int account) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.getBalance() has been invoked for account #" + account + ".\n");
        Account acct= bank.get(account);
	if(acct == null)
	{   System.out.println("Account #" + account + " is null!\n");
	}
        return acct.getBalance();
    }





   /**
    * Transfers the specified amount from one account to another.
    */
    public void transfer(int fromAccount, int toAccount, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.transfer() has been invoked: $" + amount +
                         " from account #" + fromAccount + " to account " +
			 toAccount + "\n"
		        );
        Account fromAcct= bank.get(fromAccount);
	Account toAcct=   bank.get(toAccount);



        // verify that there is enough money in the fromAccount
	// to cover this transaction
	//------------------------------------------------------
	float fromAcctBalance= fromAcct.getBalance();
        if(fromAcctBalance < amount)
	{   throw new ATMException("Insuffient Funds for Transfer; Tranfer Cancelled.");
	}



	// withdrawing the funds from the fromAccount and
	// depositing them into the toAccount.
	//-------------------------------------------------
	fromAcct.withdraw(amount);
	toAcct.deposit(amount);

    }
}


