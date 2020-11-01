package pt.tecnico.sauron.spotter;

import java.util.*;
import pt.tecnico.sauron.silo.client.SiloFrontend;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;

public class SpotterApp {
	
	public static void main(String[] args) throws ZKNamingException {
        SiloFrontend frontend = null;
		System.out.println(SpotterApp.class.getSimpleName());
		
		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

        final String zooHost = args[0];
        final String zooPort = args[1];

        if(args.length == 3){
            final String replica = args[2];
            final String path = "/grpc/sauron/silo/"+replica;
            frontend = new SiloFrontend(zooHost, zooPort, path);
        } else {
            frontend = new SiloFrontend(zooHost, zooPort);
        }

        try (Scanner scanner = new Scanner(System.in)) {

            String comando;

            while (scanner.hasNextLine()) {
                comando = scanner.nextLine();
                String[] dados = comando.split(" ");

                if (dados[0].equals("spot") && dados.length == 3) {

                    String tipo = dados[1];
                    String id = dados[2];
                    if (!id.contains("*")) {
                        System.out.println(frontend.track(tipo, id));
                    } else {
                        System.out.println(frontend.trackMatch(tipo, id));
                    }

                } else if (dados[0].equals("trail") && dados.length == 3) {

                    String tipo = dados[1];
                    String id = dados[2];
                    System.out.println(frontend.trace(tipo, id));

                } else if (dados[0].equals("move") && dados[1].equals("to") && dados.length == 3) {

                    final String path = "/grpc/sauron/silo/" + dados[2];
                    frontend.switchChannel(path);
                }

            }

        }
	}

}
