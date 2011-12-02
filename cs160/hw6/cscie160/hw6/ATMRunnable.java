/*----------------------------------------------------------------------------
# File:		ATMRunnable.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an ATMRunnable class for cscie160, hw6.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw6;


import java.io.*;


/**
 * Implements ATMRunnable; a work order object for the threaded ATM.
 */

public class ATMRunnable implements Runnable
{


    //----------------
    // 3 Data Members
    //----------------
    private String command;           // transaction string that server read from client
    private ATM atm;                  // reference to ATM object
    private PrintStream printstream;  // object to write results back to client





    //---------------
    // Constructor
    //---------------

   /**
    * A default constructor that sets the account balance to 0.
    */
    public ATMRunnable(String c, ATM a, PrintStream p)
    {   command     = c;
        atm         = a;
	printstream = p;
    }





    //-----------------
    // Method Members
    //-----------------

   /**
    * Parses the request string, invokes the appropriate method
    * through the ATM interface, and writes a status message to
    * the client.
    */
    public void run()
    {   printstream.println("THIS IS THE RUN METHOD:");
        printstream.println("=======================");

    }


}
