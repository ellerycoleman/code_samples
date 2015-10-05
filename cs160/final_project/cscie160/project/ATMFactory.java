/*----------------------------------------------------------------------------
# File:		ATMFactory.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Provides an ATMFactory interface for cscie160, final project.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/


package cscie160.project;
import java.rmi.*;


/**
 * Interface for an ATMFactory.
 */
public interface ATMFactory extends Remote
{
    // Method Members
    public ATM getATM() throws ATMException, RemoteException;
    public ATM getATM(int atmNumber) throws ATMException, RemoteException;
}
