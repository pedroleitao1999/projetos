package pt.tecnico.sauron.silo;

import pt.tecnico.sauron.common.*;
import pt.tecnico.sauron.silo.domain.Observation;
import pt.tecnico.sauron.silo.grpc.*;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.*;

public class SiloSauron {

    private int replica;
    private HashMap<String, Coords> cameras;
    private ArrayList<Observation> observations;
    private List<Integer> timestamp;

    public SiloSauron(int replica, HashMap<String, Coords> cameras, ArrayList<Observation> observations, List<Integer> timestamp) {
        this.replica = replica;
        this.cameras = cameras;
        this.observations = observations;
        this.timestamp = timestamp;
    }

    public void setReplica(int replicaNew){
        this.replica = replicaNew;
    }

    public int getReplica() {
        return replica;
    }

    public List<Integer> getTimestamp(){
        return timestamp;
    }

    public ArrayList<Observation> getObservations() {
        return observations;
    }

    public void addObservation(Observation obs) {
        observations.add(obs);
    }

    public void cam_join(String nome, double latitude, double longitude){
        Eye camera = new Eye(nome, latitude, longitude);
        cameras.put(nome, camera.getCoords());
    }

    public Coords cam_info(String nome){
        Coords c = new Coords(0, 0);
        Set<String> nomes = cameras.keySet();
        for(String n : nomes){
            if(n.equals(nome)){
                c = cameras.get(n);
                break;
            }
        }
        return c;
    }

    public void report(List<ObservationReport> observationsReport) {

        LocalDateTime dateObject = LocalDateTime.now();
        DateTimeFormatter formato = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String data = dateObject.format(formato);

        for (ObservationReport obsRep : observationsReport){
            String nome = obsRep.getNome();
            String tipo = obsRep.getTipo();
            String id = obsRep.getId();
            timestamp.set(replica - 1, timestamp.get(replica-1) + 1);
            String camNome = obsRep.getNome();
            double camLatitude = cam_info(camNome).getLat();
            double camLongitude = cam_info(camNome).getLon();
            Coords coords = new Coords(camLatitude, camLongitude);
            Observation observation = new Observation(nome, tipo, id, data, replica, timestamp.get(replica-1), coords);
            observations.add(observation);
        }

    }

    public String track(String tipo, String id){

        String observacao = "";
        Collections.reverse(observations);

        for (Observation o : observations){
            if(o.getTipo().equals(tipo) && o.getId().equals(id)){
                observacao = o.toString();
                return observacao;
            }
        }

        Collections.reverse(observations);
        return observacao;

    }

    public List<observationTrackMatch> trackMatch(String tipo, String id){

        List<observationTrackMatch> observacoes = new ArrayList<observationTrackMatch>();
        Collections.reverse(observations);
        List<String> ids = new ArrayList<String>();
        List<String> observacaoStrings = new ArrayList<String>();

        if (!id.equals("*")) {

            if (id.startsWith("*")) {
                String idEnding = id.substring(1);

                for (Observation o : observations){
                    if(o.getTipo().equals(tipo) && o.getId().endsWith(idEnding) && !ids.contains(o.getId())){
                        String observacao = o.toString();
                        observacaoStrings.add(observacao);
                        ids.add(o.getId());
                    }
                }

            } else if (id.endsWith("*")){

                String idStart = id.substring(0, id.length() - 1);

                for (Observation o : observations){
                    if(o.getTipo().equals(tipo) && o.getId().startsWith(idStart) && !ids.contains(o.getId())){
                        String observacao = o.toString();
                        observacaoStrings.add(observacao);
                        ids.add(o.getId());
                    }
                }
            }

        }
        else {

            for (Observation o : observations){
                if(o.getTipo().equals(tipo) && !ids.contains(o.getId())){
                    String observacao = o.toString();
                    observacaoStrings.add(observacao);
                    ids.add(o.getId());
                }
            }

        }
        Collections.sort(observacaoStrings);
        for (String oS : observacaoStrings){
            observationTrackMatch observation = observationTrackMatch.newBuilder().setObservacao(oS).build();
            observacoes.add(observation);
        }

        ids.clear();
        Collections.reverse(observations);
        return observacoes;

    }

    public List<observationTrace> trace(String tipo, String id) {
        List<observationTrace> observacoes = new ArrayList<observationTrace>();

        for (Observation o : observations){
            if(o.getTipo().equals(tipo) && o.getId().equals(id)){
                String observacao = o.toString();
                observationTrace observation = observationTrace.newBuilder().setObservacao(observacao).build();
                observacoes.add(observation);
            }
        }

        return observacoes;
    }

}
