using System;
using RepoIce;

public class Client: Ice.Application
{
	public override int run(string[] argc)
	{
		// Terminate cleanly on receipt of a signal
        //
        shutdownOnInterrupt();
		
		Ice.ObjectPrx obj = communicator().stringToProxy("RepoClientIce:default -p 6667");
		RepoClientIcePrx repoClient = RepoClientIcePrxHelper.checkedCast(obj);
		if (repoClient == null)
			throw new ApplicationException("Invalid proxy");
		Console.WriteLine(repoClient.ice_id());
		
		int[] typesList = repoClient.getAllTypes();
		Console.WriteLine("Count: " + typesList.Length);
		foreach (int type in typesList)
		{
			RealTypeIcePrx realType = repoClient.getTypeById(type);
			Console.WriteLine("Type N" + type + " name: " + realType.getName()
			                  + " description " + realType.getDescription() 
			                  + " ident " + realType.ice_getIdentity().name);
		}

        if (interrupted())
            Console.Error.WriteLine(appName() + ": terminating");

        return 0;
	}
	                       

}
public class ClientIce
{
	public static void Main(string[] args)
	{
		Client app = new Client();
		Environment.Exit(app.main(args));
	}
}

