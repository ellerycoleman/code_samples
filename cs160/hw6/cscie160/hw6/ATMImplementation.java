/*----------------------------------------------------------------------------
# File:		ATMImplementation.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements ATM interface for cscie160, hw6.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw6;



/**
 * Implements the ATM interface and provides a single bank account.
 */

public class ATMImplementation implements ATM
{

    //--------------
    // Data Members
    //--------------
    private Account account;




    //-------------
    // Constructor
    //-------------


   /**
    * Default constructor to initialize the bank account.
    */
    public ATMImplementation()
    {   account= new Account();
    }
    




    //-----------------
    // Method members
    //-----------------


   /**
    * Deposits the specified amount into the account.
    */
    public void deposit(float amount) throws ATMException
    {   System.out.print("ATMImplementation.deposit() has been invoked.\n");
        Float balance= account.getBalance() + amount;
	account.setBalance(balance);
    }



   /**
    * Withdraws the specified amount from the account.
    */
    public void withdraw(float amount) throws ATMException
    {   System.out.print("ATMImplementation.withdraw() has been invoked.\n");
        Float balance= account.getBalance() - amount;
	account.setBalance(balance);
    }


   /**
    * Returns the account balance.
    */
    public Float getBalance() throws ATMException
    {   System.out.print("ATMImplementation.getBalance() has been invoked.\n");
        return account.getBalance();
    }
}


