/*----------------------------------------------------------------------------
# File:		Server.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements a Server for cscie160, hw5.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw5;
import java.rmi.*;
import javax.naming.*;


public class Server
{   /**
     * This server program instantiates a remote ATMFactory object and
     * registers it with the naming service.
     */
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
