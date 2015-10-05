/*----------------------------------------------------------------------------
# File:		ATMServer.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements a Server for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;
import javax.naming.*;


/**
 * This server program instantiates an ATMFactory object and
 * registers it with the naming service.
 */
public class ATMServer
{   
     public static void main(String args[]) throws RemoteException, NamingException
     {   
         System.out.println("Initializing ATMFactory...");


         try
	 {   ATMFactoryImpl atmFactory= new ATMFactoryImpl();
	     Naming.rebind("//localhost/atmfactory", atmFactory);
	     System.out.println("atmFactory bound in registry...");
         }
	 catch (Exception e)
	 {   System.out.println("ATMServer error: " + e.getMessage());
	     e.printStackTrace();
	 }
     }
}
