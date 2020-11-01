package pt.tecnico.sauron.silo;

import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.tecnico.sauron.silo.gossip.*;
import java.util.*;

import java.io.IOException;

public class SiloServerApp {

	public static void main(String[] args) throws IOException, InterruptedException, ZKNamingException {
		System.out.println(SiloServerApp.class.getSimpleName());
		
		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

		// check arguments
		if (args.length < 5) {
			System.err.println("Argument(s) missing!");
			System.err.printf("Usage: java %s port%n", Server.class.getName());
			System.err.printf("Usage: java %s zooHost zooPort host port path%n", Server.class.getName());
			return;
		}

		final String zooHost = args[0];
		final String zooPort = args[1];
		final String host = args[2];
		final String port = args[3];
		final String path = args[4];
		final String gossipTime = args[5];

		int time = Integer.parseInt(gossipTime);
		int position = path.length() - 1;
		char lastLetter = path.charAt(position);
		int replica = Character.getNumericValue(lastLetter);

		SiloServerImpl serverImpl = new SiloServerImpl(zooHost, zooPort, replica);
		serverImpl.setReplica(replica);
		final BindableService impl = serverImpl;
		ZKNaming zkNaming = null;
		SiloSauron sauron = serverImpl.getSauron();
		Timer timer = new Timer();
		timer.schedule(new Gossip(zooHost, zooPort, sauron), 0, time);

		try {
			zkNaming = new ZKNaming(zooHost, zooPort);
			zkNaming.rebind(path, host, port);

			Server server = ServerBuilder.forPort(Integer.parseInt(port)).addService(impl).build();

			server.start();

			// Server threads are running in the background.
			System.out.println("Server started");

			// Create new thread where we wait for the user input.
			new Thread(() -> {
				System.out.println("<Press enter to shutdown>");
				new Scanner(System.in).nextLine();
				server.shutdown();
			}).start();

			// Do not exit the main thread. Wait until server is terminated.
			server.awaitTermination();

		} finally {
			if(zkNaming != null) {
				zkNaming.unbind(path, host, String.valueOf(port));
				System.exit(0);
			}
		}

	}

}
