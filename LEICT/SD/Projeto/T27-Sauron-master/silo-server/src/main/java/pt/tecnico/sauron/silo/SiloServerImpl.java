package pt.tecnico.sauron.silo;

import io.grpc.stub.StreamObserver;
import pt.tecnico.sauron.common.Coords;
import pt.tecnico.sauron.silo.domain.Observation;
import pt.tecnico.sauron.silo.grpc.*;
import pt.tecnico.sauron.silo.gossip.*;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class SiloServerImpl extends sauronGrpc.sauronImplBase {

    private String zooHost;
    private String zooPort;
    private int replica;
    private HashMap<String, Coords> cameras;
    private ArrayList<Observation> observations;
    private List<Integer> timestamp;
    private SiloSauron sauron;
    private Gossip gossip;

    public SiloServerImpl(String zHost, String zPort, int replica) throws ZKNamingException {
        this.zooHost = zHost;
        this.zooPort = zPort;
        this.replica = replica;
        this.cameras = new HashMap<String, Coords>();
        this.observations = new ArrayList<Observation>();
        this.timestamp = new ArrayList<Integer>();
        for(int i = 0; i < replica; i++) {
            this.timestamp.add(0);
        }
        this.sauron = new SiloSauron(replica, cameras, observations, timestamp);
        this.gossip = new Gossip(zooHost, zooPort, sauron);
    }

    public void setReplica(int replicaNew){
        sauron.setReplica(replicaNew);
    }

    public SiloSauron getSauron(){
        return sauron;
    }

    @Override
    public void camJoin(camJoinRequest request, StreamObserver<camJoinResponse> responseObserver) {

        camJoinResponse response = camJoinResponse.newBuilder().build();
        sauron.cam_join(request.getNome(), request.getLatitude(), request.getLongitude());

        // Send a single response through the stream.
        responseObserver.onNext(response);

        // Notify the client that the operation has been completed.
        responseObserver.onCompleted();

    }

    @Override
    public void camInfo(camInfoRequest request, StreamObserver<camInfoResponse> responseObserver) {

        camInfoResponse response = camInfoResponse.newBuilder().setLatitude(sauron.cam_info(request.getNome()).getLat()).setLongitude(sauron.cam_info(request.getNome()).getLon()).build();

        // Send a single response through the stream.
        responseObserver.onNext(response);

        // Notify the client that the operation has been completed.
        responseObserver.onCompleted();

    }

    @Override
    public void report(reportRequest request, StreamObserver<reportResponse> responseObserver) {

        reportResponse response = reportResponse.newBuilder().build();

        sauron.report(request.getObservationsList());

        responseObserver.onNext(response);

        responseObserver.onCompleted();

    }

    @Override
    public void track(trackRequest request, StreamObserver<trackResponse> responseObserver) {

        trackResponse response = trackResponse.newBuilder().setObservacao(sauron.track(request.getTipo(), request.getId())).build();

        responseObserver.onNext(response);

        responseObserver.onCompleted();

    }

    @Override
    public void trackMatch(trackMatchRequest request, StreamObserver<trackMatchResponse> responseObserver) {

        trackMatchResponse response = trackMatchResponse.newBuilder().addAllObservations(sauron.trackMatch(request.getTipo(), request.getId())).build();

        responseObserver.onNext(response);

        responseObserver.onCompleted();

    }

    @Override
    public void trace(traceRequest request, StreamObserver<traceResponse> responseObserver) {

        traceResponse response = traceResponse.newBuilder().addAllObservations(sauron.trace(request.getTipo(), request.getId())).build();

        responseObserver.onNext(response);

        responseObserver.onCompleted();

    }

    @Override
    public void update(updateRequest request, StreamObserver<updateResponse> responseObserver) {

        updateResponse response = updateResponse.newBuilder().addAllTimestamp(gossip.receive(request.getTimestampList()).getTimestamp()).addAllObservationUpdates(gossip.receive(request.getTimestampList()).getObservationUpdates()).build();

        responseObserver.onNext(response);

        responseObserver.onCompleted();

    }

}


