  package cscie160.project;
  import java.net.MalformedURLException;
  import java.rmi.Naming;
  import java.rmi.NotBoundException;
  import java.rmi.RemoteException;
  import java.rmi.UnknownHostException;
  import java.rmi.server.*;
  import javax.naming.*;
  import java.util.*;
  


 /** 
  * This client is a modified version of Prof Sawyer's client.
  */
  public class Client extends UnicastRemoteObject implements ATMListener
  {


     //-------------
     // Constructor
     //-------------
     protected Client() throws RemoteException 
     {   super();
     }



     //-----------
     // Constants
     //-----------
     private static final long serialVersionUID = 1L;




     //----------------
     // Method Members
     //----------------
     
    /**
     * Per the spec, a helper class to create AccountInfo objects.
     */
     public static AccountInfo getAccountInfo(int acctNum, int pinNum)
     {   return new AccountInfo(acctNum,pinNum);
     }


    /**
     * An update() method as required by the Observer interface.
     * Simply passes the TransactionNotification to the handleTXNotification()
     * method of the ATMListener interface.
     */
     public void  update(Object observable, Object updateMesg)
     {   handleTXNotification(observable,updateMesg);
     }


    /**
     * Per the spec, a method to handle TransactionNotification messages from the ATM.
     */
     public void  handleTXNotification(Object observable, Object updateMesg)
     {   // Per the spec, simply displays the TransactionNotification message
         // to STDOUT.
         TransactionNotification tn= (TransactionNotification) updateMesg;
	 System.out.println(tn);
     }






//==============================================================================
//  BEGIN -->  VERBATIM CLIENT CODE PROVIDED AND REQUIRED BY PROFESSOR SAWYER
//==============================================================================

   public static void testATM(ATM atm) {
      if (atm!=null) {
         printBalances(atm);
         performTestOne(atm);
         performTestTwo(atm);
         performTestThree(atm);
         performTestFour(atm);
         performTestFive(atm);
         performTestSix(atm);
         performTestSeven(atm);
         performTestEight(atm);
         performTestNine(atm);
         printBalances(atm);
      }
   }        
   public static void printBalances(ATM atm) {        
      try {
         System.out.println("Balance(0000001): "+atm.getBalance(getAccountInfo(0000001, 1234)));
         System.out.println("Balance(0000002): "+atm.getBalance(getAccountInfo(0000002, 2345)));
         System.out.println("Balance(0000003): "+atm.getBalance(getAccountInfo(0000003, 3456)));
      } catch (Exception e) {
         e.printStackTrace();
      }
   }
   public static void performTestOne(ATM atm) {       
      try {
         atm.getBalance(getAccountInfo(0000001, 5555));
      } catch (Exception e) {
         System.out.println("Failed as expected: "+e);
      }
   }
   public static void performTestTwo(ATM atm) {       
      try {
         atm.withdraw(getAccountInfo(0000002, 2345), 500);
      } catch (Exception e) {
         System.out.println("Failed as expected: "+e);
      }
   }
   public static void performTestThree(ATM atm) {        
      try {
         atm.withdraw(getAccountInfo(0000001, 1234), 50);
      } catch (Exception e) {
         System.out.println("Failed as expected: "+e);
      }
   }
   public static void performTestFour(ATM atm) {         
      try {
         atm.deposit(getAccountInfo(0000001, 1234), 500);
      } catch (Exception e) {
         System.out.println("Unexpected error: "+e);
      }
   }
   public static void performTestFive(ATM atm) {         
      try {
         atm.deposit(getAccountInfo(0000002, 2345), 100);
      } catch (Exception e) {
         System.out.println("Unexpected error: "+e);
      }
   }
   public static void performTestSix(ATM atm) {       
      try {
         atm.withdraw(getAccountInfo(0000001, 1234), 100);
      } catch (Exception e) {
         System.out.println("Unexpected error: "+e);
      }
   }
   public static void performTestSeven(ATM atm) {        
      try {
         atm.withdraw(getAccountInfo(0000003, 3456), 300);
      } catch (Exception e) {
         System.out.println("Unexpected error: "+e);
      }
   }
   public static void performTestEight(ATM atm) {        
      try {
         atm.withdraw(getAccountInfo(0000001, 1234), 200);
      } catch (Exception e) {
         System.out.println("Failed as expected: "+e);
      }
   }
   public static void performTestNine(ATM atm) {        
      try {
         atm.transfer(getAccountInfo(0000001, 1234),getAccountInfo(0000002, 2345), 100);
      } catch (Exception e) {
         System.out.println("Unexpected error: "+e);
      }
   }

//==============================================================================
//   END -->  VERBATIM CLIENT CODE PROVIDED AND REQUIRED BY PROFESSOR SAWYER
//==============================================================================









     public static void main(String[] args) throws RemoteException, NamingException, ATMException, NSFException, SecurityException 
    {   
    
    
        System.out.println("\nStarting test harness...\n");



     //----------------------
     //   Test Harness
     //----------------------
        ATM atm = null;
        try {
           ATMFactory factory = (ATMFactory)Naming.lookup("//localhost/atmfactory");
           atm = factory.getATM();
        } catch (MalformedURLException mue) {
           mue.printStackTrace();
        } catch (NotBoundException nbe) {
           nbe.printStackTrace();
        } catch (UnknownHostException uhe) {
           uhe.printStackTrace();
        } catch (RemoteException re) {
           re.printStackTrace();
        }
        if (atm!=null) 
	{
              // Per the spec, register this client as an ATMListener (i.e. Observer)
	      // of the ATM.  Since we are in a static context, we declare a new Client
	      // object and register it as an ATMListener.  When the ATM sends messages
	      // to this client object, they will be displayed on STDOUT.
	      //
	      // If we were in a non-static context, we could have said
	      // atm.addObserver(this).  That statement would register "this" object
	      // as the ATMListener.  We can accomplish this by moving this test
	      // harness to a non-static method, and then calling this non-static
	      // method from main().
	      //---------------------------------------------------------------------
	      Client c= new Client();
	      atm.addObserver(c);



	      // Run Professor Sawyer's test suite.
	      //------------------------------------
	      testATM(atm);

        }


	// Since this client implements the ATMListener using the
	// RemoteObserver interface, it must extend the UnicastRemoteObject.
	// Extending this object causes the thread to hang around indefinitely
	// unless it is explicitly asked to exit.  Thus, we ask it to exit
	// once the test suite has run.
	//--------------------------------------------------------------------
	System.exit(0);

    }
}

