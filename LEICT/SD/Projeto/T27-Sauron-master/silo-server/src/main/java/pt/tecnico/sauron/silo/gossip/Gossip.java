package pt.tecnico.sauron.silo.gossip;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import pt.tecnico.sauron.common.Coords;
import pt.tecnico.sauron.silo.domain.Observation;
import pt.tecnico.sauron.silo.grpc.*;
import pt.tecnico.sauron.silo.info.Info;
import pt.ulisboa.tecnico.sdis.zk.ZKNaming;
import pt.ulisboa.tecnico.sdis.zk.ZKNamingException;
import pt.ulisboa.tecnico.sdis.zk.ZKRecord;
import pt.tecnico.sauron.silo.*;
import java.util.*;

public class Gossip extends TimerTask{

    private String zooHost;
    private String zooPort;
    private SiloSauron sauron;


    public Gossip(String zHost, String zPort, SiloSauron siloSauron) throws ZKNamingException {
        zooHost = zHost;
        zooPort = zPort;
        sauron = siloSauron;
    }

    public void run() {

        try {
            update(sauron.getTimestamp());
        } catch (ZKNamingException e) {
            e.printStackTrace();
        }

    }

    public void update(List<Integer> timestamp) throws ZKNamingException {

        ManagedChannel channel;
        sauronGrpc.sauronBlockingStub stub;
        ZKNaming zkNaming = new ZKNaming(zooHost, zooPort);
        ArrayList<ZKRecord> listRecord;
        listRecord = new ArrayList<ZKRecord>(zkNaming.listRecords("/grpc/sauron/silo"));
        int myReplica = sauron.getReplica();

        for(ZKRecord zk : listRecord){

            String thisPath = zk.getPath();
            int position = thisPath.length() - 1;
            char lastLetter = thisPath.charAt(position);
            int replica = Character.getNumericValue(lastLetter);

            if(replica != myReplica) {
                String target = zk.getURI();
                channel = ManagedChannelBuilder.forTarget(target).usePlaintext().build();
                stub = sauronGrpc.newBlockingStub(channel);
                updateResponse updateResp = stub.update(updateRequest.newBuilder().addAllTimestamp(timestamp).build());
                List<Integer> newTimestamp = updateResp.getTimestampList();
                List<ObservationUpdate> newObservationUpdates = updateResp.getObservationUpdatesList();
                convertToObservationsAndAdd(newObservationUpdates);
            }

        }
    }

    public Info receive(List<Integer> timestamp) {

        List<Integer> myTimestamp = sauron.getTimestamp();
        ArrayList<Observation> myObservations = sauron.getObservations();
        ArrayList<ObservationUpdate> newObservationUpdates = new ArrayList<ObservationUpdate>();

        if(myTimestamp.size() > timestamp.size()) {
            for(int i = timestamp.size(); i < myTimestamp.size(); i++){
                timestamp.add(0);
            }
        }
        else if(timestamp.size() > myTimestamp.size()) {
            for(int i = myTimestamp.size(); i < timestamp.size(); i++){
                myTimestamp.add(0);
            }
        }

        List<Integer> newTimestamp = new ArrayList<Integer>();
        for(int i = 0; i < timestamp.size(); i++){
            if(myTimestamp.get(i) > timestamp.get(i)){
                for(Observation obs : myObservations){
                    if(obs.getReplica() == i + 1 && obs.getSequenceNumber() > timestamp.get(i)){
                        double latitude = obs.getCoords().getLat();
                        double longitude = obs.getCoords().getLon();
                        ObservationUpdate obsUpd = ObservationUpdate.newBuilder().setNome(obs.getNome()).setTipo(obs.getTipo()).setId(obs.getId()).setData(obs.getData()).setReplica(obs.getReplica()).setSequenceNumber(obs.getSequenceNumber()).setLatitude(latitude).setLongitude(longitude).build();
                        newObservationUpdates.add(obsUpd);
                    }
                }
                newTimestamp.add(myTimestamp.get(i));
            }
            else {
                newTimestamp.add(timestamp.get(i));
            }
        }
        timestamp = newTimestamp;

        Info info = new Info(timestamp, newObservationUpdates);
        return info;

    }

    public void convertToObservationsAndAdd(List<ObservationUpdate> observationUpdates){
        for(ObservationUpdate obsUpd : observationUpdates){
            Coords coords = new Coords(obsUpd.getLatitude(), obsUpd.getLongitude());
            Observation obs = new Observation(obsUpd.getNome(), obsUpd.getTipo(), obsUpd.getId(), obsUpd.getData(), obsUpd.getReplica(), obsUpd.getSequenceNumber(), coords);
            sauron.addObservation(obs);
        }
    }

}
