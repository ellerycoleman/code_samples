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
import java.net.MalformedURLException;


/**
 * Implements the ATM interface and provides a single bank account.
 */

public class ATMImpl extends UnicastRemoteObject implements ATM
{

    //------------------
    // 1 Data Members
    //------------------
    private static Bank bank;




    //-------------
    // Constructor
    //-------------


   /**
    * Default constructor to connect to the remote Bank server.
    */
    public ATMImpl() throws RemoteException
    {   
    
        // Connect to the remote Bank
        //----------------------------
        try 
        {
            bank = (Bank) Naming.lookup("//localhost/bank");
        } catch (MalformedURLException mue) {
           mue.printStackTrace();
        } catch (NotBoundException nbe) {
           nbe.printStackTrace();
        } catch (UnknownHostException uhe) {
           uhe.printStackTrace();
        } catch (RemoteException re) {
           re.printStackTrace();
        }
    }





    //-----------------
    // Method members
    //-----------------


   /**
    * Deposits the specified amount into the account.
    */
    public void deposit(int account, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.deposit() has been invoked on account #" + account + ".\n");
        Account acct= bank.getAccount(account);
	acct.deposit(amount);
    }



   /**
    * Withdraws the specified amount from the account.
    */
    public void withdraw(int account, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.withdraw() has been invoked for account #" + account + ".\n");
        Account acct= bank.getAccount(account);
	acct.withdraw(amount);
    }



   /**
    * Returns the balance of the specified account.
    */
    public Float getBalance(int account) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.getBalance() has been invoked for account #" + account + ".\n");
        Account acct= bank.getAccount(account);
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
        Account fromAcct= bank.getAccount(fromAccount);
	Account toAcct=   bank.getAccount(toAccount);



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


