package cscie160.project;
import java.rmi.*;

/**
 * Interface for an ATM.
 */
public interface Bank extends Remote
{
    // Method Members
    public Account  getAccount(int account) throws RemoteException;
    public void     addAccount(float balance) throws RemoteException;
}
