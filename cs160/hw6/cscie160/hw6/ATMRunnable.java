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
import java.util.*;


/**
 * Implements ATMRunnable; a work order object for the threaded ATM.
 */

public class ATMRunnable
{


    //----------------
    // 3 Data Members
    //----------------
    public String command;           // transaction string that server read from client
    public ATM atm;                  // reference to ATM object
    public PrintStream printstream;  // object to write results back to client





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
    public void run() throws ATMException
    {   
    

	/*  The logic here is specific to our protocol.  We parse the string
	 *  according to that protocol.
	 */
	        String commandLine= command;
		PrintStream clientOut= printstream;


		System.out.println("Server received command: " + commandLine);


		// Break out the command line into String[]
		StringTokenizer tokenizer = new StringTokenizer(commandLine);
		String commandAndParam[] = new String[tokenizer.countTokens()];
		int index = 0;
		while (tokenizer.hasMoreTokens())
		{
			commandAndParam[index++] = tokenizer.nextToken();
		}


		// store the command
		String command = commandAndParam[0];


		// Dispatch BALANCE request without further ado.
		if (command.equalsIgnoreCase(Commands.BALANCE.toString()))
		{       System.out.println("el: Client wants to check balance...");
			clientOut.println(atm.getBalance());
			return;
		}


		// Must have 2nd arg for amount when processing DEPOSIT/WITHDRAW commands
		if (commandAndParam.length < 2)
		{   throw new ATMException("Missing amount for command \"" + command + "\"");
		}



                // execute deposit or withdraw command
		try
		{
		    float amount = Float.parseFloat(commandAndParam[1]);
		    if (command.equalsIgnoreCase(Commands.DEPOSIT.toString()))
		    {
			atm.deposit(amount);
			return;
		    }
		    else if (command.equalsIgnoreCase(Commands.WITHDRAW.toString()))
		    {
			atm.withdraw(amount);
			return;
		    }
		    else
		    {
			throw new ATMException("Unrecognized command: " + command);
		    }
		}
		catch (NumberFormatException nfe)
		{
			throw new ATMException("Unable to make float from input: " + commandAndParam[1]);
		}
		// BALANCE command returned result above.  Other commands return null;
      }






   /**
    * Returns the client command for this ATMRunnable.
    */
    public String getCommand()
    {   return command;
    }




   /**
    * Returns the ATM reference for this ATMRunnable.
    */
    public ATM getATM()
    {   return atm;
    }




   /**
    * Returns the PrintStream reference for this ATMRunnable.
    */
    public PrintStream getPrintStream()
    {   return printstream;
    }




}
