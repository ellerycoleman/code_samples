/*----------------------------------------------------------------------------
# File:		ATMListener.java
# Date:		Wed Jan  4 02:34:05 GMT 2012
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Defines the ATMListener interface for the final project.
#-----------------------------------------------------------------------------
# Revision: $Id: ATMImpl.java 65 2011-11-24 08:49:27Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.*;
import java.util.*;



/**
 * Remote interface for an ATMListener.
 */
public interface ATMListener extends Remote 
{
    // Method Members
    public void  registerATMListener(Observer o) throws RemoteException;
    public void  handleTXNotification(Observable obj, Object arg) throws RemoteException;
}
