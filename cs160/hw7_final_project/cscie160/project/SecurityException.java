package cscie160.project;

/**
 * An exception for signaling an authentication or authorization issue.
 */
public class SecurityException extends Exception 
{
    public SecurityException(String msg) 
    {   super(msg);
    }
}
