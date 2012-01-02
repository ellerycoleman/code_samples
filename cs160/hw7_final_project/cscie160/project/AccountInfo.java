/*----------------------------------------------------------------------------
# File:		AccountInfo.java
# Date:		Mon Jan  2 12:10:15 GMT 2012
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Provides the AccountInfo class for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.io.Serializable;




/**
 * AccountInfo class for user authentication and authorization.
 */
public class AccountInfo implements Serializable
{
    //-----------------
    // 2 Data Memebers
    //-----------------
    private int accountNumber;
    private int pin;


    //-----------------
    // Constructor
    //-----------------
    public AccountInfo(int acctNum, int pinCode)
    {   accountNumber= acctNum;
        pin= pinCode;
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
}
