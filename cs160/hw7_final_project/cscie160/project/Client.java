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
    {   System.out.println("\nStarting test harness...\n");



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
        if (atm!=null) {

              // Per the spec, register this client as an ATMListener (i.e. Observer)
	      // of the ATM.
	      Client c= new Client();
	      atm.addObserver(c);
	      testATM(atm);



/*

           try {
              // Specify account credentials
	      AccountInfo acct1= getAccountInfo(1,1234);
	      AccountInfo acct2= getAccountInfo(2,2345);
	      AccountInfo acct3= getAccountInfo(3,3456);


              // get initial account balances
              System.out.println("Initial Balances");
              System.out.println("Balance(acct1): "+atm.getBalance(acct1));
              System.out.println("Balance(acct2): "+atm.getBalance(acct2));
              System.out.println("Balance(acct3): "+atm.getBalance(acct3));
              System.out.println();



              // make $1000 depoist in acct2 and get new balance
              System.out.println("Depositting(acct1): 1000 ");
              atm.deposit(acct1, 1000);
              System.out.println("Depositting(acct2): 1000 ");
              atm.deposit(acct2, 1000);


              // make $20 transfer from acct3 to acct1 and get new balance
              System.out.println("transferring( acct3 --> acct1): 20 ");
              atm.transfer(acct3, acct1, 20);


              //System.out.println("transferring( acct1 --> acct2): 500 ");
              atm.transfer(acct1, acct2, 50);


              // withdraw $100 from acct1
              System.out.println("withdrawing(acct1): 100 ");
              atm.withdraw(acct1, 10);
              atm.withdraw(acct3, 501);



              // get final account balances
              System.out.println();
              System.out.println("Final Balances");
              System.out.println("Balance(acct1): "+atm.getBalance(acct1));
              System.out.println("Balance(acct2): "+atm.getBalance(acct2));
              System.out.println("Balance(acct3): "+atm.getBalance(acct3));
              System.out.println();


           } 
	   catch (RemoteException re) 
	   {   System.out.println("An exception occurred while communicating with the ATM");
               re.printStackTrace();
	       System.exit(1);
           }
	   catch (ATMException atme)
	   {   atme.printStackTrace();
	       System.exit(1);
	   }
	   catch (NSFException nsfe)
	   {   nsfe.printStackTrace();
	       System.exit(1);
	   }
	   catch (SecurityException sece)
	   {   sece.printStackTrace();
	       System.exit(1);
	   }

*/

        }
	System.exit(0);

    }
}

