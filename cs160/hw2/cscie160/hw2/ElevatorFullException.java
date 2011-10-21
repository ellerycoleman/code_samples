/*----------------------------------------------------------------------------
# File:		ElevatorFullException.java
# Date:		Fri Oct 21 02:40:11 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements a custom exception for cscie160, hw2.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw2;



//-----------------------
//   Custom Exception
//-----------------------
public class ElevatorFullException extends Exception
{   

   /**
    * An exception to signal that the elevator has reached
    * maximum capacity and cannot board any more passengers.
    */
    public ElevatorFullException(String msg)
    {   super("ElevatorFullException: " + msg);
    }
}



