package cscie160.project;

/**
 * An exception for signaling Insufficient Funds within an Account to cover a transaction.
 */
public class NSFException extends Exception 
{
    public NSFException(String msg) 
    {   super(msg);
    }
}
