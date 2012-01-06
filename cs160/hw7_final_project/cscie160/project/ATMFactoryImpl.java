/*----------------------------------------------------------------------------
# File:		ATMFactoryImpl.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the ATMFactory interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;


/**
 * Implements the ATMFactory interface.
 */

public class ATMFactoryImpl extends UnicastRemoteObject implements ATMFactory
{

    //-----------------
    // 2 Data Members
    //-----------------
    private static Map<Integer, ATMImpl> atms;
    private static int numberGenerator;


    //-------------
    // Constructor 
    //-------------


   /**
    * Default constructor to initialize atms collection.
    */
    public ATMFactoryImpl() throws RemoteException
    {   // Initialize numberGenerator and atms collection
        numberGenerator= 0000001;
        atms= new HashMap<Integer, ATMImpl>();
        

	// add one ATM for test purposes
	atms.put(numberGenerator, new ATMImpl());
	System.out.println("ATM #" + numberGenerator + " has been added to the atm collection.");
	numberGenerator++;
    }
    




    //-----------------
    // Method members
    //-----------------

   /**
    * Returns a reference to the first ATM
    */
    public ATM getATM() throws ATMException, RemoteException
    {   return atms.get(1);
    }


   /**
    * Returns a reference to the specified ATM
    */
    public ATM getATM(int atmNumber) throws ATMException, RemoteException
    {   return atms.get(atmNumber);
    }

}


