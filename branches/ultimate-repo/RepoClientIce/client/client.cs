using System;
using RepoIce;

public class ClientIce
{
	public static void Main(string[] args)
	{
		int status = 0;
		Ice.Communicator ic = null;
		try {
			ic = Ice.Util.initialize(ref args);
			Ice.ObjectPrx obj = ic.stringToProxy("RepoClientIce:default -p 6667");
			RepoClientIcePrx repoClient = RepoClientIcePrxHelper.checkedCast(obj);
			if (repoClient == null)
				throw new ApplicationException("Invalid proxy");
			Console.WriteLine(repoClient.ice_id());
		} 
		catch (Exception e) {
			Console.Error.WriteLine(e);
			status = 1;
		}
		
		if (ic != null) {
			// Clean up
			try {
				 ic.destroy();
		 	} 
			catch (Exception e) {
				Console.Error.WriteLine(e);
				status = 1;
			}
		}
		Environment.Exit(status);
	}
}

