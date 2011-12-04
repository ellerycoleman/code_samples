/*----------------------------------------------------------------------------
# File:		Account.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Account class for cscie160, hw6.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw6;


/**
 * Implements a bank account.
 */

public class Account
{


    //----------------
    // 1 Data Member
    //----------------
    private Float balance;






    //---------------
    // Constructor
    //---------------

   /**
    * A default constructor that sets the account balance to 0.
    */
    public Account()
    {   balance= 0.0F;
    }





    //-----------------
    // Method Members
    //-----------------

   /**
    * Returns the account balance.
    */
    public synchronized Float getBalance()
    {   return balance;
    }


   /**
    * Deposits the specified amount into the account.
    */
    public synchronized void deposit(Float amt)
    {   balance+= amt;
    }


   /**
    * Withdraws the specified amount from the account.
    */
    public synchronized void withdraw(Float amt)
    {   balance-= amt;
    }
}
