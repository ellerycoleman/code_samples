/*----------------------------------------------------------------------------
# File:		ATMImpl.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the ATM interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.io.Serializable;
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;
import java.net.MalformedURLException;


/**
 * Implements the ATM interface and provides a single bank account.
 */

public class ATMImpl extends Observable implements ATM
{
    //-----------------------------------------------------
    // Internal class to support TransactionNotifications
    //-----------------------------------------------------
    private class WrappedObserver implements Observer, Serializable
    {
        private static final long serialVersionUID = 1L;
        private ATMListener tn = null;
        public WrappedObserver(ATMListener tn) {
            this.tn = tn;
        }

        public void update(Observable o, Object arg) 
        {
            try {
                tn.update(o.toString(), arg);
            } catch (RemoteException e) {
                System.out
                        .println("Remote exception removing observer:" + this);
                o.deleteObserver(this);
            }
        }
    }





    //----------------------------------
    //        3 Data Members
    //----------------------------------
    private static Bank bank;
    private static Security securityService;
    private float cashOnHand;




    //----------------------------------
    //          Constructor
    //----------------------------------
   /**
    * Default constructor to connect to the remote Bank and Security service.
    */
    public ATMImpl() throws RemoteException
    {   

        // Initialze cashOnHand to $500
	//-----------------------------
	cashOnHand= 500F;


    
        // Connect to the remote Bank
        //----------------------------
        try 
        {
            bank = (Bank) Naming.lookup("//localhost/bank");
	    securityService= (Security) Naming.lookup("//localhost/security");
        } catch (MalformedURLException mue) {
           mue.printStackTrace();
        } catch (NotBoundException nbe) {
           nbe.printStackTrace();
        } catch (UnknownHostException uhe) {
           uhe.printStackTrace();
        } catch (RemoteException re) {
           re.printStackTrace();
        }


	// Export the remote object to make it available to receive
	// incoming calls on any port.
	//----------------------------------------------------------
	UnicastRemoteObject.exportObject(this,0);
    }





    //----------------------------------
    //        Method members
    //----------------------------------
    public void addObserver(ATMListener o) throws RemoteException {
        WrappedObserver mo = new WrappedObserver(o);
        addObserver(mo);
        System.out.println("Registered ATMListener:" + mo);
    }




   /**
    * Authenticates the specified account.
    */
    public void authenticate(AccountInfo account) throws SecurityException, RemoteException
    {   
        // Authenticate the specified account
	//------------------------------------
	if(securityService.validAuth(account) == false)
	{   throw new SecurityException("Invalid PIN number.");
	}
    }




   /**
    * Deposits the specified amount into the account.
    */
    public void deposit(AccountInfo account, float amount) throws SecurityException, RemoteException
    {   
        // Transaction Description
	//-------------------------
        String txDesc= "ATMImpl.deposit() for $" + amount + " has been invoked on account #" + 
	                account.getAccountNumber(); 


        // Display pending transaction to STDOUT on server
	//-------------------------------------------------
	System.out.println(txDesc);


        // Notify all ATMListeners of the pending transactcion
	//-----------------------------------------------------
	TransactionNotification tn= new TransactionNotification("[ATMListener]: " + txDesc);
	setChanged();
	notifyObservers(tn);


        // Authenticate the specified account
	//------------------------------------
	authenticate(account);


        // Verify that user is authorized to make a deposit
	//--------------------------------------------------
	if(securityService.depositAllowed(account) == false)
	{   throw new SecurityException("User is not authorized to perform a deposit.");
	}
    

        // perform deposit
	//-----------------
        Account acct= bank.getAccount(account.getAccountNumber());
	acct.deposit(amount);
    }



   /**
    * Withdraws the specified amount from the account.
    */
    public void withdraw(AccountInfo account, float amount) throws ATMException, NSFException, RemoteException, SecurityException
    {   
        // Transaction Description
	//-------------------------
	String txDesc= "ATMImpl.withdraw() for $" + amount + 
	               " has been invoked for account #" + 
	                account.getAccountNumber();


        // Display pending transaction to STDOUT on server
	//-------------------------------------------------
	System.out.println(txDesc);



        // Notify all ATMListeners of the pending transactcion
	//-----------------------------------------------------
	TransactionNotification tn= new TransactionNotification("[ATMListener]: " + txDesc);
	setChanged();
	notifyObservers(tn);



        // Authenticate the specified account
	//------------------------------------
	authenticate(account);



        // Verify that user is authorized to withdraw funds
	//--------------------------------------------------
	if(securityService.withdrawAllowed(account) == false)
	{   throw new SecurityException("User is not authorized to withdraw funds.");
	}
    


	// Verify that the ATM has sufficient funds...
	//---------------------------------------------
	if(amount > cashOnHand)
	{   throw new ATMException("ATM is not able to fund this transaction.\n" +
	                           "ATM cashOnHand: $" + cashOnHand);
	}


        // Perform withdrawal
	//--------------------
        Account acct= bank.getAccount(account.getAccountNumber());
	acct.withdraw(amount);


	// Update cashOnHand
	//--------------------
	cashOnHand-= amount;

    }






   /**
    * Returns the balance of the specified account.
    */
    public Float getBalance(AccountInfo account) throws SecurityException, RemoteException
    {   
        // Transaction Description
	//-------------------------
	String txDesc= "ATMImpl.getBalance() has been invoked for account #" +
	                account.getAccountNumber();


        // Display pending transaction to STDOUT on server
	//-------------------------------------------------
	System.out.println(txDesc);



        // Notify all ATMListeners of the pending transactcion
	//-----------------------------------------------------
	TransactionNotification tn= new TransactionNotification("[ATMListener]: " + txDesc);
	setChanged();
	notifyObservers(tn);



        // Authenticate the specified account
	//------------------------------------
	authenticate(account);



        // Verify that user is authorized to get balance
	//-----------------------------------------------
	if(securityService.balanceAllowed(account) == false)
	{   throw new SecurityException("User is not authorized to view account balance.");
	}


        // return balance
	//----------------
        Account acct= bank.getAccount(account.getAccountNumber());
	if(acct == null)
	{   System.out.println("Account #" + account + " is null!\n");
	}
        return acct.getBalance();
    }





   /**
    * Transfers the specified amount from one account to another.
    */
    public void transfer(AccountInfo fromAccount, AccountInfo toAccount, float amount) throws SecurityException, NSFException, RemoteException
    {   
        // Transaction Description
	//-------------------------
        String txDesc= "ATMImpl.transfer() has been invoked: $" + amount +
                        " from account #" + fromAccount.getAccountNumber() + " to account " +
			 toAccount.getAccountNumber();



        // Display pending transaction to STDOUT on server
	//-------------------------------------------------
	System.out.println(txDesc);



        // Notify all ATMListeners of the pending transactcion
	//-----------------------------------------------------
	TransactionNotification tn= new TransactionNotification("[ATMListener]: " + txDesc);
	setChanged();
	notifyObservers(tn);



        // Authenticate both accounts
	//----------------------------
	authenticate(fromAccount);
	authenticate(toAccount);



        // Verify that fromAccount is authorized for a withdrawal
	//--------------------------------------------------------
	if(securityService.withdrawAllowed(fromAccount) == false)
	{   throw new SecurityException("User is not authorized to withdraw funds from " +
	                                "account #" + fromAccount.getAccountNumber() + ".");
	}



        // Verify that toAccount is authorized for a deposit
	//---------------------------------------------------
	if(securityService.depositAllowed(toAccount) == false)
	{   throw new SecurityException("User is not authorized to deposit funds into " +
	                                "account #" + toAccount.getAccountNumber() + ".");
	}



	// withdrawing the funds from the fromAccount and
	// depositing them into the toAccount.
	//-------------------------------------------------
        Account fromAcct= bank.getAccount(fromAccount.getAccountNumber());
	Account toAcct=   bank.getAccount(toAccount.getAccountNumber());
	fromAcct.withdraw(amount);
	toAcct.deposit(amount);

    }
}


