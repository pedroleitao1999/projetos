package pt.tecnico.sauron.silo.client;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import pt.tecnico.sauron.silo.grpc.camJoinRequest;
import pt.tecnico.sauron.silo.grpc.*;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.ulisboa.tecnico.sdis.zk.ZKRecord;
import java.util.*;
import java.util.concurrent.TimeUnit;

public class SiloFrontend {
    ManagedChannel channel;
    sauronGrpc.sauronBlockingStub stub;
    ZKNaming zkNaming;
    private Random randomGenerator;

    public SiloFrontend(String zooHost, String zooPort, String path) throws ZKNamingException {
        zkNaming = new ZKNaming(zooHost, zooPort);
        // lookup
        ZKRecord record = zkNaming.lookup(path);
        String target = record.getURI();
        channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();
        this.stub = sauronGrpc.newBlockingStub(channel);
    }

    public SiloFrontend(String zooHost, String zooPort) throws ZKNamingException {

        ZKNaming zkNaming = new ZKNaming(zooHost, zooPort);
        ArrayList<ZKRecord> listRecord= (ArrayList<ZKRecord>) zkNaming.listRecords("/grpc/sauron/silo");

        int tamanho = listRecord.size();
        int index = (int)(Math.random()*tamanho);

        ZKRecord record = listRecord.get(index);
        String target = record.getURI();
        channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();
        this.stub = sauronGrpc.newBlockingStub(channel);

    }


    public void camJoin(String nome, double latitude, double longitude) {
        this.stub.camJoin(camJoinRequest.newBuilder().setNome(nome).setLatitude(latitude).setLongitude(longitude).build());
    }

    public void report(List<ObservationReport> observations) {
        this.stub.report(reportRequest.newBuilder().addAllObservations(observations).build());
    }

    public trackResponse track(String tipo, String id) {
        return(this.stub.track(trackRequest.newBuilder().setTipo(tipo).setId(id).build()));
    }

    public trackMatchResponse trackMatch(String tipo, String id) {
        return(this.stub.trackMatch(trackMatchRequest.newBuilder().setTipo(tipo).setId(id).build()));
    }

    public traceResponse trace(String tipo, String id) {
        return(this.stub.trace(traceRequest.newBuilder().setTipo(tipo).setId(id).build()));
    }

    public void switchChannel(String path) throws ZKNamingException{
        channel.shutdownNow();
        ZKRecord record = zkNaming.lookup(path);
        String target = record.getURI();
        channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();
        this.stub = sauronGrpc.newBlockingStub(channel);
    }

}