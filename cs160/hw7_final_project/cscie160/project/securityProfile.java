/*----------------------------------------------------------------------------
# File:		securityProfile.java
# Date:		Mon Jan  2 12:54:22 GMT 2012
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements customerProfile class for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.io.Serializable;
import java.util.*;




/**
 * The securityProfile class stores user authentication and authorization data.
 */
public class securityProfile implements Serializable
{


    //---------------------
    //  Constants
    //---------------------
    public enum action { BALANCE, DEPOSIT, WITHDRAW };



    //-----------------
    // 3 Data Members
    //-----------------
    private int accountNumber;
    private int pin;
    private boolean permissions[];




    //-----------------
    // Constructor
    //-----------------
    public securityProfile(int acctNum, int pinCode)
    {   accountNumber= acctNum;
        pin= pinCode;
	permissions= new boolean[] {true,true,true};  //permissions default to true
    }



    //----------------
    // Method Members
    //----------------

   /**
    * Returns the accountNumber.
    */
    public int getAccountNumber()
    {   return accountNumber;
    }


   /**
    * Returns the pin number.
    */
    public int getPinNumber()
    {   return pin;
    }


   /**
    * Returns balance permissions
    */
    public boolean balanceAllowed()
    {   return permissions[action.BALANCE.ordinal()];
    }


   /**
    * Returns deposit permissions
    */
    public boolean depositAllowed()
    {   return permissions[action.DEPOSIT.ordinal()];
    }


   /**
    * Returns withdraw permissions
    */
    public boolean withdrawAllowed()
    {   return permissions[action.WITHDRAW.ordinal()];
    }
    


   /**
    * Sets permissions to allow balance inquiries
    */
    public void allowBalance()
    {   permissions[action.BALANCE.ordinal()]= true;
    }



   /**
    * Sets permissions to deny balance inquiries
    */
    public void denyBalance()
    {   permissions[action.BALANCE.ordinal()]= false;
    }



   /**
    * Sets permissions to allow deposits
    */
    public void allowDeposit()
    {   permissions[action.DEPOSIT.ordinal()]= true;
    }



   /**
    * Sets permissions to deny deposits
    */
    public void denyDeposit()
    {   permissions[action.DEPOSIT.ordinal()]= false;
    }




   /**
    * Sets permissions to allow withdrawals
    */
    public void allowWithdraw()
    {   permissions[action.WITHDRAW.ordinal()]= true;
    }



   /**
    * Sets permissions to deny withdrawals
    */
    public void denyWithdraw()
    {   permissions[action.WITHDRAW.ordinal()]= false;
    }








    // test harness
    public static void main(String args[])
    {   securityProfile s= new securityProfile(1,1234);
        System.out.println("acct num: " + s.getAccountNumber());
        System.out.println(" pin num: " + s.getPinNumber());
        System.out.println(" balance: " + s.balanceAllowed());
        System.out.println(" deposit: " + s.depositAllowed());
        System.out.println("withdraw: " + s.withdrawAllowed());


	System.out.println("\n\n\nFlippin Bits...\n\n");

        s.denyBalance();
        System.out.println(" balance: " + s.balanceAllowed());
        s.allowBalance();
        System.out.println(" balance: " + s.balanceAllowed());


        System.out.println("\n\n\n");
        s.denyDeposit();
        System.out.println(" deposit: " + s.depositAllowed());
        s.allowDeposit();
        System.out.println(" deposit: " + s.depositAllowed());



        System.out.println("\n\n\n");
        s.denyWithdraw();
        System.out.println("withdraw: " + s.withdrawAllowed());
        s.allowWithdraw();
        System.out.println("withdraw: " + s.withdrawAllowed());


    }
}
