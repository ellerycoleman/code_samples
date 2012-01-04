/*----------------------------------------------------------------------------
# File:		ATMImpl.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the ATM interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
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


    //------------------
    // 3 Data Members
    //------------------
    private static Bank bank;
    private static Security securityService;
    private float cashOnHand;




    //-------------
    // Constructor
    //-------------


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





    //-----------------
    // Method members
    //-----------------
    public void addObserver(ATMListener o) throws RemoteException {
        WrappedObserver mo = new WrappedObserver(o);
        addObserver(mo);
        System.out.println("Added observer:" + mo);
    }




   /**
    * Authenticates the specified account.
    */
    public void authenticate(AccountInfo account) throws ATMException, RemoteException
    {   
        // Authenticate the specified account
	//------------------------------------
	if(securityService.validAuth(account) == false)
	{   throw new ATMException("Invalid PIN number.");
	}
    }




   /**
    * Deposits the specified amount into the account.
    */
    public void deposit(AccountInfo account, float amount) throws ATMException, RemoteException
    {   
        // Transaction Description
	//-------------------------
        String txDesc= "ATMImpl.deposit() has been invoked on account #" + 
	                account.getAccountNumber(); 


        // Display pending transaction to STDOUT on server
	//-------------------------------------------------
	System.out.println(txDesc);


        // Notify all ATMListeners of the pending transactcion
	//-----------------------------------------------------
	TransactionNotification tn= new TransactionNotification("ATMListener: " + txDesc);
	setChanged();
	notifyObservers(tn);


        // Authenticate the specified account
	//------------------------------------
	authenticate(account);


        // Verify that user is authorized to get balance
	//-----------------------------------------------
	if(securityService.depositAllowed(account) == false)
	{   throw new ATMException("User is not authorized to perform a deposit.");
	}
    

        // perform deposit
	//-----------------
        Account acct= bank.getAccount(account.getAccountNumber());
	acct.deposit(amount);
    }



   /**
    * Withdraws the specified amount from the account.
    */
    public void withdraw(AccountInfo account, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.withdraw() has been invoked for account #" + account + ".\n");

        // Authenticate the specified account
	//------------------------------------
	authenticate(account);


        // Verify that user is authorized to get balance
	//-----------------------------------------------
	if(securityService.withdrawAllowed(account) == false)
	{   throw new ATMException("User is not authorized to withdraw funds.");
	}
    


        // Verify that the account has sufficient funds...
	//-------------------------------------------------
        Account acct= bank.getAccount(account.getAccountNumber());
	float currBalance= acct.getBalance();
	if(currBalance < amount)
	{   throw new ATMException("Insufficient Funds.");
	}


	// Verify that the ATM has sufficient funds...
	//---------------------------------------------
	if(amount > cashOnHand)
	{   throw new ATMException("ATM is not able to fund this transaction.\n" +
	                           "ATM cashOnHand: $" + cashOnHand);
	}


        // Perform withdrawal
	//--------------------
	acct.withdraw(amount);



	// Update cashOnHand
	//--------------------
	cashOnHand-= amount;

    }






   /**
    * Withdraws the specified amount from the account for the purpose of a transfer;
    * does not verify or update the ATM's cashOnHand.
    */
    public void withdrawForTransfer(AccountInfo account, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.withdrawForTransfer() has been invoked for account #" + account + ".\n");

        // Authenticate the specified account
	//------------------------------------
	authenticate(account);


        // Verify that user is authorized to get balance
	//-----------------------------------------------
	if(securityService.withdrawAllowed(account) == false)
	{   throw new ATMException("User is not authorized to withdraw funds.");
	}
    

        // Verify that the account has sufficient funds...
	//-------------------------------------------------
        Account acct= bank.getAccount(account.getAccountNumber());
	float currBalance= acct.getBalance();
	if(currBalance < amount)
	{   throw new ATMException("Insufficient Funds.");
	}


        // Perform withdrawal
	//--------------------
	acct.withdraw(amount);


    }








   /**
    * Returns the balance of the specified account.
    */
    public Float getBalance(AccountInfo account) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.getBalance() has been invoked for account #" + account.getAccountNumber() + ".\n");

        // Authenticate the specified account
	//------------------------------------
	authenticate(account);


        // Verify that user is authorized to get balance
	//-----------------------------------------------
	if(securityService.balanceAllowed(account) == false)
	{   throw new ATMException("User is not authorized to view account balance.");
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
    public void transfer(AccountInfo fromAccount, AccountInfo toAccount, float amount) throws ATMException, RemoteException
    {   System.out.print("ATMImpl.transfer() has been invoked: $" + amount +
                         " from account #" + fromAccount.getAccountNumber() + " to account " +
			 toAccount.getAccountNumber() + "\n"
		        );


        // Authenticate both accounts
	//----------------------------
	authenticate(fromAccount);
	authenticate(toAccount);


        // Verify that fromAccount is authorized for a withdrawal
	//--------------------------------------------------------
	if(securityService.withdrawAllowed(fromAccount) == false)
	{   throw new ATMException("User is not authorized to withdraw funds from " +
	                           "account #" + fromAccount.getAccountNumber() + ".");
	}


        // Verify that toAccount is authorized for a deposit
	//---------------------------------------------------
	if(securityService.depositAllowed(toAccount) == false)
	{   throw new ATMException("User is not authorized to deposit funds into " +
	                           "account #" + toAccount.getAccountNumber() + ".");
	}




        // verify that there is enough money in the fromAccount
	// to cover the transfer transaction.
	//------------------------------------------------------
        Account fromAcct= bank.getAccount(fromAccount.getAccountNumber());
	float fromAcctBalance= fromAcct.getBalance();
        if(fromAcctBalance < amount)
	{   throw new ATMException("Insuffient Funds for Transfer; Tranfer Cancelled.");
	}



	// withdrawing the funds from the fromAccount and
	// depositing them into the toAccount.
	//-------------------------------------------------
	Account toAcct=   bank.getAccount(toAccount.getAccountNumber());
	fromAcct.withdraw(amount);
	toAcct.deposit(amount);

    }
}


