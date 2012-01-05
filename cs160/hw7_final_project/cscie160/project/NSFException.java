package cscie160.project;

/**
 * An exception for signaling Insufficient Funds.
 */
public class NSFException extends Exception 
{
    public NSFException(String msg) 
    {   super(msg);
    }
}
