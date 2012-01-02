/*----------------------------------------------------------------------------
# File:		Security.java
# Date:		Mon Jan  2 12:10:15 GMT 2012
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Provides the Security interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;




/**
 * Interface for Security service.
 */
public interface Security extends Remote
{
    //----------------
    // Method Members
    //----------------
    public boolean validAuth(AccountInfo a) throws RemoteException;
    public boolean depositAllowed(AccountInfo a) throws RemoteException;
    public boolean withdrawAllowed(AccountInfo a) throws RemoteException;
    public boolean BalanceAllowed(AccountInfo a) throws RemoteException;
}
