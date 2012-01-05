/*----------------------------------------------------------------------------
# File:		ATMListener.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Provides ATMListener interface for cscie160, final project.
#               This interface extends Java's native Observer pattern to work
#               with RMI.  Full description here:
# http://sites.google.com/site/jamespandavan/Home/java/sample-remote-observer-based-on-rmi
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.project;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ATMListener extends Remote 
{   void update(Object observable, Object updateMsg) throws RemoteException;
}
