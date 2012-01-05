/*----------------------------------------------------------------------------
# File:		Account.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements the Account interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;




/**
 * Interface for a bank Account.
 */
public interface Account extends Remote
{
    // Method Members
    public Float getBalance() throws RemoteException;
    public void deposit(Float f) throws RemoteException;
    public void withdraw(Float f) throws RemoteException,NSFException;
    public void setBalance(Float b) throws RemoteException;
    public int getAccountNumber() throws RemoteException;
}
