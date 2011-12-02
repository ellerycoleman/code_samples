package cscie160.hw6;

/**
 * Interface for an ATM.
 */
public interface ATM 
{
	public void deposit(float amount) throws ATMException;
	public void withdraw(float amount) throws ATMException;
	public Float getBalance() throws ATMException;
}
