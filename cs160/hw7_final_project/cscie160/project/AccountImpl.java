/*----------------------------------------------------------------------------
# File:		AccountImpl.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the Account interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;



/**
 * Implements the Account interface.
 */

public class AccountImpl extends UnicastRemoteObject implements Account
{


    //----------------
    // 3 Data Members
    //----------------
    private Float balance;
    private int   accountNumber;
    private static int numberGenerator= 0000001;





    //---------------
    // Constructors
    //---------------

   /**
    * A default constructor that sets the account balance to 0.
    */
    public AccountImpl() throws RemoteException
    {   balance= 0.0F;
        accountNumber= numberGenerator++;
	System.out.print("Account #" + accountNumber + " was created with balance of $" + balance + ".\n");
    }



   /**
    * A constructor that initializes bank account balance to the specified amount.
    */
    public AccountImpl(Float f) throws RemoteException
    {   balance= f;
        accountNumber= numberGenerator++;
	System.out.print("Account #" + accountNumber + " was created with balance of $" + balance + ".\n");
    }




    //-----------------
    // Method Members
    //-----------------

   /**
    * Returns the account balance.
    */
    public Float getBalance() throws RemoteException
    {   return balance;
    }


   /**
    * Deposits money into the account.
    */
    public void deposit(Float f) throws RemoteException
    {   balance+= f;
        System.out.println("Deposit to account #" + accountNumber + ",  new balance: $" + balance + "\n");
    }



   /**
    * Withdraws money from the account.
    */
    public void withdraw(Float f) throws RemoteException,NSFException
    {   if(f > balance)
        {   throw new NSFException("Insufficient Funds: account #" + accountNumber +
	                           " has $" + balance + " and withdraw request is $" + f);
        }
        balance-= f;
        System.out.println("Withdrawal from account #" + accountNumber + ",  new balance: $" + balance + "\n");
    }



   /**
    * Sets the account balance according to the parameter.
    */
    public void setBalance(Float b) throws RemoteException
    {   balance= b;
    }



   /**
    * Returns the account number.
    */
    public int getAccountNumber() throws RemoteException
    {   return accountNumber;
    }



   /**
    * Test harness for Account.
    */
    public static void main(String args[]) throws RemoteException
    {
        // create a bank; a collection of bank accounts.
        //Map<Integer, Account> bank;
        //bank= new HashMap<Integer, Account>();



	// add bank accounts with specified balances.
        //bank.put(numberGenerator,new Account());      // account 1 with balance of 0.00.
	//bank.put(numberGenerator,new Account(100F));  // account 2, balance of 100.00.
	//bank.put(numberGenerator,new Account(500F));  // account 3, balance of 500.00.


	// deposit to an account.
	//int acctNum= 0000001;
        //Account a= bank.get(acctNum);
	//a.deposit(75F);
	//System.out.println("Account num " + acctNum + " has balance of $" + a.getBalance());


        // withdraw money from an account
	//acctNum= 0000002;
	//a= bank.get(acctNum);
	//a.withdraw(75F);
	//System.out.println("Account num " + acctNum + " has balance of $" + a.getBalance());

    }
}
