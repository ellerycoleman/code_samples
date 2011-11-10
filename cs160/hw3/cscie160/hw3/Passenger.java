/*----------------------------------------------------------------------------
# File:		Passenger.java
# Date:		Sun Nov  6 19:05:16 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Passenger class for cscie160, hw3.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw3;





public class Passenger
{

    //-----------------------
    //    3 Data Members
    //-----------------------
    private String  name;  //helpful for debugging
    private int     currFloor;
    private int     destFloor;





    //-----------------------
    //     Constructors
    //-----------------------



   /**
    * Default constructor
    */
    public Passenger()
    {   this.name      = null;
        this.currFloor = 0;
        this.destFloor = 0;
    }



   /**
    * A constructor that instantiates passengers according
    * to input parameters.
    */
    public Passenger(String name, int currFloor, int destFloor)
    {   this.name      = name;
        this.currFloor = currFloor;
        this.destFloor = destFloor;
    }





    //-----------------------
    //     Method Members
    //-----------------------


    /*---------------------------------------------------------------------
    | method name: getCurrFloor
    | return type: int
    | param  type: none
    |    Abstract: getter for currFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the value of currFloor.
    */
    public int getCurrFloor()
    {   return currFloor;
    }


    /*---------------------------------------------------------------------
    | method name: setCurrFloor
    | return type: void
    | param  type: int (current floor number)
    |    Abstract: setter for currFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Sets the currFloor according to the value of the parameter.
    */
    public void setCurrFloor(int currFloor)
    {   this.currFloor= currFloor;
    }


    /*---------------------------------------------------------------------
    | method name: getDestFloor
    | return type: int
    | param  type: none
    |    Abstract: getter for destFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the value of destFloor.
    */
    public int getDestFloor()
    {   return destFloor;
    }



    /*---------------------------------------------------------------------
    | method name: setDestFloor
    | return type: void
    | param  type: int (destination floor number)
    |    Abstract: getter for destFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Sets destFloor according to the parameter.
    */
    public void setDestFloor(int destFloor)
    {   this.destFloor= destFloor;
    }



    /*---------------------------------------------------------------------
    | method name: getName
    | return type: String
    | param  type: none
    |    Abstract: getter for name.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the value of name.
    */
    public String getName()
    {   return name;
    }




    /*---------------------------------------------------------------------
    | method name: setName
    | return type: void
    | param  type: String (name)
    |    Abstract: setter for name.
    +--------------------------------------------------------------------*/
   /**
    *  Sets the name according to the parameter.
    */
    public void setName(String name)
    {   this.name= name;
    }




    /*---------------------------------------------------------------------
    | method name: arrive
    | return type: void 
    | param  type: int (current floor number)
    |    Abstract: Updates the passenger's state upon arrival; sets
    |              currFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Updates the passenger's state upon arrival; sets currFloor.
    */
    public void arrive(int floorNum)
    {   currFloor= floorNum;
    }




    /*---------------------------------------------------------------------
    | method name: toString
    | return type: String
    | param  type: void
    |    Abstract: Returns a string containing Passenger's status.
    +--------------------------------------------------------------------*/
   /**
    *  Returns a String containing Passenger's status.
    */
    public String toString()
    {   String status= "+--- " + name + " ---\n";
        status+= "| currFloor: " + currFloor + "\n";
        status+= "| destFloor: " + destFloor + "\n";
	status+= "+----------------";
	return status;
    }







    //----------------------------------
    //     Main Method (test harness)
    //----------------------------------
   /**
    * A simple test harness for the Passenger class.
    */
    public static void main(String args[])
    {
        System.out.println("Running test harness...\n");

        // Create a few passengers and display their state.
        Passenger   devin= new Passenger("devin",3,5);
        Passenger vanessa= new Passenger("vanessa",2,4);


        // Display passenger status
	System.out.println(devin.toString());
	System.out.println("\n\n");
	System.out.println(vanessa.toString());

    }
}


