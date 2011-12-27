/*----------------------------------------------------------------------------
# File:		Account.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Account class for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id: Account.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.util.*;

/**
 * Implements a bank account.
 */

public class Account
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
    public Account()
    {   balance= 0.0F;
        accountNumber= numberGenerator++;
	System.out.print("Account #" + accountNumber + " was created with balance of $" + balance + ".\n");
    }



   /**
    * A constructor that initializes bank account balance to the specified amount.
    */
    public Account(Float f)
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
    public Float getBalance()
    {   return balance;
    }


   /**
    * Deposits money into the account.
    */
    public void deposit(Float f)
    {   balance+= f;
        System.out.println("Deposit to account #" + accountNumber + ",  new balance: $" + balance + "\n");
    }



   /**
    * Withdraws money from the account.
    */
    public void withdraw(Float f)
    {   balance-= f;
        System.out.println("Withdrawal from account #" + accountNumber + ",  new balance: $" + balance + "\n");
    }



   /**
    * Sets the account balance according to the parameter.
    */
    public void setBalance(Float b)
    {   balance= b;
    }


   /**
    * Returns the account number.
    */
    public int getAccountNumber()
    {   return accountNumber;
    }



   /**
    * Test harness for Account.
    */
    public static void main(String args[])
    {
        // create a bank; a collection of bank accounts.
        Map<Integer, Account> bank;
        bank= new HashMap<Integer, Account>();



	// add bank accounts with specified balances.
        bank.put(numberGenerator,new Account());      // account 1 with balance of 0.00.
	bank.put(numberGenerator,new Account(100F));  // account 2, balance of 100.00.
	bank.put(numberGenerator,new Account(500F));  // account 3, balance of 500.00.


	// deposit to an account.
	int acctNum= 0000001;
        Account a= bank.get(acctNum);
	a.deposit(75F);
	System.out.println("Account num " + acctNum + " has balance of $" + a.getBalance());


        // withdraw money from an account
	acctNum= 0000002;
	a= bank.get(acctNum);
	a.withdraw(75F);
	System.out.println("Account num " + acctNum + " has balance of $" + a.getBalance());



    }
}
