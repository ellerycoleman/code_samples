/*----------------------------------------------------------------------------
# File:		BankServer.java
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
 * This server program instantiates remote BankImpl and SecurityImpl objects
 * and registers them with the naming service.
 */
public class BankServer
{   
     public static void main(String args[]) throws RemoteException, NamingException
     {   
         System.out.println("Initializing Bank...");
         try
	 {   BankImpl bank= new BankImpl();
	     Naming.rebind("//localhost/bank", bank);
	     System.out.println("bank bound in registry...");
         }
	 catch (Exception e)
	 {   System.out.println("BankServer error: " + e.getMessage());
	     e.printStackTrace();
	 }



         System.out.println("Initializing Security Service...");
         try
	 {   SecurityImpl securityService= new SecurityImpl();
	     Naming.rebind("//localhost/security", securityService);
	     System.out.println("Security service bound in registry...");
         }
	 catch (Exception e)
	 {   System.out.println("BankServer error: " + e.getMessage());
	     e.printStackTrace();
	 }

     }
}
