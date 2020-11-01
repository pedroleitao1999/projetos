package pt.tecnico.sauron.silo.info;

import pt.tecnico.sauron.silo.grpc.ObservationUpdate;

import java.util.ArrayList;
import java.util.List;

public class Info {

    private List<Integer> timestamp = new ArrayList();
    private List<ObservationUpdate> observationUpdates = new ArrayList();

    public Info(List<Integer> timestamp, List<ObservationUpdate> observationUpdates) {
        this.timestamp = timestamp;
        this.observationUpdates = observationUpdates;
    }

    public List<Integer> getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(List<Integer> timestamp) {
        this.timestamp = timestamp;
    }

    public List<ObservationUpdate> getObservationUpdates() {
        return observationUpdates;
    }

    public void setObservationUpdates(List<ObservationUpdate> observationUpdates) {
        this.observationUpdates = observationUpdates;
    }

}
