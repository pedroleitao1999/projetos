package pt.tecnico.sauron.silo.domain;

import pt.tecnico.sauron.common.Coords;

public class Observation {
    private String nome;
    private String tipo;
    private String id;
    private String data;
    private int replica;
    private int sequenceNumber;
    private Coords coords;

    public Observation(String nome, String tipo, String id, String data, int replica, int sequenceNumber, Coords coords) {
        this.nome = nome;
        this.tipo = tipo;
        this.id = id;
        this.data = data;
        this.replica = replica;
        this.sequenceNumber = sequenceNumber;
        this.coords = coords;
    }

    public String getNome() {
        return nome;
    }

    public String getTipo() {
        return tipo;
    }

    public String getId() {
        return id;
    }

    public String getData() {
        return data;
    }

    public int getReplica(){
        return replica;
    }

    public int getSequenceNumber() {
        return sequenceNumber;
    }

    public Coords getCoords() {
        return coords;
    }

    @Override
    public String toString() {
        String observacao = "";
        return observacao+tipo+","+id+","+data+","+nome+","+coords.getLat()+","+coords.getLon();
    }

}
