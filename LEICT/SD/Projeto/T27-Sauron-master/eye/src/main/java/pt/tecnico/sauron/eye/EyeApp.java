package pt.tecnico.sauron.eye;

import java.util.*;

import pt.tecnico.sauron.silo.client.SiloFrontend;
import pt.tecnico.sauron.silo.grpc.*;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;

public class EyeApp {

	public static void main(String[] args) throws ZKNamingException {
		System.out.println(EyeApp.class.getSimpleName());

		// receive and print arguments
		System.out.printf("Received %d arguments%n", args.length);
		for (int i = 0; i < args.length; i++) {
			System.out.printf("arg[%d] = %s%n", i, args[i]);
		}

		runCam(args);
	}

	private static void runCam(String[] args) throws ZKNamingException {

		final String zooHost = args[0];
		final String zooPort = args[1];
		final String nome = args[2];
		final double latitude = Double.parseDouble(args[3]);
		final double longitude = Double.parseDouble(args[4]);

		SiloFrontend frontend = null;

		if(args.length == 6) {
			final String replica = args[5];
			final String path = "/grpc/sauron/silo/"+replica;
			frontend = new SiloFrontend(zooHost, zooPort, path);
		}
		else {
			frontend = new SiloFrontend(zooHost, zooPort);
		}

		frontend.camJoin(nome, latitude, longitude);
		try (Scanner scanner = new Scanner(System.in)) {

			String comando;
			List<ObservationReport> observationReports = new ArrayList<ObservationReport>();
			ObservationReport observationReport;

			while(scanner.hasNextLine()) {

				comando = scanner.nextLine();

				if(!comando.isBlank()) {

					if (!comando.startsWith("#")) {

						String[] dados = comando.split(",");
						if (dados[0].equals("zzz")) {
							int pausa = Integer.parseInt(dados[1]);
							try {
								Thread.sleep(pausa);
							} catch (InterruptedException ex) {
							}
						} else if (dados[0].equals("person") || dados[0].equals("car")) {
							observationReport = ObservationReport.newBuilder().setNome(nome).setTipo(dados[0]).setId(dados[1]).build();
							observationReports.add(observationReport);
						}
					}
				}
				else {
					frontend.report(observationReports);
					observationReports.clear();
				}

			}

		}

	}

}