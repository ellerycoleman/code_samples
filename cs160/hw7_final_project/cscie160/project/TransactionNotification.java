/*----------------------------------------------------------------------------
# File:		TransactionNotification.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the TransactionNotification object for cscie160,
#               final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.io.Serializable;





public class TransactionNotification implements Serializable
{
    //----------------
    // 1 Data Member
    //----------------
    String message;



    //----------------
    // Constructor
    //----------------
    public TransactionNotification(String mesg)
    {   message= mesg;
    }



    //----------------
    // Method Member
    //----------------
    public String toString()
    {   return message;
    }
}
